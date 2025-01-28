#ifndef FT_PING_H
#define FT_PING_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/time.h>

#define PING_PACKET_SIZE 64
#define TIMEOUT_SECONDS 2

// Color definitions
#define YELLOW "\033[1;33m"
#define RESET  "\033[0m"

// Function prototypes
void    *ft_memset(void *b, int c, size_t len);
void    *ft_memcpy(void *dest, const void *src, size_t n);
unsigned short checksum(void *b, int len);
void    create_packet(struct icmp *icmp_packet);
void    print_resolved_ips(struct addrinfo *ai_result);
int     resolve_dns(const char *hostname, struct addrinfo **result);
void    print_dns_error(int error_code);
void    remove_protocol(char *url);

#endif