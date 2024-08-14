#!/usr/bin/env python

import requests
import os

SHODAN_API_KEY = os.environ['SHODAN_API_KEY']
ip = '1.1.1.1'

def ShodanInfo(ip):
    try:
        result = requests.get(f"https://api.shodan.io/shodan/host/{ip}?key={SHODAN_API_KEY}&minify=True").json()
    except Exception as exception:
        result = {"error":"Information not available"}
    return result

print(ShodanInfo(ip))
