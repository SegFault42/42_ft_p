/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   port.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/05 21:44:03 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/05 21:47:30 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

int	is_port_valid(char **argv, bool type)
{
	int	port;

	if (type == SERVER)
	{
		if ((port = ft_atoi(argv[1])) < 1024 || port > 65535)
			usage(argv[0], SERVER);
	}
	else
	{
		if ((port = ft_atoi(argv[2])) < 1024 || port > 65535)
			usage(argv[0], CLIENT);
	}
	return (port);
}

