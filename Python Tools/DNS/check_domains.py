#!/usr/bin/env python

import argparse
import dns.name

def main(domain1, domain2):
    domain1 = dns.name.from_text(domain1)
    domain2 = dns.name.from_text(domain2)
    print("domain1 is subdomain of domain2: ", domain1.is_subdomain(domain2)) 
    print("domain1 is superdomain of domain2: ", domain1.is_superdomain(domain2))

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Check 2 domains with dns Python')
    parser.add_argument('--domain1', action="store", dest="domain1",  default='python.org')
    parser.add_argument('--domain2', action="store", dest="domain2",  default='docs.python.org')
    given_args = parser.parse_args() 
    domain1 = given_args.domain1
    domain2 = given_args.domain2
    main (domain1, domain2)
