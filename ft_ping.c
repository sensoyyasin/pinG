#include "ft_ping.h"

int main(int argc, char *argv[])
{
    if (argc != 2) {
        write(2, "arg error!\n", 11);
        return 1;
    }

    const char *target_ip = argv[1];
    struct sockaddr_in dest_addr;
    char packet[PING_PACKET_SIZE];
    struct icmp *icmp_packet = (struct icmp *)packet;
    long rtt;
    struct timeval start, end, timeout;


    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        write(2, "Socket creation failed\n", 22);
        return -1;
    }


    timeout.tv_sec = TIMEOUT_SECONDS;
    timeout.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) < 0) {
        write(2, "Socket timeout setting failed\n", 29);
        close(sockfd);
        return -1;
    }

    // Setup for DNS resolution
    struct addrinfo hints, *res;
    ft_memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;

    // Perform DNS resolution
    int status = getaddrinfo(target_ip, NULL, &hints, &res);
    if (status != 0) {
        write(2, "DNS resolution failed\n", 21);
        close(sockfd);
        return -1;
    }

    // Prepare destination address structure
    ft_memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    ft_memcpy(&dest_addr.sin_addr, &((struct sockaddr_in *)res->ai_addr)->sin_addr, sizeof(struct in_addr));


    create_packet(icmp_packet);


    gettimeofday(&start, NULL);


    if (sendto(sockfd, packet, sizeof(struct icmp), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) <= 0) {
        write(2, "Failed to send packet\n", 21);
        close(sockfd);
        freeaddrinfo(res);
        return -1;
    }

    printf("PING %s (%s)\n", target_ip, inet_ntoa(dest_addr.sin_addr));


    char recv_packet[PING_PACKET_SIZE + sizeof(struct ip)]; // Extra space for IP header
    struct sockaddr_in recv_addr;
    socklen_t addr_len = sizeof(recv_addr);

    int recv_size = recvfrom(sockfd, recv_packet, sizeof(recv_packet), 0, (struct sockaddr *)&recv_addr, &addr_len);

    if (recv_size <= 0) {
        write(2, "Failed to receive response\n", 26);
        close(sockfd);
        freeaddrinfo(res);
        return -1;
    }

    struct ip *ip_header = (struct ip *)recv_packet;
    int ip_header_len = ip_header->ip_hl * 4;
    struct icmp *icmp_reply = (struct icmp *)(recv_packet + ip_header_len);

    if (icmp_reply->icmp_type != ICMP_ECHOREPLY) {
        write(2, "Wrong ICMP response type\n", 24);
        close(sockfd);
        freeaddrinfo(res);
        return -1;
    }

    gettimeofday(&end, NULL);
    rtt = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;

    // Print results
    printf("64 bytes from %s: icmp_seq=%d ttl=%d time=%ld ms\n",
           inet_ntoa(recv_addr.sin_addr),
           icmp_reply->icmp_seq,
           ip_header->ip_ttl,
           rtt);

    close(sockfd);
    freeaddrinfo(res);
    return 0;
}
