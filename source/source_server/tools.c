/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 06:27:02 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/24 06:29:28 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

char	*get_directory(char **split)
{
	char	*directory;

	if (!ft_strchr(split[1], '/'))
		return (NULL);
	directory = ft_strdup(split[1]);
	if ((directory = ft_str_erase_after_last(directory, '/')) == NULL)
	{
		ft_dprintf(2, RED"Failure : Malloc error"END);
		return (NULL);
	}
	ft_printf(YELLOW"%s\n"END, directory);
	return (directory);
}

void	exec_medium_cmd(int socket, char **split)
{
	char	buff[4096];

	if (check_right(split[ft_count_2d_tab(split) - 1], buff) != 1)
	{
		ft_strcat(buff, "\n");
		send(socket, buff, sizeof(buff), 0);
		send(socket, KEY, 64, 0);
		return ;
	}
	if (!ft_strcmp(split[0], "ls"))
		exec_ls(socket, split);
	send(socket, KEY, 64, 0);
}

void	exec_hard_cmd(int socket, char **split)
{
	if (!ft_strcmp(split[0], "get"))
		server_get(socket, split);
	else if (!ft_strcmp(split[0], "put"))
		server_put(socket, split);
	ft_printf(GREEN"Done\n"END);
}
