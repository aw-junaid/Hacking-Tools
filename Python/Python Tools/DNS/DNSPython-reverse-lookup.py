import dns.reversename

domain = dns.reversename.from_address("45.55.99.72")
print(domain)
print(dns.reversename.to_address(domain))



