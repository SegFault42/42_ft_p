/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_erase_after_last.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/17 08:45:04 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/17 08:45:07 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libft.h"

char	*ft_str_erase_after_last(char *str, char c)
{
	if (!ft_strchr(str, c))
		return (NULL);
	str[ft_strrclen(str, c)] = '\0';
	return (str);
}
