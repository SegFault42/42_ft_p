#include "./includes/libft.h"

size_t	ft_strrclen(char *s, char c)
{
	size_t	i;

	if (!ft_strchr(s, c))
		return (0);
	ft_strrev(s);
	i = ft_strlen(s) - ft_strclen(s, c);
	ft_strrev(s);
	return (i);
}
