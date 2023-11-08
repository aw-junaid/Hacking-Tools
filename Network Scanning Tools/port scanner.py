import socket
import termcolor

def scan(target, ports):
    print('\n' + ' Starting Scan For ' + str(target))
    for port in range(1, ports):
        scan_port(target, port)

def scan_port(ipaddress, port):
    try:
        # Create a new socket
        sock = socket.socket()

        # Try to connect to the specified IP address and port
        sock.connect((ipaddress, port))

        # If the connection is successful, print that the port is open
        print("[+] Port Opened " + str(port))

        # Close the socket
        sock.close()

    except:
        # If an exception occurs (e.g., connection error), pass and continue
        pass

# Ask the user for input
targets = input("[*] Enter Targets To Scan(split them by ,): ")
ports = int(input("[*] Enter How Many Ports You Want To Scan: "))

# Check if multiple targets are specified
if ',' in targets:
    print(termcolor.colored(("[*] Scanning Multiple Targets"), 'green'))
    
    # Split targets by comma and iterate through them
    for ip_addr in targets.split(','):
        # Call the scan function for each target
        scan(ip_addr.strip(' '), ports)
else:
    # If only one target is specified, call the scan function for that target
    scan(targets, ports)
