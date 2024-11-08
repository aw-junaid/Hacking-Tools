<img align = "" src="https://github.com/aw-junaid/aw-junaid/blob/main/peakpx.jpg"  alt="ipv4 header">

The IPv4 header is a fundamental part of the Internet Protocol (IP) version 4, containing essential information about how a packet should be handled and delivered. Here’s a breakdown of the IPv4 header fields, including their functions and sizes.

### IPv4 Header Structure (20-60 bytes)

The IPv4 header has a minimum size of 20 bytes but can be up to 60 bytes if options are used. Here’s a breakdown of each field:

| Field              | Size (bits) | Description |
|--------------------|-------------|-------------|
| **Version**        | 4           | Specifies the IP version, which is 4 for IPv4. |
| **IHL (Internet Header Length)** | 4 | Specifies the length of the IP header in 32-bit words (minimum value is 5, meaning 20 bytes). |
| **DSCP (Differentiated Services Code Point)** | 6 | Used for Quality of Service (QoS) to prioritize certain types of traffic. |
| **ECN (Explicit Congestion Notification)** | 2 | Provides network congestion feedback without dropping packets. |
| **Total Length**   | 16          | Specifies the total length of the IP packet (header + data) in bytes, up to 65,535 bytes. |
| **Identification** | 16          | Unique ID for each packet, used for reassembling fragmented packets. |
| **Flags**          | 3           | Controls or indicates whether a packet can be fragmented. Key flags include: DF (Don't Fragment) and MF (More Fragments). |
| **Fragment Offset**| 13          | Specifies the position of this fragment relative to the start of the original packet, used in packet reassembly. |
| **TTL (Time to Live)** | 8      | Indicates the maximum number of hops (routers) the packet can pass through; decrements by 1 at each hop, and discarded when it reaches 0. |
| **Protocol**       | 8           | Specifies the protocol used in the data portion of the IP packet (e.g., TCP is 6, UDP is 17). |
| **Header Checksum**| 16          | Ensures the integrity of the header; recalculated at each router. |
| **Source IP Address** | 32      | The IPv4 address of the sender. |
| **Destination IP Address** | 32  | The IPv4 address of the recipient. |
| **Options**        | Variable (0-40 bytes) | Optional fields for special requirements like security, timestamping, etc. |
| **Padding**        | Variable   | Used to ensure the header is a multiple of 32 bits if options are included. |

### Example of Key Fields in Action

- **Version and IHL**: A packet with version 4 and IHL 5 indicates a standard IPv4 packet with a minimum header size.
- **Total Length**: Ensures that both sender and receiver know the full size of the packet, making reassembly and data extraction easier.
- **TTL**: Limits the packet's lifespan in the network, helping to avoid routing loops by discarding the packet if it exceeds the maximum hop count.
- **Protocol**: Directs the packet to the correct protocol handler at the destination (e.g., TCP or UDP).

### IPv4 Header Example in Hexadecimal

Here's a simplified IPv4 header example (20 bytes) in hexadecimal, showing how each field is represented:

```
45 00 00 3c 1c 46 40 00 40 06 b1 e6 c0 a8 00 68 c0 a8 00 01
```

This header breaks down as:
- **Version + IHL (45)**: Version 4 and IHL of 5 (20 bytes)
- **Total Length (00 3c)**: 60 bytes
- **TTL (40)**: 64 hops
- **Protocol (06)**: TCP
