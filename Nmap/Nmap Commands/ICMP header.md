<img align = "" src="https://github.com/aw-junaid/aw-junaid/blob/main/peakpx.jpg" alt="ICMP Header">


The ICMP (Internet Control Message Protocol) header is used primarily for network diagnostics and error messaging in IPv4 networks. It provides information about network issues to the source of an IP packet and is commonly used in tools like `ping` and `traceroute`.

### ICMP Header Structure (8 bytes for basic types)

The ICMP header is typically 8 bytes long, though certain types of ICMP messages can have additional data fields. Here’s a breakdown of the fields in a basic ICMP header:

| Field               | Size (bits) | Description |
|---------------------|-------------|-------------|
| **Type**            | 8           | Specifies the type of ICMP message (e.g., 0 for Echo Reply, 8 for Echo Request). |
| **Code**            | 8           | Provides further information about the type (used for finer granularity in error messages). |
| **Checksum**        | 16          | Used for error-checking of the header and data. |
| **Rest of Header**  | 32          | Contains additional information based on the ICMP message type, such as Identifier and Sequence Number for echo requests/replies. |

### Common ICMP Message Types

| Type | Code | Message Type             | Description |
|------|------|---------------------------|-------------|
| 0    | 0    | Echo Reply                | Response to an Echo Request (used by `ping`). |
| 3    | 0-15 | Destination Unreachable   | Indicates the destination is unreachable for various reasons (e.g., network unreachable, port unreachable). |
| 5    | 0-3  | Redirect Message          | Suggests a different route to reach the destination. |
| 8    | 0    | Echo Request              | Used to test reachability of a host (used by `ping`). |
| 11   | 0-1  | Time Exceeded             | Indicates that the TTL expired (used in `traceroute`). |

### Example ICMP Echo Request/Reply Header (Ping)

For an **Echo Request (Type 8)** or **Echo Reply (Type 0)**, the "Rest of Header" includes:

- **Identifier** (16 bits): Used to match requests with replies.
- **Sequence Number** (16 bits): Used to order echo requests and replies.

Here’s an example of an ICMP header for an Echo Request in hexadecimal:

```
08 00 F7 FF 1A 2B 00 01
```

This header breaks down as:
- **Type (08)**: Echo Request
- **Code (00)**: No additional code information
- **Checksum (F7 FF)**: Error-checking value for the header and data
- **Identifier (1A 2B)**: Identifies the session or process sending the request
- **Sequence Number (00 01)**: First sequence in the session
