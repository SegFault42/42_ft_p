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

static int	check_right_writing(int socket, char *split)
{
	int		fd;
	char	buffer[4096];

	fd = open(extract_name_from_path(split), O_RDWR | O_CREAT | O_TRUNC, 0644);
	ft_printf(ORANGE"name = %s\n"END, extract_name_from_path(split));
	if (fd == -1)
		ft_strcpy(buffer, "ERROR");
	else
		ft_strcpy(buffer, "SUCCESS");
	ft_printf(ORANGE"%s\n"END, buffer);
	send(socket, buffer, sizeof(buffer), 0);
	return (fd);
}

#define NB_BAR	size[1]
#define START	time_[0]
#define STOP	time_[1]

static void	progress_bar(struct timeval start, struct timeval stop, ssize_t ret_recv, long size_total)
{
	time_t	timee;
	time_t	utime;
	long double	udelta;
	long double add_char = size_total / 99;
	static int x =0;

	timee = stop.tv_sec - start.tv_sec;
	utime = stop.tv_usec - start.tv_usec;
	/*if (!last || stop.tv_sec - last > 0)*/
	/*{*/
		udelta = ((float)utime / (float)1000000) + timee;
		/*last = stop.tv_sec;*/
	/*}*/

	/*s += offset; //keep save*/

	while (ret_recv >= add_char)
	{
		ret_recv -= add_char;
		x++;
	}

	if (ret_recv > BUFFER_SIZE)
		write(1, "\033[1A", 4);

	write(1, "[", 1);
	print_n_char('X', x);
	print_n_char(' ', 99 - x);
	write(1, "]", 1);
	printf(" %.3Lf ko/s il reste %i second\r\n", BUFFER_SIZE / udelta / 1000, (int) ((size_total - j) / (BUFFER_SIZE / udelta)));

	t++;

}


static void	get_cmd(int socket, char **split)
{
	char	buffer[BUFFER_SIZE];
	int		fd;
	ssize_t	ret_recv;
	long	size[2];
	struct timeval	time_[2];

	if ((fd = check_right_writing(socket, split[1])) == -1)
		return ;
	recv(socket, buffer, sizeof(buffer), 0);
	size[0] = ft_atol(buffer);
	while (size[0] > 0)
	{
		gettimeofday(&START, NULL);
		ret_recv = recv(socket, buffer, sizeof(buffer), 0);
		NB_BAR += ret_recv;
		gettimeofday(&STOP, NULL);
		NB_BAR = progress_bar(START, STOP, NB_BAR, size[0]);
		write(fd, &buffer, (size_t)ret_recv);
		ft_memset(buffer, 0, sizeof(buffer));
		size[0] -= ret_recv;
	}
	RC;
	ft_printf(GREEN"Transfert success\n"END);
}

static int8_t	hard_cmd(int socket, char *comp_cmd, char **split)
{
	char	buffer[4096];
	ssize_t	ret_send;

	if ((ret_send = send(socket, comp_cmd, ft_strlen(comp_cmd), 0)) == -1)
	{
		ft_printf(RED"Send %s failure\n"END);
		return (-1);
	}

	recv(socket, buffer, sizeof(buffer), 0);

	if (!ft_strcmp(buffer, "SUCCESS"))
	{ //  HEre verfi for file ok
		if (!ft_strcmp(split[0], "get"))
			get_cmd(socket, split);
	}
	else
		ft_printf("%s\n", buffer);
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
			medium_cmd(socket, buff);
		else if (level == HARD)
			hard_cmd(socket, buff, split);
		else
			ft_dprintf(2, RED"Unknow command !\n"END);
		ft_2d_tab_free(split);
	}
	easy_cmd(socket, "quit", NULL);
}
