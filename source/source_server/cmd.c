/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 06:23:07 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/24 06:48:37 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

char		g_orig_dir[PATH_MAX];
uint8_t	g_auth;

int8_t	get_level_cmd(char *str)
{
	int8_t		incr;
	int8_t		level;
	const char	*cmd[] = {"cd", "pwd", "quit", "mkdir", "rmdir",
	"rm", "ls", "get", "put", NULL};

	incr = 0;
	level = 0;
	while (cmd[incr])
	{
		if (!ft_strcmp(cmd[incr], str))
		{
			if (incr <= 5)
				level = EASY;
			else if (incr == 6)
				level = MEDIUM;
			else
				level = HARD;
		}
		++incr;
	}
	return (level);
}

#define CUR_F "Get current dir failure."
#define INV_DIR "Failure : Invalid directory"
#define NEWFAIL "Get new dir failure."

int8_t	check_right(char *path, char *buff)
{
	char	cur_dir[PATH_MAX];
	char	new_dir[PATH_MAX];

	if ((path == NULL || g_auth == ROOT) ||
		(getcwd(cur_dir, PATH_MAX) == NULL && ft_strcpy(buff, CUR_F)) ||
		(chdir(path) == -1 && ft_strcpy(buff, INV_DIR)) ||
		(getcwd(new_dir, PATH_MAX) == NULL && ft_strcpy(buff, NEWFAIL)))
		return (1);
	if (ft_strncmp(g_orig_dir, new_dir, ft_strlen(g_orig_dir)))
	{
		ft_strcpy(buff, RED"Failure : Insufficient permissions"END);
		if (chdir(cur_dir))
			ft_strcpy(path, RED"Failure : Invalid directory"END);
		return (0);
	}
	if (ft_count_char(new_dir, '/') < ft_count_char(g_orig_dir, '/'))
	{
		ft_strcpy(buff, RED"Failure : Insufficient permissions"END);
		if (chdir(cur_dir))
			ft_strcpy(path, RED"Failure : Invalid directory"END);
		return (0);
	}
	if (chdir(cur_dir))
		ft_strcpy(path, RED"Failure : Invalid directory"END);
	return (1);
}

int8_t	mkdir_directory(char **split, char *buff)
{
	char	*directory;
	int8_t	ret;

	if ((directory = get_directory(split)) == NULL)
		ret = 1;
	else
		ret = check_right(directory, buff);
	ft_strdel(&directory);
	return (ret);
}

ssize_t	prepare_ls(int socket, char **split, struct stat *st)
{
	if (dup2(socket, STDOUT_FILENO) == -1 || dup2(socket, STDERR_FILENO) == -1)
		ft_error(FT_DUP2_ERROR);
	close(socket);
	execv("/bin/ls", split);
	fstat(socket, st);
	return (st->st_size);
}

void	exec_ls(int socket, char **split)
{
	int			child_pid;
	char		buff[BUFF_SIZE + 1];
	ssize_t		ret_send;
	ssize_t		size;
	struct stat	st;

	if ((child_pid = fork()) == -1)
		exit(errno);
	else if (child_pid == 0)
	{
		size = prepare_ls(socket, split, &st);
		while (size > 0)
		{
			ret_send = send(socket, buff, 4096, 0);
			if (ret_send == -1 && ft_dprintf(2, RED"Send error\n"END))
				break ;
			size -= ret_send;
		}
		exit(0);
	}
	else
		wait4(child_pid, 0, 0, 0);
}
