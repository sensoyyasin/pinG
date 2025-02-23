#include "ft_ping.h"
/*
struct icmp {
    u_char icmp_type;    // 1 byte
    u_char icmp_code;    // 1 byte
    u_short icmp_cksum;  // 2 bytes (checksum)
    u_short icmp_id;     // 2 bytes (ID)
    u_short icmp_seq;    // 2 bytes (sequence number)
    char icmp_data[];    // Variable length (data payload)
};
*/

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
        return (ip_header->ip_ttl);  // TTL değerini geri döndür
    }

    if (icmp_reply->type == ICMP_UNREACH || icmp_reply->type == ICMP_TIMXCEED || icmp_reply->type == ICMP_PARAMPROB) {
        printf("Error: Host unreachable\n");
        exit(1);
    }

    return -1;
}
