#include <stdio.h>
#include <stdlib.h>

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i = 0;
	char	*str1 = (char *)src;
	char	*str2 = (char *)dest;

	if (!str1 && !str2)
		return (NULL);
	while (i < n)
	{
		str2[i] = str1[i];
		i++;
	}
	return (str2);
}
