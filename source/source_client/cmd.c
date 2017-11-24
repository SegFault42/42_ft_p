/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 04:30:52 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/24 04:52:37 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

extern char	*g_ft_errno[];

int8_t	easy_cmd(int socket, char *comp_cmd, char **split)
{
	ssize_t	ret_send;
	ssize_t	ret_recv;
	char	buff[4096 + 1];

	ret_send = send(socket, comp_cmd, ft_strlen(comp_cmd), 0);
	if (ret_send == -1)
		ft_printf(RED"%s\n"END, ERRNO);
	if (split == NULL || !ft_strcmp(split[0], "quit"))
		return (QUIT);
	ret_recv = recv(socket, buff, 4096, 0);
	buff[ret_recv] = 0;
	ft_printf("%s\n", buff);
	return (0);
}

int8_t	medium_cmd(int socket, char *comp_cmd)
{
	ssize_t	ret_send;
	ssize_t	ret_recv;
	char	buff[4096 + 1];

	ret_send = send(socket, comp_cmd, ft_strlen(comp_cmd), 0);
	if (ret_send == -1)
		ft_printf(RED"%s\n"END, ERRNO);
	else
	{
		while (1)
		{
			if ((ret_recv = recv(socket, buff, 4096, 0)) == -1)
				ft_printf(RED"Send %s failure\n"END);
			buff[ret_recv] = 0;
			if (ft_strstr(buff, KEY))
			{
				buff[ret_recv - 64] = 0;
				ft_printf("%s", buff);
				break ;
			}
			ft_printf("%s", buff);
		}
	}
	return (0);
}

int8_t	hard_cmd(int socket, char *comp_cmd, char **split)
{
	if (ft_count_2d_tab(split) > 2)
	{
		ft_dprintf(2, RED"Failure : Too many argument\n"END);
		return (-1);
	}
	else if (ft_count_2d_tab(split) == 1)
	{
		ft_dprintf(2, RED"Failure : Too few argument\n"END);
		return (-1);
	}
	if (!ft_strcmp(split[0], "get"))
		return (client_get(socket, comp_cmd, split));
	else if (!ft_strcmp(split[0], "put"))
		return (client_put(socket, comp_cmd, split));
	return (0);
}
