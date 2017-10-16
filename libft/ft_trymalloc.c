/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_trymalloc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/12 05:33:13 by rabougue          #+#    #+#             */
/*   Updated: 2017/02/12 05:43:17 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libft.h"

void	*ft_trymalloc(size_t size)
{
	char	*str;

	str = NULL;
	if ((str = (void *)malloc(size)) == NULL)
	{
		ft_putendl_fd(RED"ft_trymalloc : Memory allocation failure"END, 2);
		exit(-1);
	}
	ft_memset(str, 0, size);
	return (str);
}
