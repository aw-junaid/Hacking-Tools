**Host Discovery**, also known as “ping scanning,” is the phase in an Nmap scan where the tool identifies which hosts are live and reachable on a network. This allows Nmap to focus on scanning only active hosts, which saves time and resources. Nmap has several techniques to detect live hosts, each adapted to different network scenarios and firewall rules.

### Key Techniques in Host Discovery

1. **ICMP Echo Requests (Ping Request)**:
   - This is the simplest and most common form of host discovery.
   - Nmap sends an ICMP Echo Request (like the `ping` command), and if a host responds with an Echo Reply, it’s considered alive.
   - **Limitation**: Many firewalls and routers block ICMP packets, so this may not always work.

2. **TCP SYN Ping**:
   - Sends a TCP SYN packet to a specified port (commonly port 80 or 443, which are often open).
   - If the target replies with a SYN/ACK response, the host is alive. If it responds with a RST (Reset) packet, it’s unreachable.
   - **Advantage**: This technique can bypass some firewall rules that allow HTTP or HTTPS traffic but block ICMP.
   - **Command**: `nmap -PS<port> <target>`

3. **TCP ACK Ping**:
   - Sends a TCP ACK packet to a specified port.
   - If the host responds with a RST packet, it’s alive; no response may indicate the host is unreachable.
   - **Use Case**: Useful in environments where SYN packets might be blocked by firewalls, but ACK packets are allowed.
   - **Command**: `nmap -PA<port> <target>`

4. **UDP Ping**:
   - Sends a UDP packet to a specified port (often 53, used for DNS).
   - If the host responds with an ICMP Port Unreachable message, the host is considered alive.
   - **Limitation**: UDP ping can be slow and unreliable because UDP doesn’t require acknowledgment, and firewalls often block UDP traffic.
   - **Command**: `nmap -PU<port> <target>`

5. **ARP Ping (for Local Networks)**:
   - Uses ARP requests to discover hosts on the same local network. ARP requests don’t pass through routers, so this is only effective on local subnets.
   - **Advantage**: ARP is not typically filtered by firewalls on local networks, making it very reliable and fast for LAN environments.
   - **Command**: `nmap -PR <target>`

### Combining Techniques

Nmap allows combining multiple ping techniques for a more comprehensive host discovery. For instance, you can send both TCP SYN and ICMP Echo Requests simultaneously to increase the chance of detecting live hosts.

```bash
nmap -PE -PS80,443 -PA <target>
```

### Common Host Discovery Commands

- **ICMP Only**: `nmap -sn <target>`
  - This runs a basic ping scan with ICMP requests only.
  
- **TCP SYN and ACK Scan**: `nmap -PS80 -PA443 <target>`
  - Combines SYN and ACK scans on common web ports (80 and 443).

- **No Ping (Assume Hosts Are Up)**: `nmap -Pn <target>`
  - Skips the host discovery phase and scans all specified hosts, assuming they’re alive. This can be useful for highly restrictive networks but may be slower.

### Example Host Discovery Command

```bash
nmap -sn -PE -PS22,80,443 -PU53 <target-network>
```

- **Explanation**:
  - **`-sn`**: Tells Nmap to perform host discovery only, skipping port scanning.
  - **`-PE`**: Sends ICMP Echo Requests.
  - **`-PS22,80,443`**: Sends TCP SYN requests to ports 22, 80, and 443.
  - **`-PU53`**: Sends a UDP packet to port 53.

### Host Discovery Output

In host discovery, Nmap returns a list of hosts that responded to the discovery probes, marking them as "up." The result can then guide the subsequent scanning phase to focus on active hosts.
