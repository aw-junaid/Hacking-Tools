Here are the 10 most important commands of Nmap, along with a full explanation for each:

1. Scan a single host: `nmap <host>`
   This command will perform a scan on a single host, specified by its IP address or hostname. By default, Nmap will perform a basic scan of the 1000 most common ports.

2. Scan a range of IP addresses: `nmap <start IP>-<end IP>`
   This command will perform a scan on a range of IP addresses, specified by the starting and ending IP addresses. For example, `nmap 192.168.1.1-10` will scan IP addresses from 192.168.1.1 to 192.168.1.10.

3. Scan a subnet: `nmap <subnet>`
   This command will perform a scan on a whole subnet, specified by the IP address and subnet mask. For example, `nmap 192.168.1.0/24` will scan all IP addresses on the 192.168.1.0 subnet.

4. Scan a list of hosts: `nmap -iL <list>`
   This command will perform a scan on a list of hosts, specified by a text file containing one IP address or hostname per line. For example, `nmap -iL targets.txt` will scan all hosts listed in the file `targets.txt`.

5. Perform a fast scan: `nmap -F <host>`
   This command will perform a fast scan on a host, scanning only the 100 most common ports. This can be useful for quickly identifying open ports and potential vulnerabilities.

6. Perform a more thorough scan: `nmap -sS <host>`
   This command will perform a more thorough scan on a host, using the TCP SYN scan technique. This technique is faster and less detectable than the default TCP connect scan.

7. Perform a TCP SYN scan: `nmap -sS <host>`
   This command will perform a scan using the TCP SYN technique, which sends a SYN packet to each port and waits for a response. This can be useful for stealthy scanning and can often bypass firewalls and intrusion detection systems.

8. Perform a UDP scan: `nmap -sU <host>`
   This command will perform a scan using the UDP protocol, which is commonly used for services such as DNS and DHCP. UDP scans can be slower and less reliable than TCP scans, but can often uncover additional open ports.

9. Scan for specific ports: `nmap -p <port(s)> <host>`
   This command will perform a scan on specific ports, specified by a comma-separated list of port numbers or ranges. For example, `nmap -p 80,443,8080 192.168.1.1` will scan for open ports 80, 443, and 8080 on the host 192.168.1.1.

10. Scan all ports: `nmap -p- <host>`
    This command will perform a scan on all ports (1-65535) of a host. This can take a long time to complete, but can be useful for discovering hidden or lesser-known services.
