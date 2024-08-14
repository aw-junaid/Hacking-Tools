import requests
import sys
from bs4 import BeautifulSoup, SoupStrainer

xsspayloads = []

with open('XSS-attack-vectors.txt', 'r') as filehandle:
    for line in filehandle:
        xsspayload = line[:-1]
        xsspayloads.append(xsspayload)

#print(xsspayloads)

url = 'http://testphp.vulnweb.com/search.php?test=query'

data ={}
response = requests.get(url)

for payload in xsspayloads:
    for field in BeautifulSoup(response.text,"html.parser",parse_only=SoupStrainer('input')):
        print(field)
        if field.has_attr('name'):
            if field['name'].lower() == "submit":
                data[field['name']] = "submit"
            else:
                data[field['name']] = payload
    response = requests.post(url, data=data)
    if payload in response.text:
        print("Payload "+ payload +" returned in the response")
