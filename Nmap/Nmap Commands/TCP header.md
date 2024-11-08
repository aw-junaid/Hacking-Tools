<img align = "" src="https://github.com/aw-junaid/aw-junaid/blob/main/peakpx.jpg" alt="TCP header">


The TCP (Transmission Control Protocol) header is a fundamental part of the TCP/IP suite, providing the necessary information to establish, manage, and terminate connections between devices over the network. It ensures reliable data transmission by managing packet ordering, error-checking, and retransmission.

### TCP Header Structure (20-60 bytes)

The TCP header is typically 20 bytes in size but can be up to 60 bytes if options are included. Here’s a breakdown of each field:

| Field               | Size (bits) | Description |
|---------------------|-------------|-------------|
| **Source Port**     | 16          | The port number of the sender. |
| **Destination Port**| 16          | The port number of the recipient. |
| **Sequence Number** | 32          | Identifies the order of the data bytes sent from sender to receiver. |
| **Acknowledgment Number** | 32    | If the ACK flag is set, this field contains the next expected sequence number from the sender. |
| **Data Offset**     | 4           | Specifies the size of the TCP header in 32-bit words (minimum value is 5, meaning 20 bytes). |
| **Reserved**        | 3           | Reserved for future use and must be set to zero. |
| **Flags**           | 9           | Controls the state of the connection. Common flags include: |
|                     |             | - **URG**: Urgent pointer field is significant. |
|                     |             | - **ACK**: Acknowledgment field is significant. |
|                     |             | - **PSH**: Push function; requests that the receiver passes data to the application as soon as possible. |
|                     |             | - **RST**: Reset the connection. |
|                     |             | - **SYN**: Synchronize sequence numbers to initiate a connection. |
|                     |             | - **FIN**: No more data from the sender; end of the connection. |
| **Window Size**     | 16          | Specifies the size of the sender’s receive window, which limits the amount of data that can be sent before an acknowledgment is received. |
| **Checksum**        | 16          | Used for error-checking of the header and data. |
| **Urgent Pointer**  | 16          | If the URG flag is set, this field indicates the offset from the sequence number of the last urgent data byte. |
| **Options**         | Variable (0-40 bytes) | Optional parameters for additional functionality, such as Maximum Segment Size (MSS), Window Scaling, and Timestamps. |
| **Padding**         | Variable   | Added to ensure the header is a multiple of 32 bits if options are used. |

### Key Fields in Action

- **Source and Destination Ports**: Used to specify the communication endpoints on the source and destination devices.
- **Sequence and Acknowledgment Numbers**: Enable reliable, ordered data transfer by allowing both ends of the connection to keep track of what data has been sent and acknowledged.
- **Flags**: Control connection setup, data transfer, and termination. For example:
  - A **SYN** flag initiates a connection.
  - **ACK** confirms received data.
  - **FIN** ends a connection.
- **Window Size**: Implements TCP flow control by specifying the number of bytes the sender can transmit without acknowledgment.
- **Checksum**: Ensures data integrity by verifying that the header and data haven’t been corrupted during transmission.

### TCP Three-Way Handshake Example

The TCP connection typically uses a **three-way handshake**:

1. **SYN**: The client sends a TCP packet with the SYN flag set to initiate the connection.
2. **SYN-ACK**: The server responds with a packet that has both SYN and ACK flags set, indicating acknowledgment and willingness to establish a connection.
3. **ACK**: The client responds with an ACK flag, completing the handshake and establishing the connection.

### Example TCP Header in Hexadecimal

Here’s a simple example of a TCP header (20 bytes) in hexadecimal:

```
00 50 1F 90 00 00 00 01 00 00 00 00 50 02 20 00 91 7C 00 00
```

This header breaks down as:
- **Source Port (00 50)**: 80 (indicating HTTP traffic from a web server)
- **Destination Port (1F 90)**: 8080
- **Sequence Number (00 00 00 01)**: Sequence number 1
- **Data Offset and Flags (50 02)**: Offset is 5 (20 bytes header) and flags indicate SYN
