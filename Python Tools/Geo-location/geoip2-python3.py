#!/usr/bin/env python3

import socket
import geoip2.database
import argparse

parser = argparse.ArgumentParser(description='Get IP Geolocation info')
parser.add_argument('--hostname', action="store", dest="hostname",default='python.org')

given_args = parser.parse_args()
hostname = given_args.hostname
ip_address = socket.gethostbyname(hostname)
print("IP address: {0}".format(ip_address))

reader = geoip2.database.Reader('GeoLite2-City.mmdb')
response = reader.city(ip_address)

if response is not None:
    print('Country: ',response.country)
    print('Continent: ',response.continent)
    print('Location: ', response.location)
