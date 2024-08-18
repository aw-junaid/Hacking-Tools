#!/usr/bin/env python3

import ftplib

FTP_SERVER_URL = 'ftp.be.debian.org'
DOWNLOAD_DIR_PATH = '/pub/linux/kernel/v5.x/'
DOWNLOAD_FILE_NAME = 'ChangeLog-5.0'

def ftp_file_download(server, username):
    ftp_client = ftplib.FTP(server, username)
    ftp_client.cwd(DOWNLOAD_DIR_PATH)
    try:
        with open(DOWNLOAD_FILE_NAME, 'wb') as file_handler:
            ftp_cmd = 'RETR %s' %DOWNLOAD_FILE_NAME
            ftp_client.retrbinary(ftp_cmd,file_handler.write)
            ftp_client.quit()
    except Exception as exception:
        print('File could not be downloaded:',exception)

if __name__ == '__main__':
    ftp_file_download(server=FTP_SERVER_URL,username='anonymous')
