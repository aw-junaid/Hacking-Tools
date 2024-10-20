#!/usr/bin/env python3

import requests,json
data_dictionary = {"id": "0123456789"}
headers = {"Content-Type" : "application/json","Accept":"application/json"}
response = requests.post("http://httpbin.org/post",data=data_dictionary,headers=headers,json=data_dictionary)
print("HTTP Status Code: " + str(response.status_code))

print(response.headers)

if response.status_code == 200:

	results = response.json()
	for result in results.items():
		print(result)

	print("Headers response: ")
	for header, value in response.headers.items():
		print(header, '-->', value)

	print("Headers request : ")
	for header, value in response.request.headers.items():
		print(header, '-->', value)

	print("Server:" + response.headers['server'])
else:
	print("Error code %s" % response.status_code)
