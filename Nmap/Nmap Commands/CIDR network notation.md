CIDR (Classless Inter-Domain Routing) notation is a compact way to represent IP addresses and their associated routing prefixes, making it easier to define ranges of IP addresses without adhering to the old, rigid class-based system. CIDR notation is commonly used in IP addressing, subnetting, and firewall configurations.

### CIDR Notation Format

A CIDR notation consists of:
1. **IP Address**: The base address of the network.
2. **Slash Notation**: A slash (`/`) followed by a number that represents the number of bits in the network prefix.

The format is:  
```
<IP address>/<prefix length>
```

For example: `192.168.1.0/24`

- **`192.168.1.0`**: The base IP address of the network.
- **`/24`**: The prefix length, indicating that the first 24 bits are part of the network portion, leaving the remaining 8 bits for host addresses.

### How CIDR Works

- The prefix length (`/x`) determines which part of the IP address is the **network portion** and which part is the **host portion**.
- In IPv4, an IP address is 32 bits, so a `/24` CIDR prefix means the first 24 bits represent the network, leaving 8 bits for host addresses. 
- This means that `192.168.1.0/24` encompasses IP addresses from `192.168.1.0` to `192.168.1.255`, totaling 256 addresses (2^8).

### CIDR Subnet Table

Here’s a summary of some common CIDR prefixes and the number of addresses they represent:

| CIDR Notation | Network Mask       | Number of Addresses | Hosts Available (IPv4) |
|---------------|--------------------|----------------------|-------------------------|
| /8            | 255.0.0.0          | 16,777,216          | 16,777,214              |
| /16           | 255.255.0.0        | 65,536              | 65,534                  |
| /24           | 255.255.255.0      | 256                 | 254                     |
| /30           | 255.255.255.252    | 4                   | 2                       |
| /32           | 255.255.255.255    | 1                   | 1 (specific host)       |

- **/8** to **/15**: Typically used by large organizations for extensive networks.
- **/16** to **/23**: Often used by medium-sized organizations or ISPs.
- **/24** and smaller: Commonly used for LANs or smaller subnets.

### CIDR Example Calculations

**Example**: For `192.168.1.0/24`

1. **Network Prefix**: 24 bits (192.168.1).
2. **Host Bits**: Remaining 8 bits.
3. **Range of Addresses**:
   - First address: `192.168.1.0` (Network Address).
   - Last address: `192.168.1.255` (Broadcast Address).
   - Usable range for hosts: `192.168.1.1` to `192.168.1.254`.

**Example**: For `10.0.0.0/8`

1. **Network Prefix**: 8 bits (`10`).
2. **Host Bits**: Remaining 24 bits.
3. **Range of Addresses**:
   - First address: `10.0.0.0` (Network Address).
   - Last address: `10.255.255.255` (Broadcast Address).
   - Usable range for hosts: `10.0.0.1` to `10.255.255.254`.

### Benefits of CIDR Notation

- **Efficient IP Address Allocation**: It allows for flexible allocation of IP addresses based on actual need, rather than being restricted to predefined classes (A, B, C).
- **Reduced Routing Table Size**: ISPs can advertise large blocks of addresses in a single entry, reducing the size of routing tables and improving network efficiency.
- **Subnetting and Supernetting**: CIDR notation allows for both subnetting (dividing a network into smaller networks) and supernetting (combining multiple networks).
