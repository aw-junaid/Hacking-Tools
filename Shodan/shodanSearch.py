#!/usr/bin/env python

import shodan
import argparse
import socket
import sys
import os

SHODAN_API_KEY = os.environ['SHODAN_API_KEY']

api = shodan.Shodan(SHODAN_API_KEY)

parser = argparse.ArgumentParser(description='Shodan search')

parser.add_argument("--target", dest="target", help="target IP / domain", required=None)
parser.add_argument("--search", dest="search", help="search", required=None)

parsed_args = parser.parse_args()

if len(sys.argv)>1 and sys.argv[1] == '--search':
    try:
        results = api.search(parsed_args.search)
        print('Results: %s' % results['total'])
        for result in results['matches']:
            print('IP: %s' % result['ip_str'])
            print(result['data'])
    except shodan.APIError as exception:
        print('Error: %s' % exception)
        
if len(sys.argv)>1 and sys.argv[1] == '--target':
    try:
        hostname = socket.gethostbyname(parsed_args.target)
        results = api.host(hostname)
        print("""
                IP: %s
                Organization: %s
                Operating System: %s
        """ % (results['ip_str'], results.get('org', 'n/a'), results.get('os', 'n/a')))

        for item in results['data']:
            print("""Port: %s Banner: %s""" % (item['port'], item['data']))
        
    except shodan.APIError as exception:
        print('Error: %s' % exception)       

