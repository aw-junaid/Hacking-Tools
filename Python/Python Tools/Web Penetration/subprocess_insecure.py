import subprocess

data = input()
command = ' echo ' + data + ' >> ' + ' file.txt '
subprocess.call(command, shell = True) #insecure
with open('file.txt','r') as file:
    data = file.read()
