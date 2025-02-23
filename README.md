# pinG 

https://github.com/user-attachments/assets/77ea66b0-2c6b-40d3-8146-c7ed047e8e8f

 ---

Raw Socket & Ping Command

---

1 . What is a Socket and Why is it Used?

Sockets are software components that enable data transmission over a network between two systems. They allow data exchange between a client and a server using communication protocols. Today, most internet applications (such as web browsers, messaging services, and games) operate through sockets.
Normally, a server runs on a specific computer and has a socket that is bound to a specific port number. The server just waits, listening to the socket for a client to make a connection request.
On the client-side: The client knows the hostname of the machine on which the server is running and the port number on which the server is listening. To make a connection request, the client tries to rendezvous with the server on the server's machine and port. The client also needs to identify itself to the server so it binds to a local port number that it will use during this connection. This is usually assigned by the system.
If everything goes well, the server accepts the connection. Upon acceptance, the server gets a new socket bound to the same local port and also has its remote endpoint set to the address and port of the client. It needs a new socket so that it can continue to listen to the original socket for connection requests while tending to the needs of the connected client.
On the client side, if the connection is accepted, a socket is successfully created and the client can use the socket to communicate with the server.
The client and server can now communicate by writing to or reading from their sockets.
Purposes of Using Sockets:
Data transmission in a client-server model
Fast data transfer in real-time applications
Communication between different devices on a network

---

2. What is a Raw Socket?
3. 
A Raw Socket allows direct interaction with IP packets, bypassing transport layer protocols like TCP and UDP. This is advantageous for operations requiring detailed control at the network level. For example, ICMP (Internet Control Message Protocol) packets are not dependent on transport layer protocols like TCP or UDP and can only be sent and received using Raw Sockets.
This way works the well-known ping command, by sending an ICMP echo request message:

---

3. Creating a Raw Socket
4. 
Unlike TCP and UDP, Raw Sockets work directly with IP packets.

Graphical demonstration of a raw socketComparison: Raw Socket vs. Other Sockets
Stream (TCP) and Datagram (UDP) sockets carry only the payload and receive data from the transport layer. They do not include header information such as the source IP address or MAC address.
Applications running on different machines or the same machine use these socket types for data exchange.
Raw Sockets provide direct access to lower-level protocols and receive unprocessed (un-extracted) packets.
Unlike Stream and Datagram sockets, Raw Sockets do not require specifying a port or IP address since the entire packet content is delivered directly to the application.
Raw Sockets bypass the normal TCP/IP processing flow and send packets directly to a specific user application.
ICMP packets operate directly over IP and cannot be used with regular sockets.

Graphical demonstration of how a raw socket works compared to other socketsint sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
AF_INET → Uses the IPv4 protocol.
SOCK_RAW → Creates a raw socket (provides control at the IP level). While normal sockets use SOCK_STREAM (TCP) or SOCK_DGRAM (UDP), raw sockets process IP packets directly. Necessary for protocols without a transport layer, such as ICMP.
IPPROTO_ICMP → Opens a socket for ICMP protocol, used in tools like ping or traceroute. If TCP or UDP were needed, IPPROTO_TCP or IPPROTO_UDP would be used.

The most common Types of Sockets:

---

4 . ICMP (Internet Control Message Protocol) Packet

ICMP is an Internet Control Message Protocol that operates at the network layer. It is typically used in network devices like routers for error management. Since the network layer encounters various error types, ICMP is used for reporting and troubleshooting.

ICMP and the OSI Model:

ICMP is used for error reporting and management queries.
It is a supporting protocol that network devices (e.g., routers) use to send error messages and operational information.
For example, it is used when a requested service is unavailable or a host/router is unreachable.
Since the IP protocol lacks built-in error reporting or correction mechanisms, ICMP messages convey such information.

The ICMP Protocol and OSI ModelOSI Layer Analyze📌 ICMP's Independence from UDP/TCP
Since ICMP operates directly over IP, it does not require a transport layer protocol like UDP or TCP. Therefore, ICMP messages do not include port numbers.

---

5 . ICMP Packet Structure

An ICMP Echo Request or Echo Reply packet consists of the following components:
Type: 8 (Echo Request), 0 (Echo Reply)
Code: Specifies various error conditions.
Checksum: Validates data integrity.
Identifier & Sequence Number: Used for tracking packets.
Data Field: Can be user-defined (e.g., timestamp).

Example ICMP Header in C:
struct icmphdr {
    uint8_t  type;        // ICMP Type (8: Echo Request, 0: Echo Reply)
    uint8_t  code;        // ICMP Code (usually 0)
    uint16_t checksum;    // ICMP Checksum (must be computed)
    uint16_t identifier;  // Process ID for ping (getpid())
    uint16_t sequence;    // Ping sequence number
};

---

6 . ICMP Use Cases

ICMP is used for error reporting between two devices connected via the internet. A router sends an error message to the source, reporting the issue. For instance, if a sender transmits a message that is too large, the receiver may drop the message and respond to the sender with an ICMP message.
ICMP is also crucial for network diagnostics, typically using tools like Traceroute and Ping:
Traceroute: Determines the path between two devices. Used to identify network issues before data transmission.
Ping: A simpler version of Traceroute that measures round-trip time by sending an Echo Request message and receiving an Echo Reply.

---

How the Ping Command Works

Sending an ICMP Echo Request: When the ping command is executed, an ICMP Echo Request packet is sent to the target IP address.
Receiving an Echo Reply: The target device receives this request and sends back an ICMP Echo Reply packet.
Measuring Response Time: The source device calculates the round-trip time and displays the result.
Identifying Packet Loss: If some ICMP packets do not receive a response, they are reported as lost packets.

---

Ping Usage Purposes

Testing network connectivity: Checks whether a specific device is reachable.
Measuring network latency: Evaluates network performance by calculating round-trip time.
Detecting packet loss: Identifies potential network issues.
Testing DNS resolution: By using a domain name instead of an IP address, the DNS configuration can be verified.

---

How Does ICMP Work?

ICMP (Internet Control Message Protocol) is a fundamental and essential protocol within the IP protocol suite. However, it is not associated with any transport layer protocol (such as TCP or UDP) because it can send messages without establishing a connection. In other words, ICMP is a connectionless protocol.
The way ICMP works is different from TCP. While TCP is a connection-oriented protocol, requiring both devices to establish a connection before sending messages, ICMP does not require such a connection.
ICMP packets are transmitted in the form of datagrams, which contain an IP header and ICMP data. An ICMP datagram is similar to a packet, as it is an independent unit of data.

---

7 . ICMP Packet Structure Analysis

An ICMP Echo Request or Echo Reply packet is structured as follows:
ICMP Header (8 Bytes):
Contains information such as packet type (Echo Request or Echo Reply), code, and checksum (error control).
The header is always 8 bytes long.

ICMP Data(Payload) size (56 Bytes):
The data section of Echo Request and Echo Reply packets is typically 56 bytes long.
This data is sent by the ICMP protocol and transmitted to the target device.

---

8 . Sending an ICMP Packet (sendto)

To send ICMP packets, we use the sendto function. This function sends an ICMP Echo Request (ping) packet to a specific destination.
void send_ping(int sockfd, struct sockaddr_in *dest_addr, int sequence_number)
{
    uint8_t buf[sizeof(struct icmphdr) + ICMP_BODY_SIZE] = {};

    if (fill_icmp_echo_packet(buf, sizeof(buf), sequence_number) == -1) {
        perror("Failed to create ICMP packet");
        exit(EXIT_FAILURE);
    }

    long sent_bytes = sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)dest_addr, sizeof(*dest_addr));
    if (sent_bytes <= 0) {
        perror("Error sending packet");
        exit(EXIT_FAILURE);
    }
}

The send_ping function is responsible for sending ICMP Echo Request packets (ping).
Creating the ICMP Packet: The fill_icmp_echo_packet function is called to construct the ICMP Echo Request (ping) packet. This packet is populated using the provided sequence_number and packet_len.
Sending the Packet: The sendto function is used to send the ICMP packet over the specified socket (sockfd) to the destination address (dest_addr). The size of the packet is also passed.

If sending the packet fails, an error message is printed, and the program exits.

Parameters:
sockfd: The socket file descriptor used for sending the ICMP packet.
dest_addr: A structure containing the destination IP address and port.
sequence_number: A unique identifier for the sent ping packet.

---

9 . Receiving an ICMP Packet (recvfrom)

The recvfrom function is used to receive an ICMP Echo Reply (ping response) packet.
int receive_ping(int sockfd, struct sockaddr_in *recv_addr, struct timeval *send_time)
{
    uint8_t recv_packet[sizeof(struct icmphdr) + ICMP_BODY_SIZE + sizeof(struct ip)];
    socklen_t addr_len = sizeof(*recv_addr);

    int recv_size = recvfrom(sockfd, recv_packet, sizeof(recv_packet), 0, (struct sockaddr *)recv_addr, &addr_len);
    if (recv_size < 0) {
        perror("Failed to receive response");
        return -1;
    }

    struct ip *ip_header = (struct ip *)recv_packet;
    int ip_header_len = ip_header->ip_hl * 4;
    struct icmphdr *icmp_reply = (struct icmphdr *)(recv_packet + ip_header_len); // 1 - IP, 2 - ICMP

    if (icmp_reply->type == ICMP_ECHOREPLY) {
        memcpy(send_time, recv_packet + ip_header_len + sizeof(struct icmphdr), sizeof(struct timeval));
        return (ip_header->ip_ttl);  // Return the TTL value
    }

    if (icmp_reply->type == ICMP_UNREACH || icmp_reply->type == ICMP_TIMXCEED || icmp_reply->type == ICMP_PARAMPROB) {
        printf("Error: Host unreachable\n");
        exit(1);
    }

    return -1;
}
The receive_ping function is used to receive ICMP Echo Reply packets (ping responses) from the network. It works as follows:

Receiving the Data: The recvfrom function is called to receive an ICMP packet. The packet is stored in the recv_packet buffer, and the source address (recv_addr) is populated.
Parsing the IP and ICMP Headers: The received packet is parsed to extract the IP header and the ICMP header. The length of the IP header is calculated.
Handling the ICMP Reply:

If the ICMP packet type is ICMP_ECHOREPLY, it means we received a valid ping response. The timestamp from the packet is copied into send_time, and the TTL (Time To Live) value is returned.
If the packet type is ICMP_UNREACH (host unreachable), ICMP_TIMXCEED (time exceeded), or ICMP_PARAMPROB (parameter problem), an error message is displayed, and the program exits.

Parameters:
sockfd: The socket file descriptor used for receiving the ICMP packet.
recv_addr: A structure that will hold the source IP address and port of the received packet.
send_time: A structure where the timestamp from the received packet is copied (this is the time when the ping was sent).

Return Value:
If successful, it returns the TTL value from the IP header.
If an error occurs, it returns -1.

The fill_icmp_echo_packet function is responsible for constructing an ICMP Echo Request packet (a "ping" request). It populates the ICMP header with the necessary information, including the packet's type, code, identifier, sequence number, and checksum. It also sets a timestamp in the packet to record the time the request was sent.
int fill_icmp_echo_packet(uint8_t *buf, int packet_len, int sequence_number)
{
    struct icmphdr *hdr = (struct icmphdr *)buf;
    struct timeval *timestamp = (struct timeval *)(buf + sizeof(struct icmphdr));

    if (gettimeofday(timestamp, NULL) == -1)
        return -1;

    hdr->type = ICMP_ECHO;
    hdr->code = 0;
    hdr->un.echo.id = getpid();
    hdr->un.echo.sequence = sequence_number;
    hdr->checksum = checksum((unsigned short *)buf, packet_len);

    return 0;
}

hdr is a pointer to an icmphdr structure, which represents the ICMP header. The buffer buf is cast to this structure type so that the program can modify the header's fields.
timestamp is a pointer to a struct timeval that will store the timestamp (the time the packet is sent). It's placed immediately after the ICMP header in the buf array. sizeof(struct icmphdr) gives the size of the ICMP header, so adding that value to buf positions timestamp right after the header.
The gettimeofday function is used to get the current time and store it in the timestamp. This records the time at which the ping request is sent. If gettimeofday fails, the function returns -1.
Set the ICMP Header Fields:
hdr->type = ICMP_ECHO;
hdr->code = 0;
hdr->un.echo.id = getpid();
hdr->un.echo.sequence = sequence_number;
hdr->type = ICMP_ECHO: Sets the type of the ICMP packet to "Echo Request" (ping request).

hdr->code = 0: Sets the code to 0, which is the standard for an Echo Request.
hdr->un.echo.id = getpid(): Sets the identifier for the ICMP Echo Request. It uses the process ID of the current program (getpid()), which uniquely identifies the packet.
hdr->un.echo.sequence = sequence_number: Sets the sequence number for the packet, which helps identify each individual ping request. The sequence number is passed as a parameter to the function.
hdr->checksum = checksum((unsigned short *)buf, packet_len);

The checksum is calculated for the ICMP packet using the checksum function. The checksum is a way of verifying the integrity of the packet, ensuring that it has not been corrupted during transmission.
The checksum function is passed the packet buffer (buf) and its length (packet_len) to compute the correct checksum.
If all steps are completed successfully, the function returns 0, indicating that the ICMP packet was created successfully.
Parameters:
uint8_t *buf: A pointer to the buffer that will hold the ICMP Echo Request packet.
int packet_len: The length of the packet, including the ICMP header and the timestamp.
int sequence_number: A sequence number that identifies the particular ping request being sent.

Return Value:
Returns 0 if the packet is successfully created.
Returns -1 if there is an error in getting the current time (e.g., if gettimeofday fails).

---

10 . Adjusting Packet Size

The ICMP packet size can be modified using the -s (size) parameter in the ping command:
ping -s 128 <destination_ip>
This command sets the ICMP Echo Request data section to 128 bytes.
Larger packets increase network load, affecting latency and error rates.

---

11 . Adjusting Packet Count & Duration

To send a specific number of packets:
ping -c 10 <destination_ip>
This sends 10 ICMP Echo Requests to yahoo.com.

ICMP operates similarly to a sonar echo system:
A small ICMP Echo Request is sent.
The recipient sends back an ICMP Echo Reply.
The round-trip time (RTT) is measured.
If the packet does not reach its destination, an error notification is generated.

Common ICMP errors include:
TTL Expired (packet exceeded allowed hops).
Destination Host Unreachable.
Timeout (no response received).

This mechanism helps diagnose network issues and measure connectivity performance. 🚀

---

12 . TTL (Time to Live) Value

TTL is a value that specifies the maximum number of routers an IP packet can pass through during its journey. In other words, TTL is decreased by one by each router as the packet travels through the network, and when TTL reaches zero, the packet is discarded from the network.
When using the Ping command, the TTL value can be an indicator of the length of the network path. If a packet returns from a very distant server, the TTL value will be lower because it has passed through more routers.
This value is used to prevent a packet from circulating endlessly in the network if it accidentally enters an infinite loop. Each router decreases the TTL value by one, and if the TTL reaches zero, the packet is discarded.

---

13 . ICMP Checksum Packet Analysis

Example ICMP Echo Request Packet (in HEX format)
08 00 00 00 12 34 00 01  61 62 63 64 65 66 67 68
Let's break down the sections of this packet:
Step 1: Divide into 16-bit blocks
Let's split the packet into 16-bit blocks:
0800 0000 1234 0001 6162 6364 6566 6768
Blocks:
Step 2: Sum the blocks
Let's sum all the blocks in hexadecimal:
0800 + 0000 + 1234 + 0001 + 6162 + 6364 + 6566 + 6768
Calculations:
0x0800  = 2048
0x0000  = 0
0x1234  = 4660
0x0001  = 1
0x6162  = 24930
0x6364  = 25444
0x6566  = 25958
0x6768  = 26472
--------------------
Total: 89373 (0x15D37)
Step 3: Add carry bit
The result is 0x15D37, which exceeds a 16-bit value, so the carry bit (0x1) is added to the lower part:
0x5D37 + 0x0001 = 0x5D38

---

Step 4: Take the one's complement
To obtain the checksum value, we flip 0x5D38:
0xFFFF - 0x5D38 = 0xA2C7
In this case, the checksum value is: A2C7

---

Step 5: Insert the checksum into the packet
Now, let's insert the checksum value into the packet:
08 00 A2 C7 12 34 00 01  61 62 63 64 65 66 67 68
Rearranged ICMP packet:

---

14 . Verification Process on the Receiver Side

When the receiver gets the packet, it sees the checksum field as 0xA2C7.
All fields in the packet are summed again (including the checksum field).
If the result is 0xFFFF, the packet is valid; otherwise, there is an error.
For example:
0x0800 + 0x0000 + 0x1234 + 0x0001 + 0x6162 + 0x6364 + 0x6566 + 0x6768 + 0xA2C7 = 0xFFFF (Valid)
Since the result is 0xFFFF, the packet is error-free.

---

Summary of Checksum Calculation Steps
Divide into 16-bit blocks.
Sum the blocks.
Add the carry bit.
Take the one's complement.
Insert the checksum into the packet.
Perform verification on the receiver side.

---

15 . Start Packet Capture Using Wireshark

The Wireshark screen will be appeared.
Double-click the interface name you want to monitor, and you can see the packets sent to the interface.
After compiling the ft_ping.c code with make, you can ping and check network connectivity using ./ft_ping <destination_addr>.
$ cd /ft_ping  
$ sudo make re  
$ ./ft_ping <destination_addr>  
Sent!

---

16 . Check Frames with Wireshark

Wireshark icmp packets analyze

---

SOURCES

Cisco Learning Network
Edit descriptionlearningnetwork.cisco.com
https://www.baeldung.com/cs/raw-sockets
GitHub - sensoyyasin/pinG: ping - dns resolution
ping - dns resolution . Contribute to sensoyyasin/pinG development by creating an account on GitHub.github.com
