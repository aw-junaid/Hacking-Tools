import requests

def geoip(domain):
	headers = {
            "Content-Type": "application/json"
    }
    
	response = requests.get('http://freegeoip.app/json/' + domain,headers=headers)
	return(response.text)
	
print(geoip('python.org'))
