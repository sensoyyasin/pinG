#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>  // IPv4 için ICMP başlık dosyası
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>

unsigned short checksum(void *b, int len) {
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *(unsigned char *)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

void send_ping(int sockfd, struct sockaddr_in *dest_addr, int sequence_number) {
    struct icmp icmp_packet;
    struct timeval send_time;

    memset(&icmp_packet, 0, sizeof(icmp_packet));
    icmp_packet.icmp_type = ICMP_ECHO; // Echo Request
    icmp_packet.icmp_code = 0; // Code is 0 for Echo Request
    icmp_packet.icmp_id = getpid(); // Process ID
    icmp_packet.icmp_seq = sequence_number; // Sequence Number

    gettimeofday(&send_time, NULL);
    // Timestamp for response time calculation
    memcpy(icmp_packet.icmp_data, &send_time, sizeof(send_time));

    // Calculate checksum
    icmp_packet.icmp_cksum = checksum(&icmp_packet, sizeof(icmp_packet));

    // Send the packet
    if (sendto(sockfd, &icmp_packet, sizeof(icmp_packet), 0, (struct sockaddr *)dest_addr, sizeof(*dest_addr)) <= 0) {
        perror("Error sending packet");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s target_name\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in dest_addr;
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(EXIT_FAILURE);
    }

    // Set up destination address
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = 0;
    if (inet_pton(AF_INET, argv[1], &dest_addr.sin_addr) <= 0) {
        perror("Invalid IP address");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    int sequence_number = 1;

    // Continuous ping in a loop
    while (1) {
        send_ping(sockfd, &dest_addr, sequence_number);
        printf("Ping sent to %s, sequence number %d\n", argv[1], sequence_number);

        // Receive the reply (for simplicity, let's just wait for one reply)
        char recv_packet[64 + sizeof(struct ip)];
        struct sockaddr_in recv_addr;
        socklen_t addr_len = sizeof(recv_addr);

        int recv_size = recvfrom(sockfd, recv_packet, sizeof(recv_packet), 0, (struct sockaddr *)&recv_addr, &addr_len);
        if (recv_size < 0) {
            perror("Failed to receive response");
        } else {
            struct ip *ip_header = (struct ip *)recv_packet;
            int ip_header_len = ip_header->ip_hl * 4;
            struct icmp *icmp_reply = (struct icmp *)(recv_packet + ip_header_len);

            if (icmp_reply->icmp_type == ICMP_ECHOREPLY) {
                printf("Received ICMP Echo Reply from %s\n", inet_ntoa(recv_addr.sin_addr));
            } else {
                printf("Received non-Echo reply\n");
            }
        }

        // Sleep for 1 second before sending the next ping
        sleep(1);
        sequence_number++;
    }

    close(sockfd);
    return 0;
}
