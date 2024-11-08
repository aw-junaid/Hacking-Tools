#!/usr/bin/env python3

import nmap
import argparse

def callbackFTP(host, result):
    try:
        script = result['scan'][host]['tcp'][21]['script']
        print("Command line"+ result['nmap']['command_line'])
        for key, value in script.items():
            print('Script {0} --> {1}'.format(key, value))
    except KeyError:
        pass
        

class NmapScannerAsyncFTP:
    def __init__(self):
        self.portScanner = nmap.PortScanner()
        self.portScannerAsync = nmap.PortScannerAsync()

    def scanning(self):
        while self.portScannerAsync.still_scanning():
            print("Scanning >>>")
            self.portScannerAsync.wait(10)    

    def nmapScanAsync(self, hostname, port):
        try:
            print("Checking port "+ port +" ..........")
            self.portScanner.scan(hostname, port)
            self.state = self.portScanner[hostname]['tcp'][int(port)]['state']
            print(" [+] "+ hostname + " tcp/" + port + " " + self.state)
            #checking FTP service
            if (port=='21') and self.portScanner[hostname]['tcp'][int(port)]['state']=='open':
                print('Checking ftp port with nmap scripts......')
                print('Checking ftp-anon.nse .....')
                self.portScannerAsync.scan(hostname,arguments="-A -sV -p21 --script ftp-anon.nse",callback=callbackFTP)
                self.scanning()
                print('Checking ftp-bounce.nse  .....')
                self.portScannerAsync.scan(hostname,arguments="-A -sV -p21 --script ftp-bounce.nse",callback=callbackFTP)
                self.scanning()
                print('Checking ftp-libopie.nse  .....')
                self.portScannerAsync.scan(hostname,arguments="-A -sV -p21 --script ftp-libopie.nse",callback=callbackFTP)
                self.scanning()
                print('Checking ftp-proftpd-backdoor.nse  .....')
                self.portScannerAsync.scan(hostname,arguments="-A -sV -p21 --script ftp-proftpd-backdoor.nse",callback=callbackFTP)
                self.scanning()
                print('Checking ftp-vsftpd-backdoor.nse   .....')
                self.portScannerAsync.scan(hostname,arguments="-A -sV -p21 --script ftp-vsftpd-backdoor.nse",callback=callbackFTP)
                self.scanning()

        except Exception as exception:
            print("Error to connect with " + hostname + " for port scanning",str(exception))
    
    
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Nmap scanner async')
    parser.add_argument("--host", dest="host", help="target IP / domain", required=True)
    parser.add_argument("--ports", dest="ports", help="Please, specify the target port(s) separated by comma[80,8080 by default]", default="21")
    parsed_args = parser.parse_args()
    port_list = parsed_args.ports.split(',')
    ip_address = parsed_args.host
    for port in port_list:
        NmapScannerAsyncFTP().nmapScanAsync(ip_address, port)
        
