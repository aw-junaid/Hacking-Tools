# Damn Web Scanner
Another web vulnerabilities scanner, this extension works on Chrome and Opera.
The extension is working on the background and will notify you if it finds any vulnerability.

![Image of a noticiation](https://github.com/swisskyrepo/DamnWebScanner/blob/master/Screens/Notification.png?raw=true)

Currently it scans for:
- SQL Injection : Time based SQLi scanner using polyglot vectors (MySQL, SQLite, Oracle, Postgresql, SQL Server)
- Cross Site Scripting : Using a browser simulator (Ghost)
- Local File Inclusion
- Remote Commands Execution using polyglot vectors based on time

All the features are:
- Detect if the server is up with a "/ping" request
- New XSS vectors, work in different contexts (JS var, JS function, inside HTML tag, outside HTML tag)
- Basic page to list the vulnerabilities URL and TYPE
- Re-use your cookies and user-agent to get access to page with cookie-authentication
- Export vulnerabilities into a CSV file
- Launch scan when a form is submitted or a page is opened via the URL bar

**Warning :** Do not use this extension for illegal purpose, the main goal of it is to simplify the life of bug hunters. It's a **BETA version**, many improvements will come don't worry

**Warning :** A crash can happen server-side, this issue isn't resolved yet. The docker-compose.yml is set to automatically restart the docker when this happen..

## Install
You need to install and configure the server, it uses ghost and flask.
1. To get started you only need to start the docker and the dependencies will be installed.
```bash
cd server
sudo docker-compose -f docker-compose.yml up -d --build
```

2. Edit the server configuration
By default it's listening to the port 8000, it can be modified in the following files :  popup.js, background.js, docker-compose.yml
```js
var config_server = "http://127.0.0.1:8000";
```

3. The extension isn't packed, to use it go to chrome://extensions or opera://extensions then select "Load unpacked extension"

4. Browse the internet ! (Don't forget to start the extension by clicking the 'START' button)


You can try the Error SQL, Blind SQL, LFI with Damn Vulnerable Web App.
The SQL Injection scanner was also tested on https://pentesterlab.com/badges/essential.    
![Image of DVWA Result](https://github.com/swisskyrepo/DamnWebScanner/blob/master/Screens/Results%20DVWA.png?raw=true)


## Debugging
It's strongly recommended to debug any problem with Portainer using the logs table.
```
http://127.0.0.1:9000/#/containers/[INSERT HERE THE CONTAINER'S ID]/logs
```

You will have an output like this :
```
STDOUT
---[ POST - New parameter rememberme for url: http://[REDACTED]/login.php ]---
		[94mXSS Failed [0m for  rememberme  with the payload : jaVasCript:alert(1)//" name=alert(1) ]'"><!--
		[94mLFI Failed [0m for  rememberme  with the payload : /etc/passwd
		[94mSQLi Detected [0m for  rememberme  with the payload : '
		[94mTime Based SQLi ( MySQL ) Failed [0m for  rememberme  with the payload : SLEEP(4) /*' || SLEEP(4) || '" || SLEEP(4) || "*/
		[94mTime Based SQLi ( SQLite ) Failed [0m for  rememberme  with the payload : substr(upper(hex(randomblob(55555555))),0,1) /[*' or substr(upper(hex(randomblob(55555555))),0,1) or '" or substr(upper(hex(randomblob(55555555)])),0,1) or "*/
		[94mTime Based SQLi ( PostgreSQL ) Failed [0m for  rememberme  with the payload : (SELECT 55555555 FROM PG_SLEEP(4)) /*' || (SELECT 55555555 FROM PG_SLEEP(4)) || '" || (SELECT 55555555 FROM PG_SLEEP(4)) || "*/
		[94mTime Based SQLi ( OracleSQL ) Failed [0m for  rememberme  with the payload : DBMS_PIPE.RECEIVE_MESSAGE(chr(65)||chr(65)||chr(65),5) /*' || DBMS_PIPE.RECEIVE_MESSAGE(chr(65)||chr(65)||chr(65),5) || '" || DBMS_PIPE.RECEIVE_MESSAGE(chr(65)||chr(65)||chr(65),5) || "*/
		[94mTime Based SQLi ( SQL Server ) Failed [0m for  rememberme  with the payload : WAITFOR DELAY chr(48)+chr(58)+chr(48)+chr(58)+chr(52) /*' || WAITFOR DELAY chr(48)+chr(58)+chr(48)+chr(58)+chr(52) || '" || WAITFOR DELAY chr(48)+chr(58)+chr(48)+chr(58)+chr(52) || "*/
		[94mRCE Failed [0m for  rememberme  with the payload : `#'|sleep${IFS}4|'`"|sleep${IFS}4|";sleep${IFS}4

STDERR
* Running on http://0.0.0.0:8000/ (Press CTRL+C to quit)
172.30.0.1 - - [11/Sep/2017 16:04:21] "GET /ping HTTP/1.1" 200 -
172.30.0.1 - - [11/Sep/2017 16:19:05] "GET /?url=http%3A//[REDACTED]/login.php&deep=0&impact=0&cookies=PHPSESSID%3DREMOVED&useragent=Mozilla/5.0%20(X11;%20Linux%20x86_64)%20AppleWebKit/537.36%20(KHTML,%20like%20Gecko)%20Chrome/60.0.3112.90%20Safari/537.36%20OPR/47.0.2631.80&method=POST&data=username%3Abob%7Cpassword%3Abob%7Crememberme%3Aon%7C%3A%7C HTTP/1.1" 200 -
172.30.0.1 - - [11/Sep/2017 16:19:16] "GET /ping HTTP/1.1" 200 -
```
A "/ping" request is done at every opening of the "Plugin Window" in your browser.

If you need the exact version of the component used in the beta test, here they are.
```
Ghost.py==2.0.0-dev   
PySide==2.0.0~alpha0
Qt==5.5.1
```

## Thanks, Contributors and Ideas
- Polyglot vector for SQL injections [The Ultimate SQL Injection Payload](https://labs.detectify.com/2013/05/29/the-ultimate-sql-injection-payload/)
- Polyglot vector for XSS injection 1 [One vector to rule them all](http://www.thespanner.co.uk/2010/09/15/one-vector-to-rule-them-all/)
- Polyglot vector for XSS injection 2 [Unleashing an Ultimate XSS Polyglot](https://github.com/0xsobky/HackVault/wiki/Unleashing-an-Ultimate-XSS-Polyglot)
