#include "./includes/libft.h"

char	*ft_str_erase_after_last(char *str, char c)
{
	if (!ft_strchr(str, c))
		return (NULL);
	str[ft_strrclen(str, c)] = '\0';
	return (str);
}
