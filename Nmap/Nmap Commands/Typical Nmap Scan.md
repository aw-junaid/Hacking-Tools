A typical Nmap scan often includes a balance of speed, stealth, and useful information about the target. Here’s a rundown of commonly used scan types and their purposes.

### 1. Basic SYN Scan (Stealth Scan)

**Command:**
```bash
nmap -sS <target>
```

**Description:**
- This scan type is often called a "stealth scan" because it only sends SYN packets to initiate a connection without completing it.
- If a SYN-ACK is received, it means the port is open; if RST (reset) is received, the port is closed.
- It’s relatively fast and harder to detect since it doesn’t fully establish a connection.
  
### 2. Service Version Detection

**Command:**
```bash
nmap -sV <target>
```

**Description:**
- This command probes open ports to detect the version of the services running on them.
- Useful for identifying specific versions of applications, which can help in identifying potential vulnerabilities.

### 3. OS Detection and Service Versioning

**Command:**
```bash
nmap -O -sV <target>
```

**Description:**
- Combines OS detection (`-O`) and service version detection (`-sV`).
- Helps in gaining insight into the operating system running on the host, along with versions of exposed services.

### 4. Full Port Scan

**Command:**
```bash
nmap -p- <target>
```

**Description:**
- Scans all 65,535 TCP ports on the target.
- Useful for comprehensive scanning but can be time-consuming, especially on networks with many hosts or firewalls.

### 5. Aggressive Scan

**Command:**
```bash
nmap -A <target>
```

**Description:**
- Performs a more intense scan that includes OS detection, version detection, script scanning, and traceroute.
- This is useful when you want a complete profile of the target, but it’s louder (more likely to be detected by IDS/IPS).

### 6. Scan a Range of IP Addresses

**Command:**
```bash
nmap 192.168.1.1-254
```

**Description:**
- This command scans an entire IP range and can quickly check which hosts are up and which ports are open.
- Useful for network discovery and mapping.

### 7. UDP Scan

**Command:**
```bash
nmap -sU <target>
```

**Description:**
- Scans UDP ports, which are often used for services like DNS and SNMP.
- UDP scans are generally slower than TCP scans and are useful for uncovering additional services.

### 8. Scan with Timing Templates

**Command:**
```bash
nmap -T4 <target>
```

**Description:**
- `-T4` sets a faster timing template. Nmap timing levels range from `-T0` (slowest) to `-T5` (fastest).
- Higher numbers increase the speed of the scan but may trigger security alarms. `-T3` or `-T4` are typical choices for a balance between speed and stealth.

### Example Typical Nmap Scan Command

Combining common options:

```bash
nmap -sS -sV -O -p- -T4 <target>
```

**Explanation:**
- This example performs a stealth scan (`-sS`), detects service versions (`-sV`), attempts OS detection (`-O`), scans all ports (`-p-`), and uses a faster timing template (`-T4`).
- It’s a well-rounded scan providing information on open ports, services, and operating system without being too slow.
