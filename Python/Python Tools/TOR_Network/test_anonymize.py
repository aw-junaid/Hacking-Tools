import requests
from anonymize import enable_proxy, disable_proxy

#This url returns your ip as plain text
url = 'http://icanhazip.com'


def test_requests():
    print('requests: %s' % requests.get(url).text)

enable_proxy()
test_requests()

disable_proxy()
test_requests()
