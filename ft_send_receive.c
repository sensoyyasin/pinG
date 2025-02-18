#include "ft_ping.h"

void send_ping(int sockfd, struct sockaddr_in *dest_addr, int sequence_number)
{
    struct icmp icmp_packet;
    struct timeval send_time;

    memset(&icmp_packet, 0, sizeof(icmp_packet));
    icmp_packet.icmp_type = ICMP_ECHO;
    icmp_packet.icmp_code = 0;
    icmp_packet.icmp_id = getpid();
    icmp_packet.icmp_seq = sequence_number;

    gettimeofday(&send_time, NULL);
    memcpy(icmp_packet.icmp_data, &send_time, sizeof(send_time));

    icmp_packet.icmp_cksum = checksum(&icmp_packet, sizeof(icmp_packet));

    long sent_bytes = sendto(sockfd, &icmp_packet, sizeof(icmp_packet), 0, (struct sockaddr *)dest_addr, sizeof(*dest_addr));
    if (sent_bytes <= 0) {
        perror("Error sending packet");
        exit(EXIT_FAILURE);
    }
}

int receive_ping(int sockfd, struct sockaddr_in *recv_addr, struct timeval *send_time)
{
    char recv_packet[64 + sizeof(struct ip)];
    socklen_t addr_len = sizeof(*recv_addr);

    // Receive response packet
    int recv_size = recvfrom(sockfd, recv_packet, sizeof(recv_packet), 0, (struct sockaddr *)recv_addr, &addr_len);
    if (recv_size < 0) {
        perror("Failed to receive response");
        return -1;
    }

    // Parse IP header and ICMP reply
    struct ip *ip_header = (struct ip *)recv_packet;
    int ip_header_len = ip_header->ip_hl * 4;
    struct icmp *icmp_reply = (struct icmp *)(recv_packet + ip_header_len);

    if (icmp_reply->icmp_type == ICMP_ECHOREPLY) {
        memcpy(send_time, icmp_reply->icmp_data, sizeof(struct timeval));
        return (ip_header->ip_ttl);
    }
        /*
        #define ICMP_ERRORTYPE(type) \
        ((type) == ICMP_UNREACH || (type) == ICMP_SOURCEQUENCH || \
        (type) == ICMP_REDIRECT || (type) == ICMP_TIMXCEED || \
        (type) == ICMP_PARAMPROB)
        */
    if (icmp_reply->icmp_type == (ICMP_ERRORTYPE(ICMP_UNREACH) ||
        ICMP_ERRORTYPE(ICMP_TIMXCEED) || ICMP_ERRORTYPE(ICMP_PARAMPROB))) {
        printf("Error: Host unreachable\n");
        exit(1);
    }

    return -1;
}
