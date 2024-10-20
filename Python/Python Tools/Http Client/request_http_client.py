import http.client

connection = http.client.HTTPConnection("www.google.com")
connection.request("GET", "/")
response = connection.getresponse()
print(type(response))
print(response.status, response.reason)

if response.status == 200:
    data = response.read()
    print(data)
