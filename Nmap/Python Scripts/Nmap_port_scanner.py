#!/usr/bin/env python3

import nmap
                    
portScanner = nmap.PortScanner()

host_scan = input('Host scan: ')

portlist="21,22,23,25,80"	
portScanner.scan(hosts=host_scan, arguments='-n -p'+portlist)

print(portScanner.command_line())

hosts_list = [(x, portScanner[x]['status']['state']) for x in portScanner.all_hosts()]
for host, status in hosts_list:
    print(host, status)

for protocol in portScanner[host].all_protocols():
    print('Protocol : %s' % protocol)
    listport = portScanner[host]['tcp'].keys()
    for port in listport:
        print('Port : %s State : %s' % (port,portScanner[host][protocol][port]['state']))

