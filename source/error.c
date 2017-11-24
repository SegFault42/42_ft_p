/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/05 01:05:21 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/24 05:16:20 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

extern char	*g_ft_errno[];

int	ft_error(int error)
{
	ft_printf(RED"");
	if (error == FT_SOCKET_ERROR)
		ft_dprintf(2, "socket() failure !");
	else if (error == FT_GET_PROTO_BY_NAME_ERROR)
		ft_dprintf(2, "getprotobyname() failure !");
	else if (error == FT_BIND_ERROR)
		ft_dprintf(2, "bind() failure !");
	else if (error == FT_LISTEN_ERROR)
		ft_dprintf(2, "listen() failure !");
	else if (error == FT_OPEN_ERROR)
		ft_dprintf(2, "open() failure !");
	else if (error == FT_CONNECT_ERROR)
		ft_dprintf(2, "connect() failure !");
	else if (error == FT_SEND_ERROR)
		ft_dprintf(2, "send() failure !");
	ft_printf(""END);
	ft_dprintf(2, RED"\nerrno : %s\n"END, ERRNO);
	exit(errno);
}

int	usage(char *argv, bool type)
{
	if (type == SERVER)
		ft_dprintf(2, "Usage : %s [port] (between 1024 and 65535)\n", argv);
	else if (type == CLIENT)
		ft_dprintf(2, "Usage : %s [addr] [port] (between 1024 and 65535)\n",
				argv);
	exit(EXIT_FAILURE);
}
