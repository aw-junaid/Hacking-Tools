#!/usr/bin/env python3

import getpass
import paramiko

HOSTNAME = 'localhost'
PORT = 22

def run_ssh_cmd(username, password, command, hostname=HOSTNAME,port=PORT):
    ssh_client = paramiko.SSHClient()
    ssh_client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    ssh_client.load_system_host_keys()
    ssh_client.connect(hostname, port, username, password)
    stdin, stdout, stderr = ssh_client.exec_command(command)
    #print(stdout.read())
    stdin.close()
    for line in stdout.read().splitlines():
        print(line.decode())

if __name__ == '__main__':
    hostname = input("Enter the target hostname: ")
    port = input("Enter the target port: ")
    username = input("Enter username: ")
    password = getpass.getpass(prompt="Enter password: ")
    command = input("Enter command: ")
    run_ssh_cmd(username, password, command)
