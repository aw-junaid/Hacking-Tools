#!/usr/bin/env python3

import ness6rest
import argparse

nessus_url = "https://localhost:8834"

parser = argparse.ArgumentParser()
parser.add_argument('--login',  required=True)
parser.add_argument('--password', required=True)
args = parser.parse_args()

scan = ness6rest.Scanner(url=nessus_url, login=args.login, password=args.password, insecure=True)

print(scan.scan_list())

scans = scan.scan_list()['scans']
for detail_scan in scans:
    print(scan.scan_details(detail_scan['name']))

