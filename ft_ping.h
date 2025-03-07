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
#include <signal.h>
#include <string.h>

#define PING_PACKET_SIZE 64
#define PING_HEADER_LEN 8
#define ICMP_BODY_SIZE 56
#define TIMEOUT_SECONDS 2

// Color definitions
# define DEFAULT "\033[0;39m"
# define GRAY "\033[0;90m"
# define RED "\033[0;91m"
# define GREEN "\033[0;92m"
# define YELLOW "\033[0;93m"
# define BLUE "\033[0;94m"
# define MAGENTA "\033[0;95m"
# define CYAN "\033[0;96m"
# define WHITE "\033[0;97m"
# define RESET  "\033[0m"

struct icmphdr {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    union {
        struct {
            uint16_t id;
            uint16_t sequence;
        } echo;
        uint32_t gateway;
        struct {
            uint16_t mtu;
        } frag;
    } un;
};

int packets_sent;
int packets_received;

// Function prototypes
void	*ft_memset(void *b, int c, size_t len);
void	*ft_memcpy(void *dest, const void *src, size_t n);
int		ft_strlen(char *str);
unsigned short checksum(void *b, int len);
void	create_packet(struct icmp *icmp_packet);
void	print_resolved_ips(struct addrinfo *ai_result, char *dest_ip);
int		resolve_dns(const char *hostname, struct addrinfo **result);
void	print_dns_error(int error_code);
void	remove_protocol(char *url);
void	handle_siginit(int signum);
void	handle_eof();
void	send_ping(int sockfd, struct sockaddr_in *dest_addr, int sequence_number);
int		receive_ping(int sockfd, struct sockaddr_in *recv_addr, struct timeval *send_time);
int		fill_icmp_echo_packet(uint8_t *buf, int packet_len, int sequence_number);
void	arg_check(char *argv);
int		ft_strcmp(char *s1, char *s2);
int		ft_isalnum(int c);

#endif
