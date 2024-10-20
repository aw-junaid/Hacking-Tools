import time, socks, socket
import requests
from stem import Signal
from stem.control import Controller

numberIPAddresses=5

with Controller.from_port(port = 9051) as controller:
    controller.authenticate()
    socks.setdefaultproxy(socks.PROXY_TYPE_SOCKS5, "127.0.0.1", 9050)
    socket.socket = socks.socksocket

    for i in range(0, numberIPAddresses):
        newIPAddress = requests.get("http://icanhazip.com").text
        print("NewIP Address: %s" % newIPAddress)
        controller.signal(Signal.NEWNYM)
        if controller.is_newnym_available() == False:
            print("Waiting time for Tor to change IP: "+ str(controller.get_newnym_wait()) +" seconds")
            time.sleep(controller.get_newnym_wait())
            
    controller.close()

