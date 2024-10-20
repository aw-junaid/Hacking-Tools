import requests

def get_tor_session():
    session = requests.session()
    # Tor uses the 9050 port as the default socks port
    session.proxies = {'http':  'socks5://127.0.0.1:9050',
                       'https': 'socks5://127.0.0.1:9050'}
    return session

# Following prints your normal public IP
print("Normal Public IP:",requests.get("http://httpbin.org/ip").text)

# Make a request through the Tor connection
# IP visible through Tor
session = get_tor_session()
print("IP for Tor connection:",session.get("http://httpbin.org/ip").text)
# Above should print an IP different than your public IP

response = session.get('https://duckduckgogg42xjoc72x3sjasowoarfbgcmvfimaftt6twagswzczad.onion/')
#get headers dictionary response
for key,value in response.headers.items():
    print(key,value)


