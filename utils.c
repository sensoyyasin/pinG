#include "ft_ping.h"

void    *ft_memset(void *b, int c, size_t len)
{
        size_t  i = 0;
        char    *str = (char *)b;

        while (len > i)
                str[i++] = c;
        return (b);
}

int ft_strlen(char *str)
{
    int i = 0;

    while (str[i])
        i++;
    return (i);
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

void handle_eof()
{
    printf("\n" RED "Control D (EOF) received! Input terminating...\n" RESET);
    exit(1);
}

void handle_siginit(int signum)
{
    extern char *ping_target;

    if (signum == SIGINT)
    {
        if (packets_sent == 0) {
            printf("\nNo packets were transmitted.\n");
        } else {
            printf("\n" YELLOW "Control C (EOF) received! Input terminating...\n" RESET);
            printf("\n--- %s ping statistics ---\n", ping_target);
            printf("%d packets transmitted, %d packets received, %.1f%% packet loss\n", packets_sent, packets_received,
                   (float)(packets_sent - packets_received) / packets_sent * 100);
        }
        exit(0);
    }
}

int	ft_strcmp(char *s1, char *s2)
{
	unsigned char	*k1;
	unsigned char	*k2;
	size_t			i;

	k1 = (unsigned char *)s1;
	k2 = (unsigned char *)s2;
	i = 0;
	while (k1[i] && k2[i] && k1[i] == k2[i])
		i++;
	return (k1[i] - k2[i]);
}

int ft_isalnum(int c)
{
	if ((c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z'))
		return (1);
	else if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}
