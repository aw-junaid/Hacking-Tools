#!/usr/bin/env python

import socket
import sys
from datetime import datetime
import errno

remoteServer    = input("Enter a remote host to scan: ")
remoteServerIP  = socket.gethostbyname(remoteServer)

print("Please enter the range of ports you would like to scan on the machine")
startPort    = input("Enter a start port: ")
endPort    = input("Enter a end port: ")

print("Please wait, scanning remote host", remoteServerIP)

time_init = datetime.now()

try:
	for port in range(int(startPort),int(endPort)):
		print ("Checking port {} ...".format(port))
		sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		sock.settimeout(5)
		result = sock.connect_ex((remoteServerIP, port))
		if result == 0:
			print("Port {}: 	 Open".format(port))
		else:
			print("Port {}: 	 Closed".format(port))
			print("Reason:",errno.errorcode[result])
		sock.close()

except KeyboardInterrupt:
	print("You pressed Ctrl+C")
	sys.exit()
except socket.gaierror:
	print('Hostname could not be resolved. Exiting')
	sys.exit()
except socket.error:
	print("Couldn't connect to server")
	sys.exit()

time_finish = datetime.now()
total =  time_finish - time_init
print('Port Scanning Completed in: ', total)
