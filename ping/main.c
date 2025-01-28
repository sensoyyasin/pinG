#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>

int main(int argc, char **argv) {

    if (argc != 2) {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct addrinfo *result;
    int ai_res = resolve_dns(argv[1], &result);

    if (ai_res != 0) {
        print_dns_error(ai_res);
        freeaddrinfo(result);
        exit(EXIT_FAILURE);
    }

    // Print resolved IP addresses
    print_resolved_ips(result);

    struct sockaddr_in *addr_in = (struct sockaddr_in *)result->ai_addr;
    const char *dest_ip = inet_ntoa(addr_in->sin_addr);

    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(EXIT_FAILURE);
    }

    // Prepare ICMP Packet
    struct icmp icmp_packet;
    icmp_packet.icmp_type = ICMP_ECHO;  // Echo Request (Type 8)
    icmp_packet.icmp_code = 0;
    icmp_packet.icmp_id = getpid();
    icmp_packet.icmp_seq = 1;
    icmp_packet.icmp_cksum = checksum(&icmp_packet, sizeof(icmp_packet));

    // Set destination address
    struct sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = 0;
    dest_addr.sin_addr.s_addr = inet_addr(dest_ip);

    // Send the packet
    if (sendto(sockfd, &icmp_packet, sizeof(icmp_packet), 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr)) <= 0) {
        perror("Error sending packet");
        freeaddrinfo(dest_addr);
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Receive the ICMP reply
    char recv_packet[64 + sizeof(struct ip)];
    struct sockaddr_in recv_addr;
    socklen_t addr_len = sizeof(recv_addr);
    
    int recv_size = recvfrom(sockfd, recv_packet, sizeof(recv_packet), 0, (struct sockaddr *)&recv_addr, &addr_len);
    if (recv_size <= 0) {
        perror("Failed to receive response");
        freeaddrinfo(recv_addr);
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Parse IP and ICMP headers
    struct ip *ip_header = (struct ip *)recv_packet;
    int ip_header_len = ip_header->ip_hl * 4;
    struct icmp *icmp_reply = (struct icmp *)(recv_packet + ip_header_len);

    if (icmp_reply->icmp_type != ICMP_ECHOREPLY) {
        perror("Wrong ICMP response type");
        freeaddrinfo(ip_header);
        freeaddrinfo(icmp_reply);
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("64 bytes from %s: icmp_seq=%d ttl=%d\n",
           inet_ntoa(recv_addr.sin_addr),
           icmp_reply->icmp_seq,
           ip_header->ip_ttl);

    close(sockfd);
    return 0;
}
