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
		{
			ft_printf("MEDIUM\n");
		}
		else if (level == HARD)
		{
			ft_printf("HARD\n");
		}
		else
			ft_dprintf(2, RED"Unknow command !\n"END);
		ft_2d_tab_free(split);
	}
	easy_cmd(socket, "quit", NULL);
}
