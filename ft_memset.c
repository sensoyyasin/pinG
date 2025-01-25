#include <stdio.h>
#include <stdlib.h>

void	*ft_memset(void *b, int c, size_t len)
{
	size_t 	i = 0;
	char	*str = (char *)b;

	while (len > i)
		str[i++] = c;
	return (b);
}
