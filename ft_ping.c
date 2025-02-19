#include "ft_ping.h"

char *ping_target;

int main(int argc, char **argv) {

    if (argc != 2)
    {
        printf("Usage: ft_ping [Options] <destination address>\n");
        printf("Options:\n");
        printf(RED "  -v      Enable verbose output\n" RESET);
        printf(GRAY "  -?      Show this help message\n" RESET);
        exit(1);
    }

    arg_check(argv[1]);
    remove_protocol(argv[1]);

    ping_target = argv[1];

    signal(SIGINT, handle_siginit);

    packets_sent = 0;
    packets_received = 0;

    struct addrinfo *result;
    int ai_res = resolve_dns(argv[1], &result);
    if (ai_res != 0) {
        print_dns_error(ai_res);
        freeaddrinfo(result);
        exit(EXIT_FAILURE);
    }

    char dest_ip[NI_MAXHOST];
    print_resolved_ips(result, dest_ip);
    printf("PING %s (%d) data bytes\n", argv[1], (PING_PACKET_SIZE - PING_HEADER_LEN));

    // Create raw socket for ICMP
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0)
    {
        perror("Error opening socket");
        freeaddrinfo(result);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = 0;

    if (inet_pton(AF_INET, dest_ip, &dest_addr.sin_addr) <= 0)
    {
        perror("Invalid IP address");
        close(sockfd);
        freeaddrinfo(result);
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result);

    int sequence_number = 1;

    while (1)
    {
        struct timeval send_time;
        send_ping(sockfd, &dest_addr, sequence_number);
        packets_sent++;

        struct sockaddr_in recv_addr;
        struct timeval recv_time;

        gettimeofday(&recv_time, NULL);

        int ttl = receive_ping(sockfd, &recv_addr, &send_time);
        if (ttl > 0)
        {
            packets_received++;
            long sec_diff = recv_time.tv_sec - send_time.tv_sec;
            long usec_diff = recv_time.tv_usec - send_time.tv_usec;

            float rtt = ((sec_diff * 1000.0) + (usec_diff / 1000.0) * 1000);

            printf("64 bytes from %s (%s): icmp_seq=%d ttl=%d time=%.3f ms\n",
                inet_ntoa(recv_addr.sin_addr),
                dest_ip,
                sequence_number,
                ttl,
                rtt);
        }
        sleep(1);
        sequence_number++;
    }
    close(sockfd);
    return 0;
}
