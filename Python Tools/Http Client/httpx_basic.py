import httpx

client = httpx.Client(timeout=10.0)
response = client.get("http://www.google.es")
print(response)
print(response.status_code)
print(response.text)
