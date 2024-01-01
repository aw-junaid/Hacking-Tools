import socks
import socket

temp_socket = socket.socket
temp_create_connection = socket.create_connection

def disable_proxy():
    socket.socket = temp_socket
    socket.create_connection = temp_create_connection
    

def enable_proxy(host="127.0.0.1", port=9050):
    
    def create_connection(address, timeout=None, source_address=None):
        sock = socks.socksocket()
        sock.connect(address)
        return sock
    
    socks.setdefaultproxy(socks.PROXY_TYPE_SOCKS5, host, port, True)
    socket.socket = socks.socksocket
    socket.create_connection = create_connection
