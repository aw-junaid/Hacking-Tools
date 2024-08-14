#!/usr/bin/env python3

import optparse
import nmap

class NmapScanner:
     
    def __init__(self): 
        self.portScanner = nmap.PortScanner()
    
    def nmapScan(self, ip_address, port): 
        self.portScanner.scan(ip_address, port) 
        self.state = self.portScanner[ip_address]['tcp'][int(port)]['state']
        print(" [+] Executing command: ", self.portScanner.command_line()) 
        print(" [+] "+ ip_address + " tcp/" + port + " " + self.state)

def main():
    parser = optparse.OptionParser("usage%prog " + "--ip_address <target ip address> --ports <target port>") 
    parser.add_option('--ip_address', dest = 'ip_address', type = 'string', help = 'Please, specify the target ip address.')
    parser.add_option('--ports', dest = 'ports', type = 'string', help = 'Please, specify the target port(s) separated by comma.')
    (options, args) = parser.parse_args()
    if (options.ip_address == None) | (options.ports == None): 
        print('[-] You must specify a target ip_address and a target port(s).')
        exit(0)
    ip_address = options.ip_address
    ports = options.ports.split(',')

    for port in ports: 
        NmapScanner().nmapScan(ip_address, port)

if __name__ == "__main__": 
    main()
