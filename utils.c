#include "ft_ping.h"

unsigned short checksum(void *b, int len)
{
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2) {
        sum += *buf++;
    }
    if (len == 1) {
        sum += *(unsigned char *)buf;
    }

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

void create_packet(struct icmp *icmp_packet)
{
    ft_memset(icmp_packet, 0, sizeof(struct icmp));

    icmp_packet->icmp_type = ICMP_ECHO;
    icmp_packet->icmp_code = 0;
    icmp_packet->icmp_id = getpid();
    icmp_packet->icmp_seq = 1;
    icmp_packet->icmp_cksum = 0;

    icmp_packet->icmp_cksum = checksum((unsigned short *)icmp_packet, sizeof(struct icmp));
}
