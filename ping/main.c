#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>         
#include <string.h>         
#include <arpa/inet.h>      
#include <sys/socket.h>     
#include <netinet/in.h>     
#include <netinet/ip.h>     
#include <netinet/ip_icmp.h> 
#include <unistd.h>

struct icmphdr_custom {
    uint8_t type;    // ICMP message type (Echo Request = 8, Echo Reply = 0)
    uint8_t code;
    uint16_t checksum;
    uint16_t id;
    uint16_t sequence;
};

int main(int argc, char **argv) {

    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        perror("error in socket");
        return -1;
    }

    printf("ICMP packet sniffer started...\n");

    unsigned char *buffer = malloc(65536);
    memset(buffer, 0, 65536);
    struct sockaddr_in source;
    socklen_t addr_len = sizeof(source);

    // Listen continuously for packets
    while (1) {
        size_t packet_size = recvfrom(sockfd, buffer, 65536, 0, (struct sockaddr *)&source, &addr_len);
        
        if (packet_size < 0) {
            perror("Failed to receive packet");
            close(sockfd);
            free(buffer);
            return -1;
        }

        struct iphdr *ip_header = (struct iphdr *)buffer;
        struct icmphdr_custom *icmp_packet = (struct icmphdr_custom *)(buffer + (ip_header->ihl * 4));

        printf("Source IP: %s\n", inet_ntoa(*(struct in_addr *)&ip_header->saddr));
        printf("Destination IP: %s\n", inet_ntoa(*(struct in_addr *)&ip_header->daddr));
        printf("ICMP Type: %d\n", icmp_packet->type);  // 8 -> echo request, 0 -> echo reply
        printf("ICMP Code: %d\n", icmp_packet->code);
        printf("ICMP ID: %d\n", ntohs(icmp_packet->id));
        printf("ICMP Sequence Number: %d\n", ntohs(icmp_packet->sequence));
    }

    close(sockfd);
    free(buffer);  // Free the allocated memory for the buffer
    return 0;
}