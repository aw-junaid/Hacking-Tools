<img align = "" src="https://github.com/aw-junaid/aw-junaid/blob/main/peakpx.jpg" alt="UDP Header">


The UDP (User Datagram Protocol) header is a simpler structure compared to TCP. UDP is a connectionless protocol, meaning it doesn’t establish or maintain a connection before sending data, which allows for faster communication but with no guarantee of delivery. The UDP header is small and efficient, containing only the essential information needed to route packets between devices.

### UDP Header Structure (8 bytes)

The UDP header is only 8 bytes (64 bits) long, making it more efficient for simple transmissions where reliability is less critical, like streaming or gaming.

| Field              | Size (bits) | Description |
|--------------------|-------------|-------------|
| **Source Port**    | 16          | The port number of the sender. |
| **Destination Port** | 16       | The port number of the recipient. |
| **Length**         | 16          | Specifies the total length of the UDP header and data in bytes. |
| **Checksum**       | 16          | Used for error-checking of the header and data (optional in IPv4, mandatory in IPv6). |

### Key Fields Explained

1. **Source Port**: Identifies the sending application’s port, used for the return data path. This field is optional; if it’s unused, it can be set to 0.
  
2. **Destination Port**: Specifies the port number on the receiving device, guiding the data to the correct application.

3. **Length**: Indicates the total length of the UDP packet, including both the header and the data. The minimum length is 8 bytes (header-only), but it can be up to 65,535 bytes.

4. **Checksum**: Ensures data integrity by detecting errors in the header and payload. While optional in IPv4, it’s mandatory in IPv6 to ensure reliable data transfer across the network.

### Example UDP Header in Hexadecimal

Here’s a simple example of a UDP header (8 bytes) in hexadecimal:

```
1F 90 00 35 00 1C B1 E6
```

This header breaks down as:
- **Source Port (1F 90)**: 8080
- **Destination Port (00 35)**: 53 (usually DNS)
- **Length (00 1C)**: 28 bytes (header + data)
- **Checksum (B1 E6)**: A calculated value for error-checking
