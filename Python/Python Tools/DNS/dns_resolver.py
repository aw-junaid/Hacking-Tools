import dns.resolver
 
hosts = ["oreilly.com", "yahoo.com", "google.com", "microsoft.com", "cnn.com"]

for host in hosts:
    print(host)
    ip = dns.resolver.query(host, "A")
    for i in ip:
        print(i)
