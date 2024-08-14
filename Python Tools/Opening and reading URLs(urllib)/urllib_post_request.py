import urllib.request
import urllib.parse

#POST request

data_dictionary = {"id": "0123456789"}
data = urllib.parse.urlencode(data_dictionary)
data = data.encode('ascii')

with urllib.request.urlopen("http://httpbin.org/post", data) as response:
	print(response.read().decode('utf-8'))
