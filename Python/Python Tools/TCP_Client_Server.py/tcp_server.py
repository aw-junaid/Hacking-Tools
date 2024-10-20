#!/usr/bin/python

import socket
import threading

SERVER_IP   = "127.0.0.1"
SERVER_PORT = 9998

#  family = Internet, type = stream socket means TCP
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server.bind((SERVER_IP,SERVER_PORT))

server.listen(5)

print("[*] Server Listening on %s:%d" % (SERVER_IP,SERVER_PORT))

client,addr = server.accept()

client.send("I am the server accepting connections...".encode())

print("[*] Accepted connection from: %s:%d" % (addr[0],addr[1]))

def handle_client(client_socket):
    request = client_socket.recv(1024)
    print("[*] Received request : %s from client %s" , request, client_socket.getpeername())
    client_socket.send(bytes("ACK","utf-8"))

while True:
    handle_client(client)
    
client_socket.close()
server.close()
