#!/usr/bin/env python3
    
import requests
import re

url = input("Enter URL > ")
var = requests.get(url).text

print("Images:")
print("#########################")
for image in re.findall("<img (.*)>",var):
    for images in image.split():
        if re.findall("src=(.*)",images):
            image = images[:-1].replace("src=\"","")
            if(image.startswith("http")):
                print(image)
            else:
                print(url+image)

print("#########################")
print("Links:")
print("#########################")
for link,name in re.findall("<a (.*)>(.*)</a>",var):
    for a in link.split():
        if re.findall("href=(.*)",a):
            url_image = a[0:-1].replace("href=\"","")
            if(url_image.startswith("http")):
                print(url_image)
            else:
                print(url+url_image)

