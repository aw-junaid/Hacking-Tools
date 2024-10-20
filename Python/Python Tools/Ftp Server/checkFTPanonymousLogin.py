#!/usr/bin/env python3

import ftplib

def anonymousLogin(hostname):
    try:
        ftp = ftplib.FTP(hostname)
        response = ftp.login('anonymous', 'anonymous')
        print(response)
        if "230 Anonymous access granted" in response:       
            print('\n[*] ' + str(hostname) +' FTP Anonymous Login Succeeded.')
            print(ftp.getwelcome())
            ftp.dir()
    except Exception as e:
        print(str(e))
        print('\n[-] ' + str(hostname) +' FTP Anonymous Login Failed.')

hostname = 'ftp.be.debian.org'
anonymousLogin(hostname)
