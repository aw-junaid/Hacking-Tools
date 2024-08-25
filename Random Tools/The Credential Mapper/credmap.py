#!/usr/bin/env python

"""
Copyright (c) 2015-2016 Roberto Christopher Salgado Bjerre.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""

from __future__ import print_function

import re

from time import strftime
from xml.etree.ElementTree import parse, ParseError
from sys import stdout as sys_stdout
from subprocess import Popen, PIPE
from optparse import OptionParser
from getpass import getpass
from random import sample
from os import listdir, makedirs
from os.path import isfile, join, dirname, exists
from urllib2 import build_opener, install_opener, ProxyHandler
from urllib2 import HTTPCookieProcessor, HTTPHandler, HTTPSHandler, quote

from lib.website import Website
from lib.common import colorize as color, COOKIE_HANDLER as cookie_handler
from lib.settings import BW
from lib.settings import ASK, PLUS, INFO, TEST, WARN, ERROR
from lib.logger import Logger

NAME = "credmap"
VERSION = "v0.1"
URL = "https://github.com/lightos/credmap/"

# Maximum length of left option column in help listing
MAX_HELP_OPTION_LENGTH = 20

# Character used for progress rotator
ROTATOR_CHARS = "|/-\\"

BANNER_PASSWORDS = ("123456", "HUNTER2", "LOVE",
                    "SECRET", "ABC123", "GOD", "SEX")

BANNER = """               . .IIIII             .II
  I%sIIII. I  II  .    II..IIIIIIIIIIIIIIIIIIII
 .  .IIIIII  II             IIIIII%sIIIII  I.
    .IIIII.III I        IIIIIIIIIIIIIIIIIIIIIII
   .II%sII           II  .IIIII IIIIIIIIIIII. I
    IIIIII             IIII I  II%sIIIIIII I
    .II               IIIIIIIIIIIII  IIIIIIIII
       I.           .III%sIIII    I   II  I
         .IIII        IIIIIIIIIIII     .       I
          IIIII.          IIIIII           . I.
          II%sIII         IIIII             ..I  II .
           IIIIII          IIII...             IIII
            IIII           III. I            II%sII
            III             I                I  III
            II                                   I   .
             I                                        """

# Location of the folder containing the websites to test
SITES_DIR = "websites"

# Location of the folder where results will be written to
OUTPUT_DIR = "output"

# Location of file containing user agents
USER_AGENTS_FILE = "agents.txt"

# Location of Git repository
GIT_REPOSITORY = "https://github.com/lightos/credmap.git"

EXAMPLES = """
Examples:
./credmap.py --username janedoe --email janedoe@email.com
./credmap.py -u johndoe -e johndoe@email.com --exclude "github.com, live.com"
./credmap.py -u johndoe -p abc123 -vvv --only "linkedin.com, facebook.com"
./credmap.py -e janedoe@example.com --verbose --proxy "https://127.0.0.1:8080"
./credmap.py --load creds.txt --format "e.u.p"
./credmap.py -l creds.txt -f "u|e:p"
./credmap.py -l creds.txt
./credmap.py --list
"""


class AttribDict(dict):
    """
    Gets and Sets attributes for a dict.
    """
    def __getattr__(self, name):
        return self.get(name)

    def __setattr__(self, name, value):
        return self.__setitem__(name, value)

    def __init__(self, *args, **kwargs):
        self.multiple_params = None
        self.multiple_params_url = None
        dict.__init__(self, *args, **kwargs)


def get_revision():
    """
    Returns abbreviated commit hash number as retrieved with:
    "git rev-parse --short HEAD".
    """

    retval = None
    filepath = None
    _ = dirname(__file__)

    while True:
        filepath = join(_, ".git", "HEAD")
        if exists(filepath):
            break
        else:
            filepath = None
            if _ == dirname(_):
                break
            else:
                _ = dirname(_)

    while True:
        if filepath and isfile(filepath):
            with open(filepath, "r") as file_:
                content = file_.read()
                filepath = None
                if content.startswith("ref: "):
                    filepath = join(
                        _, ".git", content.replace("ref: ", "")
                        ).strip()
                else:
                    match = re.match(r"(?i)[0-9a-f]{32}", content)
                    retval = match.group(0) if match else None
                    break
        else:
            break

    if not retval:
        process = Popen("git rev-parse --verify HEAD", shell=True,
                        stdout=PIPE, stderr=PIPE)
        stdout, _ = process.communicate()
        match = re.search(r"(?i)[0-9a-f]{32}", stdout or "")
        retval = match.group(0) if match else None

    return retval[:7] if retval else None


def check_revision(version):
    """
    Adapts the default version string and banner to
    use the revision number.
    """

    revision = get_revision()

    if revision:
        version = "%s-%s" % (version, revision)

    return version


def update():
    """
    Updates the program via git pull.
    """

    print("%s Checking for updates..." % INFO)

    process = Popen("git pull %s HEAD" % GIT_REPOSITORY, shell=True,
                    stdout=PIPE, stderr=PIPE)
    stdout, stderr = process.communicate()
    success = not process.returncode

    if success:
        updated = "Already" not in stdout
        process = Popen("git rev-parse --verify HEAD", shell=True,
                        stdout=PIPE, stderr=PIPE)
        stdout, _ = process.communicate()
        revision = (stdout[:7] if stdout and
                    re.search(r"(?i)[0-9a-f]{32}", stdout) else "-")
        print("%s the latest revision '%s'." %
              ("%s Already at" % INFO if not updated else
               "%s Updated to" % PLUS, revision))
    else:
        print("%s Problem occurred while updating program.\n" % WARN)

        _ = re.search(r"(?P<error>error:[^:]*files\swould\sbe\soverwritten"
                      r"\sby\smerge:(?:\n\t[^\n]+)*)", stderr)
        if _:
            def question():
                """Asks question until a valid answer of y or n is provided."""
                print("\n%s Would you like to overwrite your changes and set "
                      "your local copy to the latest commit?" % ASK)
                sys_stdout.write("%s ALL of your local changes will be deleted"
                                 " [Y/n]: " % WARN)
                _ = raw_input()

                if not _:
                    _ = "y"

                if _.lower() == "n":
                    exit()
                elif _.lower() == "y":
                    return
                else:
                    print("%s Did not understand your answer! Try again." %
                          ERROR)
                    question()

            print("%s" % _.group("error"))

            question()

            if "untracked" in stderr:
                cmd = "git clean -df"
            else:
                cmd = "git reset --hard"

            process = Popen(cmd, shell=True, stdout=PIPE, stderr=PIPE)
            stdout, _ = process.communicate()

            if "HEAD is now at" in stdout:
                print("\n%s Local copy reset to current git branch." % INFO)
                print("%s Attemping to run update again..." % INFO)
            else:
                print("%s Unable to reset local copy to current git branch." %
                      WARN)
                exit()

            update()
        else:
            print("%s Please make sure that you have "
                  "a 'git' package installed." % INFO)
            print(stderr)


def optional_arg(arg_default):
    """
    Add support to optparse for optional argument values
    """
    def func(option, opt_str, value, parser):
        """Function sent to args parser."""
        if parser.rargs and not parser.rargs[0].startswith('-'):
            _ = parser.rargs[0]
            parser.rargs.pop(0)
        else:
            _ = arg_default
        setattr(parser.values, option.dest, _)
    return func


def parse_args():
    """
    Parses the command line arguments.
    """
    # Override epilog formatting
    OptionParser.format_epilog = lambda self, formatter: self.epilog

    parser = OptionParser(usage="usage: %prog --email EMAIL | --user USER "
                          "| --load LIST [options]",
                          epilog=EXAMPLES)

    parser.add_option("-v", "--verbose", action="count", dest="verbose",
                      help="display extra output information")

    parser.add_option("-u", "--username", dest="username",
                      help="set the username to test with")

    parser.add_option("-p", "--password", dest="password",
                      help="set the password to test with")

    parser.add_option("-e", "--email", dest="email",
                      help="set an email to test with")

    parser.add_option("-l", "--load", dest="load_file",
                      help="load list of credentials in format USER:PASSWORD")

    parser.add_option("-f", "--format", dest="cred_format",
                      help="format to use when reading from file (e.g. u|e:p)")

    parser.add_option("-x", "--exclude", dest="exclude",
                      help="exclude sites from testing")

    parser.add_option("-o", "--only", dest="only",
                      help="test only listed sites")

    parser.add_option("-s", "--safe-urls", dest="safe_urls",
                      action="store_true",
                      help="only test sites that use HTTPS")

    parser.add_option("-i", "--ignore-proxy", dest="ignore_proxy",
                      action="store_true",
                      help="ignore system default HTTP proxy")

    parser.add_option("--proxy", dest="proxy", action="callback",
                      callback=optional_arg("1"),
                      help="set proxy (e.g. \"socks5://192.168.1.2:9050\")")

    parser.add_option("--list", action="store_true", dest="list",
                      help="list available sites to test with")

    parser.add_option("--update", dest="update", action="store_true",
                      help="update from the official git repository")

    parser.formatter.store_option_strings(parser)
    parser.formatter.store_option_strings = lambda _: None

    for option, value in parser.formatter.option_strings.items():
        value = re.sub(r"\A(-\w+) (\w+), (--[\w-]+=(\2))\Z", r"\g<1>/\g<3>",
                       value)
        value = value.replace(", ", '/')
        if len(value) > MAX_HELP_OPTION_LENGTH:
            value = ("%%.%ds.." % (MAX_HELP_OPTION_LENGTH -
                                   parser.formatter.indent_increment)) % value
        parser.formatter.option_strings[option] = value

    args = parser.parse_args()[0]

    if not any((args.username, args.email, args.update,
                args.list, args.load_file)):
        parser.error("Required argument is missing. Use '-h' for help.")

    return args


def list_sites(extension=False):
    """
    List available sites for testing found in the websites folder.
    Read folder containing each website's XML files.
    """

    return [_ if extension else _.replace(".xml", "")
            for _ in listdir(SITES_DIR) if isfile(join(SITES_DIR, _))]


def populate_site(site, args):
    """
    Parse sites in XML files and return objects.
    """

    try:
        xml_tree = parse("%s/%s.xml" % (SITES_DIR, site)).getroot()
    except ParseError as parse_error:
        print("%s parsing XML file \"%s\". Skipping..." % (ERROR,
                                                           color(site, BW)))
        if args.verbose:
            print("%s: %s" % (ERROR, parse_error.message))
        print()

        return

    site_properties = AttribDict()

    for _ in xml_tree:
        if _.tag == "multiple_params":
            site_properties.multiple_params = True
            site_properties.multiple_params_url = _.attrib["value"]
            continue
        if _.tag in ("custom_search", "time_parameter", "valid_http_status",
                     "invalid_http_status", "custom_response_header"):
            site_properties[_.tag] = _.attrib
            continue
        if "value" in _.attrib:
            site_properties[_.tag] = _.attrib["value"]
        if "type" in _.attrib:
            site_properties["%s_type" % _.tag] = _.attrib["type"]

    if site_properties.multiple_params:
        site_properties.multiple_params = []
        for _ in xml_tree.getiterator('param'):
            params = {}
            for k, val in _.attrib.items():
                if val:
                    params[k] = val
            if params:
                site_properties.multiple_params.append(params)

    match = re.match(r"(?P<type>[^:]+)://[^.]+(\.\w+)*",
                     site_properties.login_url, re.I)

    if not match:
        print("%s unable to read URL for login in XML file for \"%s\". "
              "Skipping site...\n" % (ERROR, color(site_properties.name, BW)))
        return

    if args.safe_urls and match.group("type").upper() != "HTTPS":
        if args.verbose:
            print("%s URL uses an unsafe transportation mechanism: \"%s\". "
                  "Skipping site...\n" % (WARN, match.group("type").upper()))
        return

    if(not site_properties.login_parameter or
       not site_properties.password_parameter):
        print("%s current XML file is missing parameter(s) for login. "
              "Skipping site...\n" % ERROR)
        return

    return site_properties


def main():
    """
    Initializes and executes the program.
    """

    login_sucessful = []
    login_failed = []
    login_skipped = []

    version = check_revision(VERSION)

    print("%s\n\n%s %s (%s)\n" % (
        BANNER % tuple([color(_) for _ in BANNER_PASSWORDS]),
        NAME, version, URL))

    args = parse_args()

    if args.update:
        update()
        exit()

    sites = list_sites()

    if args.list:
        for _ in sites:
            print("- %s" % _)
        exit()

    if not args.password and not args.load_file:
        args.password = getpass("%s Please enter password:" % INFO)
        print()

    if args.ignore_proxy:
        proxy_handler = ProxyHandler({})
    elif args.proxy:
        match = re.search(r"(?P<type>[^:]+)://(?P<address>[^:]+)"
                          r":(?P<port>\d+)", args.proxy, re.I)
        if match:
            if match.group("type").upper() in ("HTTP", "HTTPS"):
                proxy_host = "%s:%s" % (match.group("address"),
                                        match.group("port"))
                proxy_handler = ProxyHandler({"http": proxy_host,
                                              "https": proxy_host})
            else:
                from thirdparty.socks import socks
                if match.group("type").upper() == "SOCKS4":
                    socks.setdefaultproxy(socks.PROXY_TYPE_SOCKS4,
                                          match.group("address"),
                                          int(match.group("port")), True)
                elif match.group("type").upper() == "SOCKS5":
                    socks.setdefaultproxy(socks.PROXY_TYPE_SOCKS5,
                                          match.group("address"),
                                          int(match.group("port")), True)
                proxy_handler = None
        else:
            proxy_handler = ProxyHandler()
    else:
        proxy_handler = None

    opener = build_opener(HTTPHandler(), HTTPSHandler(),
                          HTTPCookieProcessor(cookie_handler))
    if proxy_handler:
        opener.add_handler(proxy_handler)

    install_opener(opener)

    with open(USER_AGENTS_FILE, 'r') as ua_file:
        args.user_agent = sample(ua_file.readlines(), 1)[0].strip()

    if args.only:
        sites = [site for site in sites if site in args.only]
    elif args.exclude:
        sites = [site for site in sites if site not in args.exclude]

    print("%s Loaded %d %s to test." %
          (INFO, len(sites), "site" if len(sites) == 1 else "sites"))

    if args.load_file:
        if not isfile(args.load_file):
            print("%s could not find the file \"%s\"" %
                  (WARN, color(args.load_file)))
            exit()

        _ = sum(1 for line in open(args.load_file, "r"))
        if _ < 1:
            print("%s the file \"%s\" doesn't contain any valid credentials." %
                  (WARN, color(args.load_file)))
            exit()

        print("%s Loaded %d credential%s from \"%s\".\n" %
              (INFO, _, "s" if _ != 1 else "", color(args.load_file)))

    print("%s Starting tests at: \"%s\"\n" % (INFO, color(strftime("%X"), BW)))

    if not exists(OUTPUT_DIR):
        makedirs(OUTPUT_DIR)

    log = Logger("%s/credmap" % OUTPUT_DIR)
    log.open()

    def get_targets():
        """
        Retrieve and yield list of sites (targets) for testing.
        """
        for site in sites:
            _ = populate_site(site, args)
            if not _:
                continue
            target = Website(_, {"verbose": args.verbose})

            if not target.user_agent:
                target.user_agent = args.user_agent

            yield target

    def login():
        """
        Verify credentials for login and check if login was successful.
        """
        if(target.username_or_email == "email" and not
           credentials["email"] or
           target.username_or_email == "username" and not
           credentials["username"]):
            if args.verbose:
                print("%s Skipping %s\"%s\" since "
                      "no \"%s\" was specified.\n" %
                      (INFO, "[%s:%s] on " %
                       (credentials["username"] or
                        credentials["email"], credentials["password"]) if
                       args.load_file else "", color(target.name),
                       color(target.username_or_email, BW)))
                login_skipped.append(target.name)
            return

        print("%s Testing %s\"%s\"..." %
              (TEST, "[%s:%s] on " % (credentials["username"] or
                                      credentials["email"],
                                      credentials["password"]) if
               args.load_file else "", color(target.name, BW)))

        cookie_handler.clear()

        if target.perform_login(credentials, cookie_handler):
            log.write(">>> %s - %s:%s\n" %
                      (target.name, credentials["username"] or
                       credentials["email"], credentials["password"]))
            login_sucessful.append("%s%s" %
                                   (target.name, " [%s:%s]" %
                                    (credentials["username"] or
                                     credentials["email"],
                                     credentials["password"]) if
                                    args.load_file else ""))
        else:
            login_failed.append(target.name)

    if args.load_file:
        if args.cred_format:
            separators = [re.escape(args.cred_format[1]),
                          re.escape(args.cred_format[3]) if
                          len(args.cred_format) > 3 else "\n"]
            cred_format = re.match(r"(u|e|p)[^upe](u|e|p)(?:[^upe](u|e|p))?",
                                   args.cred_format)
            if not cred_format:
                print("%s Could not parse --format: \"%s\""
                      % (ERROR, color(args.cred_format, BW)))
                exit()

            cred_format = [v.replace("e", "email")
                           .replace("u", "username")
                           .replace("p", "password")
                           for v in cred_format.groups() if v is not None]

        with open(args.load_file, "r") as load_list:
            for user in load_list:
                if args.cred_format:
                    match = re.match(r"([^{0}]+){0}([^{1}]+)(?:{1}([^\n]+))?"
                                     .format(separators[0], separators[1]),
                                     user)
                    credentials = dict(zip(cred_format, match.groups()))
                    credentials["password"] = quote(
                        credentials["password"])
                    if("email" in credentials and
                       not re.match(r"^[A-Za-z0-9._%+-]+@(?:[A-Z"
                                    r"a-z0-9-]+\.)+[A-Za-z]{2,12}$",
                                    credentials["email"])):
                        print("%s Specified e-mail \"%s\" does not appear "
                              "to be correct. Skipping...\n" % (WARN, color(
                                  credentials["email"], BW)))
                        continue

                    if "email" not in credentials:
                        credentials["email"] = None
                    elif "username" not in credentials:
                        credentials["username"] = None
                else:
                    user = user.rstrip().split(":", 1)
                    if not user[0]:
                        if args.verbose:
                            print("%s Could not parse credentials: \"%s\"\n" %
                                  (WARN, color(user, BW)))
                        continue

                    match = re.match(r"^[A-Za-z0-9._%+-]+@(?:[A-Z"
                                     r"a-z0-9-]+\.)+[A-Za-z]{2,12}$", user[0])
                    credentials = {"email": user[0] if match else None,
                                   "username": None if match else user[0],
                                   "password": quote(user[1])}

                for target in get_targets():
                    login()
    else:
        credentials = {"username": args.username, "email": args.email,
                       "password": quote(args.password)}
        for target in get_targets():
            login()

    log.close()

    if not args.verbose:
        print()

    if len(login_sucessful) > 0 or len(login_failed) > 0:
        _ = "%s/%s" % (color(len(login_sucessful), BW),
                       color(len(login_sucessful) + len(login_failed), BW))
        sign = PLUS if len(login_sucessful) > (len(login_failed) +
                                               len(login_skipped)) else INFO
        print("%s Succesfully logged in%s." %
              (sign, " with %s credentials on the list." % _ if args.load_file
               else "to %s websites." % _),)
        print("%s An overall success rate of %s.\n" %
              (sign, color("%%%s" % (100 * len(login_sucessful) /
                                     (len(login_sucessful) +
                                      len(login_failed))), BW)))

    if len(login_sucessful) > 0:
        print("%s The provided credentials worked on the following website%s: "
              "%s\n" % (PLUS, "s" if len(login_sucessful) != 1 else "",
                        ", ".join(login_sucessful)))

    print("%s Finished tests at: \"%s\"\n" % (INFO, color(strftime("%X"), BW)))


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\n%s Ctrl-C pressed." % INFO)
