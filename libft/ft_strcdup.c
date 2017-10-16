/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/25 21:53:58 by rabougue          #+#    #+#             */
/*   Updated: 2016/09/15 12:38:16 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libft.h"

char	*ft_strcdup(const char *s1, char c)
{
	int		i;
	int		j;
	char	*dst;

	i = 0;
	j = ft_strclen(s1, ' ');
	dst = (char *)malloc(sizeof(char) * (j + 1));
	if (dst == NULL)
		return (NULL);
	while (s1[i] != c)
	{
		if (s1[i] == '\0')
			break ;
		dst[i] = s1[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}
