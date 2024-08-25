credmap: The Credential Mapper
===

Credmap is an open source tool that was created to bring awareness to the dangers of credential reuse. It is capable of testing supplied user credentials on several known websites to test if the password has been reused on any of these. An official introductionary post can be found [here](http://websec.ca/blog/view/credmap).

### Help Menu
    Usage: credmap.py --email EMAIL | --user USER | --load LIST [options]

	Options:
	  -h/--help             show this help message and exit
	  -v/--verbose          display extra output information
	  -u/--username=USER..  set the username to test with
	  -p/--password=PASS..  set the password to test with
	  -e/--email=EMAIL      set an email to test with
	  -l/--load=LOAD_FILE   load list of credentials in format USER:PASSWORD
	  -f/--format=CRED_F..  format to use when reading from file (e.g. u|e:p)
	  -x/--exclude=EXCLUDE  exclude sites from testing
	  -o/--only=ONLY        test only listed sites
	  -s/--safe-urls        only test sites that use HTTPS.
	  -i/--ignore-proxy     ignore system default HTTP proxy
	  --proxy=PROXY         set proxy (e.g. "socks5://192.168.1.2:9050")
	  --list                list available sites to test with

### Examples
	./credmap.py --username janedoe --email janedoe@email.com
	./credmap.py -u johndoe -e johndoe@email.com --exclude "github.com, live.com"
	./credmap.py -u johndoe -p abc123 -vvv --only "linkedin.com, facebook.com"
	./credmap.py -e janedoe@example.com --verbose --proxy "https://127.0.0.1:8080"
	./credmap.py --load creds.txt --format "e.u.p"
	./credmap.py -l creds.txt -f "u|e:p"
	./credmap.py -l creds.txt
	./credmap.py --list


### Add new websites
Adding new websites to be tested using credmap can be done by creating a new XML file in the websites/ folder. To view a list of all possible tags that can be used in the XML file, please refer to the [Wiki](https://github.com/lightos/credmap/wiki).

### Docker
Build and deploy with the following:
```bash
git clone https://github.com/lightos/credmap.git
cd credmap
docker build -t credmap .
docker run -it credmap
```
