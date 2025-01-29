#include "ft_ping.h"

void    *ft_memset(void *b, int c, size_t len)
{
        size_t  i = 0;
        char    *str = (char *)b;

        while (len > i)
                str[i++] = c;
        return (b);
}

void    *ft_memcpy(void *dest, const void *src, size_t n)
{
    size_t i = 0;
    char *str1 = (char *)src;
    char *str2 = (char *)dest;

    if (!str1 && !str2)
        return NULL;
    while (n > i)
    {
        str2[i] = str1[i];
        i++;
    }
    return str2;
}

int     ft_strlen (char *str)
{
        int i = 0;

        while (str[i])
                i++;

        return (i);
}

void handle_eof()
{
    printf("\n" RED "Control D (EOF) received!" RESET);
    exit(1);
}

double  get_time_in_ms()
{
        struct timeval time_val;
        gettimeofday(&time_val, NULL);

        return (time_val.tv_sec * 1000.0) + (time_val.tv_usec / 1000.0);
}

void    handle_siginit(int signum)
{
        if (signum == SIGINT)
        {
                printf("\n" YELLOW "Control C (EOF) received!" RESET);
                print_statistics();
                exit(0);
        }
}

void    print_statistics()
{
        if (packets_sent == 0) {
                printf("No packets were sent.\n");
                return;
        }

        int packet_loss = packets_sent - packets_received;
        double loss_percentage = (packets_sent > 0) ? (packet_loss * 100.0) / packets_sent : 0.0;

        // RTT average
        double avg_rtt = (packets_received > 0) ? (total_time / packets_received) : 0.0;

        printf("\n--- Ping statistics ---\n");
        printf("%d packets transmitted, %d received, %.0f%% packet loss, time %.0fms\n",
            packets_sent, packets_received, loss_percentage, total_time);
        printf("rtt min/avg/max = %.3f/%.3f/%.3f ms\n", min_time, avg_rtt, max_time);
}
