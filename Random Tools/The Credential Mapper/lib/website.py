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

from time import time
from re import I, sub, search, escape
from cookielib import Cookie
from urllib import urlencode, unquote_plus
from urllib2 import urlopen, Request, quote, HTTPError
from urlparse import urlsplit, urlunsplit, parse_qsl
from StringIO import StringIO
from gzip import GzipFile

from .common import colorize as color
from .settings import PLUS, INFO, WARN, ERROR, DEBUG


# Posible values in XML file
XML_ELEMENTS = ("url", "name", "description", "login_url", "invalid_account",
                "inactive_account", "valid_password", "invalid_password",
                "valid_response_header", "valid_response_header_type",
                "response_headers", "valid_http_status", "headers", "data",
                "cookies", "user_agent", "username_or_email", "custom_search",
                "login_parameter", "login_parameter_type", "multiple_params",
                "password_parameter", "password_parameter_type", "status",
                "csrf_token_name", "csrf_url", "csrf_setcookie", "csrf_start",
                "csrf_regex", "csrf_end", "captcha_flag", "email_exception",
                "multiple_params_url", "custom_response_header", "csrf_token",
                "response_status", "login_redirect", "login_redirect_type",
                "time_parameter", "invalid_http_status")


class Website(object):
    """
    Populates the Website object with data from the XML file.
    """
    def __init__(self, *data, **kwargs):
        self.url = None
        self.csrf_token = None
        self.cookies = None
        self.headers = None
        self.data = None
        self.response_status = None
        self.response_headers = None

        for dictionary in data:
            for key in dictionary:
                setattr(self, key, dictionary[key])
        for key in kwargs:
            setattr(self, key, kwargs[key])

        for _ in XML_ELEMENTS:
            if not hasattr(self, _):
                setattr(self, _, None)

        self.status = {"status": 1, "msg": "%s instantiated" % self.name}

    def get_page(self, data=False, header=False):
        """
        Retrieves page content from a given target URL
        """

        headers = {}
        parsed_url = None
        page = None
        conn = None
        invalid = False

        parsed_url = urlsplit(self.url)

        if not data:
            parsed_url = parsed_url._replace(
                query=urlencode(parse_qsl(parsed_url.query)))
            self.url = urlunsplit(parsed_url)
        elif self.login_parameter_type == "json":
            self.data = unquote_plus(self.data)
        else:
            self.data = urlencode(parse_qsl(
                self.data.replace("+", "%2B"), 1), "POST")

        try:
            headers["User-agent"] = self.user_agent

            if header:
                headers.update(dict([tuple(_.split("=", 1))
                                     for _ in self.headers.split("\\n")]))

            if self.verbose >= 2:
                print("%s REQUEST\nURL: "
                      "%s\n%sHeaders: %s\n" % (DEBUG, self.url, "DATA: %s\n" %
                                               self.data if data else "",
                                               headers))

            req = Request(self.url, self.data if data else None, headers)
            conn = urlopen(req)

        except KeyboardInterrupt:
            raise
        except HTTPError as error:
            conn = error
            if(self.valid_http_status and "*" not in
               self.valid_http_status["value"] and
               error.code == int(self.valid_http_status["value"])):
                pass
            elif(self.valid_http_status and "*" in
                 self.valid_http_status["value"] and
                 str(error.code)[0] == self.valid_http_status["value"][0]):
                pass
            if(self.invalid_http_status and "*" not in
               self.invalid_http_status["value"] and
               error.code == int(self.invalid_http_status["value"])):
                invalid = True
            elif(self.invalid_http_status and "*" in
                 self.invalid_http_status["value"] and
                 str(error.code)[0] == self.invalid_http_status["value"][0]):
                invalid = True
            else:
                if self.verbose:
                    print_http_error(error)
        except Exception, error:
            if hasattr(error, "read"):
                page = error.read()
            if self.verbose:
                print_http_error(error)

        if not page and conn and conn.info().get('Content-Encoding') == 'gzip':
            page = GzipFile(fileobj=StringIO(conn.read())).read()

        if not page and conn:
            page = conn.read()

        # NEED TO CLEAN THIS WHOLE PART UP
        self.status["status"] = 1 if conn else 0
        self.status["message"] = (conn.msg if conn and hasattr(conn, "msg")
                                  else error.msg if error and
                                  hasattr(error, "msg") else "Unknown error!")
        self.response_headers = (conn.info() if conn and hasattr(conn, "info")
                                 else error.info() if error and
                                 hasattr(error, "info") else "Unknown info!")
        self.response_status = (conn.code if conn and hasattr(conn, "code")
                                else error.code if error and
                                hasattr(error, "code") else "Unknown code!")

        if self.verbose >= 2:
            print("%s RESPONSE\nSTATUS CODE: %s\n%s" % (DEBUG,
                                                        self.response_status,
                                                        self.response_headers))
        if self.verbose >= 3:
            print "%s HTML\n%s" % (DEBUG, page or "No reponse")
        if invalid:
            page = None

        if not page and not invalid:
            page = " "

        return page

    def add_cookies(self, cookie_handler, cookies, url=None):
        """
        Add cookies to the specified cookie jar.
        Domain for the cookie can be specified via url.
        """
        domain = urlsplit(url or self.login_url)
        domain = ".%s.%s" % (domain.netloc.split(".")[-2],
                             domain.netloc.split(".")[-1])
        for _ in parse_qsl(cookies, 1):
            cookie = Cookie(version=0, name=_[0], value=_[1],
                            port=None, port_specified=False,
                            domain=domain, domain_specified=True,
                            domain_initial_dot=True, path='/',
                            path_specified=True, secure=True,
                            expires=None, discard=True, comment=None,
                            comment_url=None, rest={}, rfc2109=False)
            cookie_handler.set_cookie(cookie)

    def perform_login(self, credentials, cookie_handler):
        """
        Parses CSRF token if available and performs login request.
        """

        if self.cookies:
            self.add_cookies(cookie_handler, self.cookies)

        if self.csrf_token_name:
            self.url = self.csrf_url
            csrf_response = self.get_page()

            if not csrf_response:
                if(self.invalid_http_status and self.response_status and
                   int(self.invalid_http_status["value"]) == int(
                       self.response_status)):
                    if self.verbose:
                        print("%s %s\n" %
                              (INFO, self.invalid_http_status["msg"] if "msg"
                               in self.invalid_http_status else
                               "Your IP may have been blocked..."))
                elif self.verbose:
                    print("%s problem receiving HTTP response "
                          "while fetching token!\n" % ERROR)
                return

            if self.csrf_regex or self.csrf_setcookie:
                match = search(self.csrf_regex if self.csrf_regex else
                               self.csrf_setcookie, csrf_response if
                               self.csrf_regex else str(cookie_handler), I)
                if match:
                    self.csrf_token = (match.group("token") if "token" in
                                       match.groupdict() else match.group(1))
                else:
                    self.status = {"status": 0, "msg": "No token"}
            else:
                start = csrf_response.find(self.csrf_start)
                if start == -1 and self.verbose:  # lvl 1 verbose
                    self.status = {"status": 0, "msg": "No token"}
                end = csrf_response.find(self.csrf_end,
                                         start+len(self.csrf_start))
                self.csrf_token = csrf_response[start+len(self.csrf_start):end]

            if self.status["msg"] == "No token" or not self.csrf_token:
                if self.verbose:
                    print "%s CSRF token not found. Skipping site...\n" % WARN
                return

            if self.verbose:
                print("%s Authentication Token: \"%s\"" %
                      (INFO, color(self.csrf_token)))

        def replace_param(string, param, value, param_format=None):
            """
            Replace data in parameters with given string.
            Parameter format can be json or normal POST data.
            """

            if param_format == "json":
                return sub(r"(?P<json_replacement>\"%s\"\s*:\s*)\"\s*\"" %
                           escape(str(param)), "\\1\"%s\"" % value, string)
            elif param_format == "header":
                return sub(r"%s=[^\\n]*" % escape(str(param)), r"%s=%s" %
                           (str(param).encode('string-escape'),
                            str(value).encode('string-escape')), string)
            else:
                return sub(r"%s=[^&]*" % escape(str(param)), r"%s=%s" %
                           (str(param).encode('string-escape'),
                            str(value).encode('string-escape')), string)

        if self.multiple_params:
            multiple_params_response = ""
            if(self.csrf_token_name and
               self.csrf_url == self.multiple_params_url):
                multiple_params_response = csrf_response
            else:
                self.url = self.multiple_params_url
                multiple_params_response = self.get_page()

            if(self.invalid_http_status and self.response_status and
               int(self.invalid_http_status["value"]) == int(
                   self.response_status) and self.verbose):
                print("%s %s\n" % (INFO, self.invalid_http_status["msg"]
                                   if "msg" in self.invalid_http_status else
                                   "Your IP may have been blocked..."))
                return

            if not multiple_params_response:
                print("%s problem receiving HTTP response while fetching "
                      "params! Skipping site...\n" % ERROR)
                return

            for _ in self.multiple_params:
                regex = (_["regex"] if "regex" in _ else
                         r"<\w+[^>]*(value\s*=\s*\"[^\"]*\"|name\s*=\s*"
                         r"\"?{0}(?:\"|\s))[^>]*(value\s*=\s*\"[^\"]*\""
                         r"|name\s*=\s*\"?{0}(?:\"|\s))[^>]*>"
                         .format(escape(_["value"])))
                match = search(regex, multiple_params_response)

                if not match or "value" not in _:
                    if self.verbose:
                        print("%s no match for parameter \"%s\"! "
                              "Skipping site...\n" %
                              (WARN, color(_["value"])))
                        self.status = {"status": 0, "msg": "No token"}
                    return

                if "regex" in _:
                    value = (match.group("value")
                             if "value" in match.groupdict() else
                             match.group(1))
                elif "value" in _:
                    for attrib in match.groups():
                        attrib = str(attrib).strip().split("=", 1)
                        if attrib[0] == "value":
                            value = attrib[1].strip("\"")

                if "type" not in _:
                    _["type"] = "data"

                if _["type"] == "data" and self.data:
                    self.data = replace_param(self.data, _["value"], value)
                elif _["type"] == "header":
                    self.headers = replace_param(self.headers, _["value"],
                                                 value, "header")
                elif _["type"] == "cookie":
                    self.add_cookies(cookie_handler, "%s=%s;" % (_["value"],
                                                                 value))
                else:
                    pass  # NEED TO REPLACE GET PARAMS

        if(credentials["email"] and
           self.username_or_email in("email", "both")):
            login = credentials["email"]
        elif(credentials["email"] and self.email_exception and
             self.username_or_email == "username" and
             search(self.email_exception, credentials["email"])):
            login = credentials["email"]
        else:
            login = credentials["username"]

        # need to implement support for GET logins lulz

        if self.time_parameter:
            if "type" not in self.time_parameter:
                self.time_parameter["type"] = "epoch"

            if self.time_parameter["type"] == "epoch":
                if self.data:
                    self.data = replace_param(self.data,
                                              self.time_parameter["value"],
                                              time())

        if self.data:
            self.data = replace_param(self.data, self.login_parameter,
                                      login, self.login_parameter_type)
            self.data = replace_param(self.data, self.password_parameter,
                                      credentials["password"],
                                      self.login_parameter_type)

        # need to be able to specify where tokens can be replaced
        if self.csrf_token:
            self.csrf_token = quote(self.csrf_token)
            if self.data:
                self.data = replace_param(self.data,
                                          self.csrf_token_name,
                                          self.csrf_token,
                                          self.login_parameter_type)
            if self.headers:
                self.headers = replace_param(self.headers,
                                             self.csrf_token_name,
                                             self.csrf_token, "header")
            if self.cookies:
                self.cookies = replace_param(self.cookies,
                                             self.csrf_token_name,
                                             self.csrf_token)
                self.add_cookies(cookie_handler, self.cookies)

        self.url = self.login_url
        login_response = self.get_page(data=True if self.data else False,
                                       header=True if self.headers else False)

        if not login_response:
            if self.verbose:
                print("%s no response received! Skipping site...\n" % WARN)
            return False

        if self.login_redirect:
            if self.login_redirect_type == "regex":
                self.url = search(self.login_redirect, login_response)
                self.url = (self.url.group("URL")
                            if "URL" in self.url.groupdict()
                            else self.url.group(1))
            else:
                self.url = self.login_redirect

            self.url = self.url.replace("\\", "")

            login_response = self.get_page(data=True if self.data else False,
                                           header=True if self.headers
                                           else False)

        if not login_response:
            if self.verbose:
                print("%s no response received during login redirect! "
                      "Skipping site...\n" % WARN)
            return False

        # The code for these IF checks need to be cleaned up
        # If invalid credentials http status code is returned
        elif (self.invalid_http_status and self.response_status and
              str(self.invalid_http_status["value"]) ==
              str(self.response_status)):
            if("msg" in self.invalid_http_status or not
               login_response.strip("[]")):
                if self.verbose:
                    print("%s %s\n" %
                          (INFO, self.invalid_http_status["msg"] if "msg"
                           in self.invalid_http_status else
                           "The provided credentials are incorrect "
                           "or the account doesn't exist.\n"))
                return False
        # If captcha flag is set and found in login response
        if self.captcha_flag and self.captcha_flag in login_response:
            if self.verbose:
                print "%s captcha detected! Skipping site...\n" % WARN
            return False
        # If custom search is set and found in response
        elif self.custom_search and search(self.custom_search['regex'],
                                           login_response):
            if self.verbose:
                print "%s %s\n" % (INFO, self.custom_search["value"])
            return False
        # Valid password string in response
        elif self.valid_password and self.valid_password in login_response:
            print "%s Credentials worked! Successfully logged in.\n" % PLUS
            return True
        # Valid response header type REGEX
        elif (self.valid_response_header and
              self.valid_response_header_type == "regex" and
              search(self.valid_response_header,
                     str(self.response_headers))):
            print "%s Credentials worked! Successfully logged in.\n" % PLUS
            return True
        # Valid response header for cookies type REGEX
        elif (self.valid_response_header and
              self.valid_response_header_type == "regex" and
              search(self.valid_response_header, str(cookie_handler))):
            print "%s Credentials worked! Successfully logged in.\n" % PLUS
            return True
        # Valid response header type normal
        elif (self.valid_response_header and self.valid_response_header
              in str(self.response_headers)):
            print "%s Credentials worked! Successfully logged in.\n" % PLUS
            return True
        # Valid response header for cookies type normal
        elif (self.valid_response_header and self.valid_response_header
              in str(cookie_handler)):
            print "%s Credentials worked! Successfully logged in.\n" % PLUS
            return True
        # Custom message when specified invalid header is detected
        elif (self.custom_response_header and
              self.custom_response_header["value"] in
              str(self.response_headers)):
            if self.verbose:
                print "%s %s" % (INFO, self.custom_response_header["msg"])
            return False
        # Invalid account string found in login response
        elif self.invalid_account and self.invalid_account in login_response:
            if self.verbose:
                print("%s The provided account doesn't exist on this site.\n"
                      % INFO)
            return False
        # User exists, but account isn't activate.
        elif self.inactive_account and self.inactive_account in login_response:
            if self.verbose:
                print("%s The user exists, but the account isn't activate.\n"
                      % INFO)
            return False
        # User exists, but invalid password string in login response
        elif (self.invalid_password and self.invalid_account and
              self.invalid_password in login_response):
            if self.verbose:
                print("%s The user exists, but the password is incorrect.\n"
                      % INFO)
            return False
        # Invalid password string in login response
        elif (self.invalid_password and not self.invalid_account and
              self.invalid_password in login_response):
            if self.verbose:
                print("%s The provided credentials are incorrect "
                      "or the account doesn't exist.\n" % INFO)
            return False
        # Unhandled case
        else:
            if self.verbose:
                print "%s Unable to login! Skipping site...\n" % WARN
            return False


def print_http_error(error):
    """Prints the HTTP Response error"""
    if hasattr(error, "msg"):
        print "%s msg '%s'." % (ERROR, error.msg)
    if hasattr(error, "reason"):
        print "%s reason '%s'." % (ERROR, error.reason)
    if getattr(error, "message"):
        print "%s message '%s'." % (ERROR, error.message)
    if hasattr(error, "code"):
        print "%s error code '%d'." % (ERROR, error.code)
