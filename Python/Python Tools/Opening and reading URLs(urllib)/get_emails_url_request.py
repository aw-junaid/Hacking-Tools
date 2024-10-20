import urllib.request
import re

USER_AGENT = 'Mozilla/5.0 (Linux; Android 10) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.101 Mobile Safari/537.36'

url =  input("Enter url:http://")

#https://www.packtpub.com/about/terms-and-conditions

opener = urllib.request.build_opener()
opener.addheaders = [('User-agent', USER_AGENT)]
urllib.request.install_opener(opener)

response = urllib.request.urlopen('http://'+url)
html_content= response.read()
pattern = re.compile("[-a-zA-Z0-9._]+[-a-zA-Z0-9._]+@[-a-zA-Z0-9_]+.[a-zA-Z0-9_.]+")
mails = re.findall(pattern,str(html_content))
print(mails)
