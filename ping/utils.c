#include <stdio.h>
#include <stdlib.h>

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
