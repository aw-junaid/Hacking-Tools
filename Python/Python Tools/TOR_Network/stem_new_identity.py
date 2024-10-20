import time
from stem import Signal
from stem.control import Controller

import requests

def get_tor_session():
    session = requests.session()
    # Tor uses the 9050 port as the default socks port
    session.proxies = {'http':  'socks5h://127.0.0.1:9050','https': 'socks5h://127.0.0.1:9050'}
    return session


def main():
    while True:
        time.sleep(5)
        print ("Rotating IP")
        with Controller.from_port(port = 9051) as controller:
          controller.authenticate()
          controller.signal(Signal.NEWNYM) #gets new identity
        # Make a request through the Tor connection
        # IP visible through Tor
        # Should print an IP different than your public IP
        session = get_tor_session()
        print(session.get("http://httpbin.org/ip").text)

if __name__ == '__main__':
    main()

