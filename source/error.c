/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/05 01:05:21 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/06 00:24:14 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

int	ft_error(int error)
{
	ft_printf(RED"");
	if (error == FT_SOCKET_ERROR)
		ft_dprintf(2, "socket() failure !\n");
	else if (error == FT_GET_PROTO_BY_NAME_ERROR)
		ft_dprintf(2, "getprotobyname() failure !\n");
	else if (error == FT_BIND_ERROR)
		ft_dprintf(2, "bind() failure !\n");
	else if (error == FT_LISTEN_ERROR)
		ft_dprintf(2, "listen() failure !\n");
	else if (error == FT_OPEN_ERROR)
		ft_dprintf(2, "open() failure !\n");
	else if (error == FT_CONNECT_ERROR)
		ft_dprintf(2, "connect() failure !\n");
	ft_printf(""END);
	ft_dprintf(2, YELLOW"errno : %s\n"END, strerror(errno));
	exit(error);
}

int	usage(char *argv, bool type)
{
	if (type == SERVER)
		ft_dprintf(2, "Usage : %s [port] (between 1024 and 65535)\n", argv);
	else if (type == CLIENT)
		ft_dprintf(2, "Usage : %s [addr] [port] (between 1024 and 65535)\n", argv);
	exit(EXIT_FAILURE);
}
