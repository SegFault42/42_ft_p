/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 06:20:33 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/24 06:48:48 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

char		g_orig_dir[PATH_MAX];

void		exec_pwd(int socket, char **split)
{
	char	buff[PATH_MAX];

	if (ft_count_2d_tab(split) != 1)
		ft_strcpy(buff, RED"Failure : Too many argument"END);
	else if (getcwd(buff, PATH_MAX) == NULL)
		ft_strcpy(buff, "getcwd Failure");
	send(socket, buff, ft_strlen(buff), 0);
}

void		exec_cd(int socket, char **split)
{
	char	buff[PATH_MAX];
	int8_t	ret;

	if (ft_count_2d_tab(split) > 2)
		ft_strcpy(buff, RED"Failure : Too many argument"END);
	else if (ft_count_2d_tab(split) == 1)
	{
		if (chdir(g_orig_dir) == -1)
			ft_strcpy(buff, RED"Failure : Changing directory"END);
		else
			ft_strcpy(buff, GREEN"Directory changed"END);
	}
	else if ((ret = check_right(split[1], buff)) == true)
	{
		if (chdir(split[1]) == -1)
			ft_strcpy(buff, RED"Failure : Changing directory"END);
		else
			ft_strcpy(buff, GREEN"Directory changed"END);
	}
	else if (ret == false)
		ft_strcpy(buff, RED"Failure : Insufficient permissions"END);
	send(socket, buff, ft_strlen(buff), 0);
}

void		exec_mkdir(int socket, char **split)
{
	char	buff[PATH_MAX];
	int8_t	ret;

	if (ft_count_2d_tab(split) != 2)
		ft_strcpy(buff, RED"Failure : Too many/few argument"END);
	else
	{
		ret = mkdir_directory(split, buff);
		if (ret == 1)
		{
			if (mkdir(split[1], 0744) == -1)
			{
				errno == EEXIST ?
					ft_strcpy(buff, RED"Failure : Directory exist"END) :
					ft_strcpy(buff, RED"Failure : Creating directory"END);
			}
			else
				ft_strcpy(buff, GREEN"Directory created"END);
		}
	}
	send(socket, buff, ft_strlen(buff), 0);
}

void		exec_rmdir(int socket, char **split, uint8_t flag)
{
	char	buff[BUFFER_SIZE];

	if (ft_count_2d_tab(split) > 2)
		ft_strcpy(buff, RED"Failure : Too many argument"END);
	else if (ft_count_2d_tab(split) == 1)
		ft_strcpy(buff, RED"Failure : Too few argument"END);
	else
	{
		if (flag == RMDIR)
		{
			if (rmdir(split[1]) == -1)
				ft_strcpy(buff, RED"Directory not exist"END);
			else
				ft_strcpy(buff, GREEN"Directory removed"END);
		}
		else
		{
			if (unlink(split[1]) == -1)
				ft_strcpy(buff, RED"File not exist"END);
			else
				ft_strcpy(buff, GREEN"File removed"END);
		}
	}
	send(socket, buff, ft_strlen(buff), 0);
}

int8_t		exec_easy_cmd(int socket, char **split)
{
	if (!ft_strcmp(split[0], "pwd"))
		exec_pwd(socket, split);
	else if (!ft_strcmp(split[0], "cd"))
		exec_cd(socket, split);
	else if (!ft_strcmp(split[0], "mkdir"))
		exec_mkdir(socket, split);
	else if (!ft_strcmp(split[0], "rmdir"))
		exec_rmdir(socket, split, RMDIR);
	else if (!ft_strcmp(split[0], "rm"))
		exec_rmdir(socket, split, RM);
	else if (!ft_strcmp(split[0], "quit"))
		return (QUIT);
	return (0);
}
