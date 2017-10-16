/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strccmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/28 18:54:55 by rabougue          #+#    #+#             */
/*   Updated: 2016/11/19 22:17:12 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libft.h"

int	ft_strccmp(const char *s1, const char *s2, char c)
{
	int				i;
	int				j;
	unsigned char	*ss1;
	char			*tmp;

	ss1 = (unsigned char *)s1;
	i = 0;
	j = 0;
	tmp = ft_strcdup(s2, c);
	while (ss1[i] != '\0')
	{
		if (tmp[j] != ss1[i])
			return (ss1[i] - tmp[j]);
		i++;
		j++;
	}
	free(tmp);
	return (0);
}
