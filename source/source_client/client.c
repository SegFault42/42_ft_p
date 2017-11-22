/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/18 08:10:41 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/22 04:46:57 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

extern char		*g_ft_errno[];

static int8_t	easy_cmd(int socket, char *comp_cmd, char **split)
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

static int8_t	medium_cmd(int socket, char *comp_cmd)
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

static void	get_cmd(int socket, char **split)
{
	char	buffer[BUFFER_SIZE];
	int		fd;
	ssize_t	ret_recv;
	long	size;

	if ((fd = check_right_writing(socket, split[1])) == -1)
	{
		ft_printf(RED"%s\n"END, ERRNO);
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
		ft_printf(RED"%s\n"END, ERRNO);
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

static  uint8_t	authentification()
{
	char			buff[21];
	uint8_t			attempt;
	struct termios	oldt;
	struct termios	newt;

	attempt = 1;
	ft_printf("Username : ");
	read(STDIN_FILENO, buff, sizeof(buff));
	if (!strncmp("root", buff, 4))
	{
		while (attempt <= 3)
		{
			ft_printf("pass : ");
			tcgetattr( STDIN_FILENO, &oldt);
			newt = oldt;
			newt.c_lflag &= ~(ECHO);
			tcsetattr( STDIN_FILENO, TCSANOW, &newt);
			read(STDIN_FILENO, buff, sizeof(buff));
			tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
			if (!ft_strncmp("toor", buff, 4))
			{
				ft_printf(GREEN"\nWelcome root !\n"END);
				return (ROOT);
			}
			++attempt;
			ft_printf("\n");
			sleep(2);
			ft_printf(RED"Wrong password\n"END);
		}
		ft_printf(RED"Login failure\n%slogged as anonymous\nWelcome anonymous\n"END, GREEN);
	}
	else if (!ft_strncmp(buff, "anonymous", 9))
		ft_printf(GREEN"Welcome anonymous !\n"END);
	else
		ft_printf(GREEN"login incorrect, logged as anonymous\n"END);
	return (ANONYMOUS);
}

void	send_to_server(int socket)
{
	ssize_t	read_ret;
	char	**split;
	char	buff[MAX_CMD_LEN + 1];
	int8_t	level;
	uint8_t	auth;

	auth = authentification();
	auth == ROOT ? send(socket, "root", 4, 0) : send(socket, "anonymous", 9, 0);
	while (1)
	{
		auth == ROOT ? ft_printf(RED"🖥  root:ftp> "END) : ft_printf(GREEN"💻  ftp> "END);
		if ((read_ret = read(STDIN_FILENO, buff, MAX_CMD_LEN)) == 0)
			break ;
		ft_printf(ORANGE"read_ret = %d, cmd = %s\n"END, read_ret, buff);
		if (read_ret == -1)
			ft_printf(RED"%s"END, ERRNO);
		buff[read_ret] = '\0';
		if (buff[read_ret -1] == '\n')
			buff[read_ret -1] = '\0';
		if (read_ret == 1 || !(split = ft_strsplit_blank(buff)))
			continue ;
		if ((level = cmd_exist(split)) == EASY)
		{
			ft_printf(ORANGE"EASY\n"END);
			if (easy_cmd(socket, buff, split) == QUIT)
			{
				ft_2d_tab_free(split);
				return ;
			}
		}
		else if (level == MEDIUM)
		{
			ft_printf(ORANGE"MEDIUM\n"END);
			medium_cmd(socket, buff);
		}
		else if (level == HARD)
		{
			ft_printf(ORANGE"HARD\n"END);
			hard_cmd(socket, buff, split);
		}
		else
			ft_dprintf(2, RED"Unknow command !\n"END);
		ft_2d_tab_free(split);
		ft_memset(buff, 0, sizeof(buff));
	}
	easy_cmd(socket, "quit", NULL);
}
