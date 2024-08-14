#!/usr/bin/python

import socket

try:
    print("gethostname:",socket.gethostname())
    print("gethostbyname",socket.gethostbyname('www.google.com'))
    print("gethostbyname_ex",socket.gethostbyname_ex('www.google.com'))
    print("gethostbyaddr",socket.gethostbyaddr('8.8.8.8'))
    print("getfqdn",socket.getfqdn('www.google.com'))
    print("getaddrinfo",socket.getaddrinfo("www.google.com",None,0,socket.SOCK_STREAM))

except socket.error as error:
   print (str(error))
   print ("Connection error")
   
