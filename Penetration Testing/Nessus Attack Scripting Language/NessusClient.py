#!/usr/bin/env python3

import requests
import json
import argparse

class NessusClient():
    def __init__(self, nessusServer, nessusPort):
        self.nessusServer = nessusServer
        self.nessusPort = nessusPort
        self.url='https://'+str(nessusServer)+':'+str(nessusPort)
        self.token = None
        self.headers = {}
        self.bodyRequest = {}

    def get_request(self, url):
        response = requests.get(url, data=self.bodyRequest, headers=self.headers, verify=False)
        return json.loads(response.content)

    def post_request(self, url):
        response = requests.post(url, data=self.bodyRequest, headers=self.headers, verify=False)
        return json.loads(response.content)


    def request_api(self, service, params={}):
        self.headers={'Host': str(self.nessusServer)+':'+str(self.nessusPort),
                          'Content-type':'application/x-www-form-urlencoded',
                          'X-Cookie':'token='+self.token}
        print(self.headers)
        content = self.get_request(self.url+service)
        return content

    def login(self, nessusUser, nessusPassword):
        headers={'Host': str(self.nessusServer)+':'+str(self.nessusPort),
                          'Content-type':'application/x-www-form-urlencoded'}
        params={'username':nessusUser, 'password':nessusPassword}
        self.bodyRequest.update(params)
        self.headers.update(headers)
        print(self.headers)
        content = self.post_request(self.url+"/session")
        if "token" in content:
            self.token = content['token']
        return content



parser = argparse.ArgumentParser()
parser.add_argument('--user',  required=True)
parser.add_argument('--password', required=True)
args = parser.parse_args()

user=args.user
password=args.password

client = NessusClient('127.0.0.1','8834')
client.login(user,password)
print(client.request_api('/server/status'))
scans = client.request_api('/scans')['scans']

print(scans)

for scan in scans:
    vulnerabilities= client.request_api('/scans/'+str(scan['id']))['vulnerabilities']
    for vuln in vulnerabilities:
        print(vuln['plugin_family'],vuln['plugin_name'])
