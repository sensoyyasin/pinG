#include "ft_ping.h"
#include <sys/time.h>

int packets_sent = 0;
int packets_received = 0;
double total_time = 0;
double min_time = -1;
double max_time = -1;

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s target_name\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct addrinfo *result;
    remove_protocol(argv[1]);

    signal(SIGINT, handle_siginit);

    int ai_res = resolve_dns(argv[1], &result);

    if (ai_res != 0) {
        print_dns_error(ai_res);
        freeaddrinfo(result);
        exit(EXIT_FAILURE);
    }

    // Print resolved IP addresses
    char dest_ip[NI_MAXHOST];
    print_resolved_ips(result, dest_ip);
    printf("PING %s (%s) bytes of data.\n", argv[1], dest_ip);

    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(EXIT_FAILURE);
    }

    // Prepare ICMP Packet
    struct icmp icmp_packet;
    icmp_packet.icmp_type = ICMP_ECHO;  // Echo Request (Type 8)
    icmp_packet.icmp_code = 0;
    icmp_packet.icmp_seq = 1;

    // Set destination address
    struct sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = 0;  // Port number not used in raw sockets
    dest_addr.sin_addr.s_addr = inet_addr(dest_ip);

    if (dest_addr.sin_family != AF_INET) {
        perror("Wrong IPv4 address");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Dynamically calculate packet size
    int datalen = 56;  // Example data length (could be passed as an argument)
    int packlen = datalen + sizeof(struct ip) + sizeof(struct icmp);
    unsigned char *packet = (unsigned char *)malloc(packlen);

    if (!packet) {
        perror("Out of memory\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
            struct timeval send_time;
            gettimeofday(&send_time, NULL);

        icmp_packet.icmp_cksum = checksum(&icmp_packet, sizeof(icmp_packet));

        // Send the packet
        if (sendto(sockfd, &icmp_packet, sizeof(icmp_packet), 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr)) <= 0) {
            perror("Error sending packet");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        // Receive the ICMP reply
        char recv_packet[64 + sizeof(struct ip)];
        struct sockaddr_in recv_addr;
        socklen_t addr_len = sizeof(recv_addr);

        int recv_size = recvfrom(sockfd, recv_packet, sizeof(recv_packet), 0, (struct sockaddr *)&recv_addr, &addr_len);
        if (recv_size < 0) {
            perror("Failed to receive response");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        // Parse IP and ICMP headers
        struct ip *ip_header = (struct ip *)recv_packet;
        int ip_header_len = ip_header->ip_hl * 4;  // IP header length
        struct icmp *icmp_reply = (struct icmp *)(recv_packet + ip_header_len);

        if (icmp_reply->icmp_type != ICMP_ECHOREPLY) {
            fprintf(stderr, "Wrong ICMP response type : %d\n", icmp_reply->icmp_type);
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        struct timeval recv_time;
        gettimeofday(&recv_time, NULL);

        double send_ms = (send_time.tv_sec * 1000.0) + (send_time.tv_usec / 1000.0);
        double recv_ms = (recv_time.tv_sec * 1000.0) + (recv_time.tv_usec / 1000.0);
        double rtt = recv_ms - send_ms; // RTT in ms

        packets_sent++;
        packets_received++;

        total_time += rtt;
        if (min_time == -1 || rtt < min_time)
                min_time = rtt;
        if (rtt > max_time)
                max_time = rtt;

        // Print ICMP reply in the specified format
        printf("64 bytes from %s (%s): icmp_seq=%d ttl=%d time=%.3f ms\n",
               inet_ntoa(recv_addr.sin_addr),
               dest_ip,
               icmp_reply->icmp_seq,
               ip_header->ip_ttl,
               rtt);

        icmp_packet.icmp_seq++;
        sleep(1);
    }

    close(sockfd);
    return 0;
}
