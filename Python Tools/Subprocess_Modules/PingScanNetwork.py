
#!/usr/bin/env python
import subprocess
import sys
import argparse

parser = argparse.ArgumentParser(description='Ping Scan Network')
    
# Main arguments
parser.add_argument("-network", dest="network", help="NetWork segment[For example 182.185.167.51]", required=True)
parser.add_argument("-machines", dest="machines", help="Machines number",type=int, required=True)

parsed_args = parser.parse_args()    
    
for ip in range(1,parsed_args.machines+1):
    ipAddress = parsed_args.network +'.' + str(ip)
    print("Scanning %s " %(ipAddress))
    if sys.platform.startswith('linux'):
        # Linux
        output = subprocess.Popen(['/bin/ping','-c 1',ipAddress],stdout = subprocess.PIPE).communicate()[0]
    elif sys.platform.startswith('win'):
        # Windows
        output = subprocess.Popen(['ping', ipAddress], stdin=PIPE, stdout=PIPE, stderr=PIPE).communicate()[0]
    output = output.decode('utf-8')
    print("Output",output)
    if "Lost = 0" in output or "bytes from " in output:
        print("The Ip Address %s has responded with a ECHO_REPLY!" % ipAddress)
