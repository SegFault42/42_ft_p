/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 04:40:41 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/24 06:49:35 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

char		g_orig_dir[PATH_MAX];

void	exec_lls(char **split, uint8_t flag)
{
	int		child_pid;

	child_pid = fork();
	if (child_pid == 1)
		exit(errno);
	else if (child_pid == 0)
	{
		if (flag == 1)
			execv("/bin/ls", split);
		else if (flag == 2)
			execv("/bin/pwd", split);
		exit(0);
	}
	else
		wait4(child_pid, 0, 0, 0);
}

void	exec_lcd(char **split)
{
	if (ft_count_2d_tab(split) > 2)
		ft_printf(RED"Failure : Too many argument\n"END);
	else if (ft_count_2d_tab(split) == 1)
	{
		if (chdir(g_orig_dir) == -1)
			ft_printf(RED"Failure : Changing directory\n"END);
		else
			ft_printf(GREEN"Directory changed\n"END);
	}
	else
	{
		if (chdir(split[1]) == -1)
			ft_printf(RED"Failure : Changing directory\n"END);
		else
			ft_printf(GREEN"Directory changed\n"END);
	}
}

int8_t	local_cmd(char **split)
{
	if (!ft_strcmp(split[0], "lls"))
		exec_lls(split, 1);
	else if (!ft_strcmp(split[0], "lpwd"))
	{
		if (ft_count_2d_tab(split) == 1)
			exec_lls(split, 2);
		else
			ft_dprintf(2, RED"Too many arguments\n"END);
	}
	else if (!ft_strcmp(split[0], "lcd"))
		exec_lcd(split);
	return (0);
}

int8_t	cmd_exist(char **split)
{
	int8_t		incr;
	int8_t		level;
	const char	*cmd[] = {"cd", "pwd", "quit", "mkdir", "rmdir", "rm", "ls",
	"get", "put", "lls", "lpwd", "lcd", NULL};

	incr = 0;
	level = false;
	while (cmd[incr])
	{
		if (!ft_strcmp(cmd[incr], split[0]))
		{
			if (incr <= 5)
				level = EASY;
			else if (incr == 6)
				level = MEDIUM;
			else if (incr <= 8)
				level = HARD;
			else
				level = LOCAL;
		}
		++incr;
	}
	return (level);
}
