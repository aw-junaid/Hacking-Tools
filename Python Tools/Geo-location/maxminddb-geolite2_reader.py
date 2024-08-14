#!/usr/bin/env python3

import socket
from geolite2 import geolite2
import argparse
import json

parser = argparse.ArgumentParser(description='Get IP Geolocation info')
parser.add_argument('--hostname', action="store", dest="hostname", default='python.org')

given_args = parser.parse_args()
hostname = given_args.hostname
ip_address = socket.gethostbyname(hostname)

print("IP address: {0}".format(ip_address))

reader = geolite2.reader()
response = reader.get(ip_address)
print (json.dumps(response,indent=4))

print ("Continent:",json.dumps(response['continent']['names']['en'],indent=4))
print ("Country:",json.dumps(response['country']['names']['en'],indent=4))
print ("Latitude:",json.dumps(response['location']['latitude'],indent=4))
print ("Longitude:",json.dumps(response['location']['longitude'],indent=4))
print ("Time zone:",json.dumps(response['location']['time_zone'],indent=4))

