#!/usr/bin/env python

import socket,sys

host = "domain/ip_address"
port = 80


try:
    mysocket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    print(mysocket)
    mysocket.settimeout(5)
except socket.error as e:
	print("socket create error: %s" %e)
	sys.exit(1)
	

try:
    mysocket.connect((host,port))
    print(mysocket)

except socket.timeout as e :
	print("Timeout %s" %e)
	sys.exit(1)
except socket.gaierror as e:
	print("connection error to the server:%s" %e)
	sys.exit(1)
except socket.error as e:
	print("Connection error: %s" %e)
	sys.exit(1)



