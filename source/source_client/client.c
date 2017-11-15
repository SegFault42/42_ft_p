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

static int8_t	get_cmd(int socket, char *comp_cmd, char **split)
{
	ssize_t	ret_send;
	ssize_t	ret_recv;
	char	buff[4096];
	int		fd;

		fd = open(split[1], O_RDWR | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			ft_dprintf(2, "Open error\n");
		while (1)
		{
			if ((ret_recv = recv(socket, buff, 4096, 0)) == -1)
				ft_dprintf(2, RED"Recv Failure\n"END);
			if (ret_recv != 4096 || !ft_strcmp(KEY, buff))
			{
				/*buff[ret_recv - 64] = 0;*/
				if (ft_memcmp(KEY, buff, 64) != 0)
					write(fd, &buff, (size_t)ret_recv);
				else
					write(1, &buff, 4096);
				break ;
			}
			write(fd, &buff, (size_t)ret_recv);
			ft_memset(buff, 0, 4096);
		}
		RC;
	return (0);
}

static int8_t	hard_cmd(int socket, char *comp_cmd, char **split)
{
	char	buffer[4096];
	ssize_t	ret_send;

	ret_send = send(socket, comp_cmd, ft_strlen(comp_cmd), 0);
	if (ret_send == -1)
	{
		ft_printf(RED"Send %s failure\n"END);
		return (-1);
	}
	recv(socket, buffer, sizeof(buffer), 0);
	if (!ft_strcmp(buffer, "SUCCESS"))
	{
		if (!ft_strcmp(split[1], "get"))
			get_cmd(socket, comp_cmd, split);
	}
	else
		ft_printf("%s\n", buffer);
}

static int8_t	cmd_exist(char **split)
{
	int8_t	incr;
	const char	*cmd[] = {"cd", "pwd", "quit", "mkdir", "ls", "get", "put", NULL};
	int8_t	level;

	incr = 0;
	level = false;
	while (cmd[incr])
	{
		if (!ft_strcmp(cmd[incr], split[0]))
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
