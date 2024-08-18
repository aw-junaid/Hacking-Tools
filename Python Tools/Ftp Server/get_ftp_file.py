#!/usr/bin/env python3

from ftplib import FTP

def writeData(data):
	file_descryptor.write(data+"\n")

ftp_client=FTP('ftp.be.debian.org')
ftp_client.login()
ftp_client.cwd('/pub/linux/kernel/v5.x/')

file_descryptor=open('ChangeLog-5.0','wt')
ftp_client.retrlines('RETR ChangeLog-5.0',writeData)
file_descryptor.close()
ftp_client.quit()

