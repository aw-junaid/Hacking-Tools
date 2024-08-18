#!/usr/bin/env python3

import paramiko
import getpass

def run_ssh_command(hostname, user, passwd, command):
    transport = paramiko.Transport(hostname)
    try:
        transport.start_client()
    except Exception as e:
        print(e)
    
    try:
        transport.auth_password(username=user,password=passwd)
    except Exception as e:
        print(e)
        
    if transport.is_authenticated():
        print(transport.getpeername())
        channel = transport.open_session()
        channel.exec_command(command)
        response = channel.recv(1024)
        print('Command %r(%r)-->%s' % (command,user,response))

if __name__ == '__main__':
    hostname = input("Enter the target hostname: ")
    port = input("Enter the target port: ")
    username = input("Enter username: ")
    password = getpass.getpass(prompt="Enter password: ")
    command = input("Enter command: ")
    run_ssh_command(hostname,username, password, command)
