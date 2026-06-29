#include "../header/ft_traceroute_bonus.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (unsigned char) *s1 == (unsigned char) *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char) *s1 - (unsigned char) *s2);
}

size_t	ft_strlen(const char *s)
{
	const char	*tmp;

	if (!s)
		return (0);
	tmp = s;
	while (*tmp)
		tmp++;
	return (tmp - s);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;

	if (!dst || !src)
		return (0);
	if (size == 0)
		return (ft_strlen(src));
	i = 0;
	while (src[i] && i < size - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (ft_strlen(src));
}
