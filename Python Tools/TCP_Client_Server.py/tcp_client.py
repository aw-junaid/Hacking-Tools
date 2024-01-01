#!/usr/bin/python
 
import socket

host="127.0.0.1"
port = 9998

try:
	mysocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	mysocket.connect((host, port))
	print('Connected to host '+str(host)+' in port: '+str(port))
	message = mysocket.recv(1024)
	print("Message received from the server", message)
	while True:
		message = input("Enter your message > ")
		mysocket.send(bytes(message.encode('utf-8')))
		if message== "quit":
			break
except socket.errno as error:
	print("Socket error ", error)
finally:
	mysocket.close()
