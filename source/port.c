/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   port.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/05 21:44:03 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/24 05:15:54 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

uint16_t	is_port_valid(char **argv, bool type)
{
	uint64_t	port;

	if (type == SERVER)
	{
		if (argv[1][0] == '-' ||
		(port = (uint64_t)ft_atoi(argv[1])) < 1024 || port > 65535)
			return ((uint16_t)usage(argv[0], SERVER));
	}
	else
	{
		if (argv[1][0] == '-' ||
		(port = (uint64_t)ft_atoi(argv[2])) < 1024 || port > 65535)
			return ((uint16_t)usage(argv[0], CLIENT));
	}
	return ((uint16_t)port);
}
