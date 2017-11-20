/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/18 08:10:41 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/20 08:58:04 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

static int8_t	easy_cmd(int socket, char *comp_cmd, char **split)
{
	ssize_t	ret_send;
	ssize_t	ret_recv;
	char	buff[4096 + 1];

	ret_send = send(socket, comp_cmd, ft_strlen(comp_cmd), 0);
	if (ret_send == -1)
		ft_printf(RED"Send %s failure\n"END);
	if (split == NULL || !ft_strcmp(split[0], "quit"))
		return (QUIT);
	ret_recv = recv(socket, buff, 4096, 0);
	buff[ret_recv] = 0;
	ft_printf("%s\n", buff);
	return (0);
}

static int8_t	medium_cmd(int socket, char *comp_cmd)
{
	ssize_t	ret_send;
	ssize_t	ret_recv;
	char	buff[4096 + 1];

	ret_send = send(socket, comp_cmd, ft_strlen(comp_cmd), 0);
	if (ret_send == -1)
		ft_printf(RED"Send %s failure\n"END);
	else
	{
		while (1)
		{
			if ((ret_recv = recv(socket, buff, 4096, 0)) == -1)
				ft_dprintf(2, RED"Recv Failure\n"END);
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

static void	get_cmd(int socket, char **split)
{
	char	buffer[BUFFER_SIZE];
	int		fd;
	ssize_t	ret_recv;
	long	size;

	if ((fd = check_right_writing(socket, split[1])) == -1)
	{
		ft_printf(RED"Permission denied\n"END);
		return ;
	}
	recv(socket, buffer, sizeof(buffer), 0);
	size = ft_atol(buffer);
	while (size > 0)
	{
		ret_recv = recv(socket, buffer, sizeof(buffer), 0);
		write(fd, &buffer, (size_t)ret_recv);
		ft_memset(buffer, 0, sizeof(buffer));
		size -= ret_recv;
	}
	RC;
	ft_printf(GREEN"Transfert success\n"END);
}

static int8_t		client_get(int socket, char *comp_cmd, char **split)
{
	char	buffer[BUFFER_SIZE];
	ssize_t	ret_send;
	int		fd;
	char	*file;

	file = extract_name_from_path(split[1]);
	if ((fd = open(file, O_RDONLY)) != -1)
	{
		ft_printf(RED"File exist\n"END);
		close(fd);
		return (-1);
	}
	if ((ret_send = send(socket, comp_cmd, ft_strlen(comp_cmd), 0)) == -1)
	{
		ft_printf(RED"Send %s failure\n"END);
		return (-1);
	}
	recv(socket, buffer, sizeof(buffer), 0);
	if (!ft_strcmp(buffer, "SUCCESS"))
	{ //  HEre verfi for file ok
		get_cmd(socket, split);
	}
	else
		ft_printf("%s\n", buffer);
	return (0);
}

static void			exec_put(int socket, int fd)
{
	char		buffer[BUFFER_SIZE];
	struct stat	st;
	long		size;
	ssize_t		ret_read;

	fstat(fd, &st);
	size = st.st_size;
	send_file_size(socket, size);
	while ((ret_read = read(fd, buffer, sizeof(buffer))) > 0)
		send(socket, buffer, (size_t)ret_read, 0);
	ft_printf(GREEN"Transfert success\n"END);
}

static int8_t		client_put(int socket, char *comp_cmd, char **split)
{
	int		fd;
	char	buffer[BUFFER_SIZE];

	ft_printf("put\n");

	// 1st check if file exist and if i can open as read.
	if ((fd = check_file_exist(split[1], buffer)) == -1)
	{
		ft_printf("%s\n", buffer);
		return (-1);
	}
	send(socket, comp_cmd, ft_strlen(comp_cmd), 0);

	recv(socket, buffer, BUFFER_SIZE, 0);
	if (!ft_strcmp(buffer, "ERROR"))
	{
		ft_dprintf(2, RED"Unable to write file in server side\n"END);
		return (-1);
	}
	exec_put(socket, fd);
	close(fd);

	return (0);
}

static int8_t	hard_cmd(int socket, char *comp_cmd, char **split)
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

static int8_t	cmd_exist(char **split)
{
	int8_t	incr;
	const char	*cmd[] = {"cd", "pwd", "quit", "mkdir", "rmdir", "rm", "ls", "get", "put", NULL};
	int8_t	level;

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
			else
				level = HARD;
		}
		++incr;
	}
	return (level);
}

void	send_to_server(int socket)
{
	ssize_t	read_ret;
	char	**split;
	char	buff[MAX_CMD_LEN + 1];
	int8_t	level;

	while (ft_printf(YELLOW"ftp> "END) && (read_ret = read(STDIN_FILENO, &buff, MAX_CMD_LEN)) > 0)
	{
		buff[read_ret] = '\0';
		if (buff[read_ret -1] == '\n')
			buff[read_ret -1] = '\0';
		if (read_ret == 1 || !(split = ft_strsplit_blank(buff)))
			continue ;
		if ((level = cmd_exist(split)) == EASY)
		{
			if (easy_cmd(socket, buff, split) == QUIT)
			{
				ft_2d_tab_free(split);
				return ;
			}
		}
		else if (level == MEDIUM)
			medium_cmd(socket, buff);
		else if (level == HARD)
			hard_cmd(socket, buff, split);
		else
			ft_dprintf(2, RED"Unknow command !\n"END);
		ft_2d_tab_free(split);
	}
	easy_cmd(socket, "quit", NULL);
}
