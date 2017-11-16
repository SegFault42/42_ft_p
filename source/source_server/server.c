/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/06 00:26:50 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/08 04:58:27 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

char	g_orig_dir[PATH_MAX];

/*
 ** bind() liason du socket au client
 ** listen() ecoute les conexion entrante
 */

int	create_server(uint16_t port)
{
	int					sock;
	/*uint32_t			client_socket_len;*/
	struct protoent		*proto;
	struct sockaddr_in	sin;

	if ((proto = getprotobyname("tcp")) == NULL)
		ft_error(FT_GET_PROTO_BY_NAME_ERROR);
	if ((sock = socket(PF_INET, SOCK_STREAM, proto->p_proto)) == -1)
		ft_error(FT_SOCKET_ERROR);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if ((bind(sock, (const struct sockaddr *)&sin, sizeof(sin))) == -1)
		ft_error(FT_BIND_ERROR);
	if ((listen(sock, 2)) == -1)
		ft_error(FT_LISTEN_ERROR);
	return (sock);
}

void	dup_server(int client_socket, struct sockaddr_in sin, uint32_t client_socket_len)
{
	int	child_pid;
	int	sock;

	while (true)
	{
		sock = accept(client_socket, (struct sockaddr *)&sin, &client_socket_len);
		child_pid = fork();
		if (child_pid == -1)
		{
			/*perror("can't fork");*/
			close(sock);
			continue ;
		}
		else if (child_pid > 0)
			continue ;
		else if (child_pid == 0)
		{
			ft_printf(PURPLE"New conexion\n"END);
			recv_from_client(sock);
			close(sock);
			break ;
		}
	}
}

static char	*get_directory(char **split)
{
	char	*directory;

	directory = ft_strdup(split[1]);
	if ((directory = ft_str_erase_after_last(directory, '/')) == NULL)
	{
		ft_dprintf(2, RED"Failure : Malloc error"END);
		return (NULL);
	}
	ft_printf(YELLOW"%s\n"END, directory);
	return (directory);
}

static int8_t	get_level_cmd(char *str)
{
	int8_t	incr;
	int8_t	level;
	const char	*cmd[] = {"cd", "pwd", "quit", "mkdir", "ls", "get", "put", NULL};

	incr = 0;
	level = 0;
	while (cmd[incr])
	{
		if (!ft_strcmp(cmd[incr], str))
		{
			if (incr <= 3)
				level = EASY;
			else if (incr == 4)
				level = MEDIUM;
			else
				level = HARD;
		}
		++incr;
	}
	return (level);
}

static int8_t	check_right(char *path, char *buff)
{
	char	cur_dir[PATH_MAX];
	char	new_dir[PATH_MAX];

	if (getcwd(cur_dir, PATH_MAX) == NULL)
	{
		ft_strcpy(buff, RED"Get current dir failure."END);
		return (-1);
	}
	if (chdir(path) == -1)
	{
		ft_strcpy(buff, RED"Failure : Invalid directory"END);
		return (-1);
	}
	if (getcwd(new_dir, PATH_MAX) == NULL)
	{
		ft_strcpy(buff, RED"Get new dir failure."END);
		return (-1);
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

static void	exec_pwd(int socket, char **split)
{
	char	buff[PATH_MAX];

	if (ft_count_2d_tab(split) != 1)
		ft_strcpy(buff, RED"Failure : Too many argument"END);
	else if (getcwd(buff, PATH_MAX) == NULL)
		ft_strcpy(buff, "getcwd Failure");
	send(socket, buff, ft_strlen(buff), 0);
}

static void	exec_cd(int socket, char **split)
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

static void	exec_mkdir(int socket, char **split)
{
	char	buff[PATH_MAX];
	int8_t	ret;
	char	*directory;

	if (ft_count_2d_tab(split) > 2)
		ft_strcpy(buff, RED"Failure : Too many argument"END);
	else if (ft_count_2d_tab(split) == 1)
		ft_strcpy(buff, RED"Failure : Too few argument"END);
	else
	{
		directory = get_directory(split);
		if (directory == NULL)
			ret = 1;
		else
			ret = check_right(directory, buff);
		ft_strdel(&directory);
		ft_printf("ret = %d\n", ret);
		if (ret == 1)
		{
			if (mkdir(split[1], 0744) == -1)
			{
				if (errno == EEXIST)
					ft_strcpy(buff, RED"Failure : Directory exist"END);
				else
					ft_strcpy(buff, RED"Failure : Creating directory"END);
			}
			else
				ft_strcpy(buff, GREEN"Directory created"END);
		}
	}
	send(socket, buff, ft_strlen(buff), 0);
}

static int8_t	exec_easy_cmd(int socket, char **split)
{
	if (!ft_strcmp(split[0], "pwd"))
		exec_pwd(socket, split);
	else if (!ft_strcmp(split[0], "cd"))
		exec_cd(socket, split);
	else if (!ft_strcmp(split[0], "mkdir"))
		exec_mkdir(socket, split);
	else if (!ft_strcmp(split[0], "quit"))
		return (QUIT);
	return (0);
}

static void	exec_ls(int socket, char **split)
{
	int		child_pid;
	char	buff[BUFF_SIZE + 1];
	ssize_t	ret_send;
	ssize_t	size;
	struct stat	st;

	child_pid = fork();
	if (child_pid ==1)
	{
		perror("Can't fork");
		exit(errno);
	}
	else if (child_pid == 0)
	{
		if (dup2(socket, STDOUT_FILENO) == -1)
			ft_error(FT_DUP2_ERROR);
		if (dup2(socket, STDERR_FILENO) == -1)
			ft_error(FT_DUP2_ERROR);
		close(socket);
		execv("/bin/ls", split);
		fstat(socket, &st);
		size = (ssize_t)st.st_size;
		while (1)
		{
			ret_send = send(socket, buff, 4096, 0);
			if (ret_send == -1)
			{
				ft_dprintf(2, RED"Send error\n"END);
				break ;
			}
			size -= ret_send;
			if (size == 0)
				break;
		}
		exit(0);
	}
	else
		wait4(child_pid, 0, 0, 0);
}

static void	exec_medium_cmd(int socket, char **split)
{
	if (!ft_strcmp(split[0], "ls"))
		exec_ls(socket, split);
	send(socket, KEY, 64, 0);
}

static int	check_file_exist(char *file, char *buff)
{
	int		fd;
	struct stat	st;

	if ((fd = open(file, O_RDONLY)) == -1)
	{
		ft_strcpy(buff, RED"Getting file error"END);
		return (-1);
	}
	if (fstat(fd, &st) == -1)
	{
		ft_strcpy(buff, RED"fstat error"END);
		return (-1);
	}
	if (!S_ISREG(st.st_mode))
	{
		ft_strcpy(buff, RED"Not a regular file"END);
		return (-1);
	}
	return (fd);
}

static int8_t	check_right_client(int socket)
{
	char	buffer[4096];

	recv(socket, buffer, sizeof(buffer), 0);
	if (!ft_strcmp(buffer, "ERROR"))
		return (false);
	else
		return (true);
}

static void	exec_get(int socket, char **split, int fd)
{
	char	buffer[4096];
	ssize_t	ret_read;
	ssize_t	ret_send = 0;

	if (check_right_client(socket) == false)
		return ;
	while ((ret_read = read(fd, buffer, sizeof(buffer))) > 0)
	{
		ret_send = send(socket, buffer, (size_t)ret_read, 0);
		/*ft_printf(ORANGE"{%d}"END, ret_read);*/
		/*write(1, &buffer, ret_read);*/
	}
	ft_printf(ORANGE"ret_send = %d\n"END, ret_send);
	if (ret_send == 4096)
	{
		ft_memset(buffer, 0, sizeof(buffer));
		ft_strcpy(buffer, KEY);
		ret_read = send(socket, buffer, 64, 0);
	}
	/*ft_printf(ORANGE"ret_send = %d\n"END, ret_read);*/
	ft_printf(GREEN"Transfert success\n"END);
}


static void	exec_hard_cmd(int socket, char **split)
{
	char	buffer[4096];
	int		fd;

	if (check_right(split[1], buffer) == 0 ||
		(fd = check_file_exist(split[1], buffer)) == -1)
	{
		send(socket, buffer, 4096, 0);
		return;
	}
	else
	{
		ft_strcpy(buffer, "SUCCESS");
		send(socket, buffer, sizeof(buffer), 0);
		if (!ft_strcmp(split[0], "get"))
			exec_get(socket, split, fd);
		close(fd);
	}
}

void	recv_from_client(int socket)
{
	char	complete_cmd[MAX_CMD_LEN + 1];
	ssize_t	ret_recv;
	int8_t	level_cmd;
	char	**split;


	ft_printf("path = %s\n", g_orig_dir);
	while (true)
	{
		if ((ret_recv = recv(socket, complete_cmd, MAX_CMD_LEN, 0)) == -1)
			ft_printf("recv failure\n");
		complete_cmd[ret_recv] = 0;
		split = ft_strsplit_blank(complete_cmd);
		level_cmd = get_level_cmd(split[0]);
		ft_printf(CYAN"%s receveid\n"END, complete_cmd);
		if (level_cmd == EASY)
		{
			if (exec_easy_cmd(socket, split) == QUIT)
			{
				ft_2d_tab_free(split);
				break ;
			}
		}
		else if (level_cmd == MEDIUM)
			exec_medium_cmd(socket, split);
		else if (level_cmd == HARD)
			exec_hard_cmd(socket, split);
		ft_2d_tab_free(split);
	}
}
