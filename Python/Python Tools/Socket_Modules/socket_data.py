#!/usr/bin/python

import socket

print('creating socket ...')
s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
print('socket created')
print("connection with remote host")

target_host = "www.awjunaid.com" 
target_port = 80

s.connect((target_host,target_port))
print('connection ok')

request = "GET / HTTP/1.1\r\nHost:%s\r\n\r\n" % target_host
s.send(request.encode())

data=s.recv(4096)
print("Data",str(bytes(data)))
print("Length",len(data))

print('closing the socket')
s.close()
