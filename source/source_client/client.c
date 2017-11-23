/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/18 08:10:41 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/23 05:06:05 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

extern char	*g_ft_errno[];
char		g_orig_dir[PATH_MAX];

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

static void	progress_bar(long int end, long int current)
{
	size_t	x;

	x = L_C(current, 0, end, 0, 99);
	ft_putstr(PURPLE"[ ");
	ft_putnstr(PURPLE"─", x);
	ft_putnstr(PURPLE"🐌", 1);
	ft_putnstr(GREY"─", 99 - x);
	ft_putstr(PURPLE" ]"END);
}

static void	get_cmd(int socket, char **split)
{
	char		buffer[BUFFER_SIZE];
	int			fd;
	ssize_t		ret_recv;
	long		size[2];
	long double	size_recv;

	size_recv = 0;
	if ((fd = check_right_writing(socket, split[1])) == -1 &&
	ft_printf(RED"%s\n"END, ERRNO))
		return ;
	recv(socket, buffer, sizeof(buffer), 0);
	size[0] = ft_atol(buffer);
	size[1] = size[0];
	ft_printf("\033[?25l");
	while (size[0] > 0)
	{
		ret_recv = recv(socket, buffer, sizeof(buffer), 0);
		write(fd, &buffer, (size_t)ret_recv);
		ft_memset(buffer, 0, sizeof(buffer));
		size[0] -= ret_recv;
		size_recv += ret_recv;
		progress_bar(size[1], (long)size_recv);
		ft_printf("\r");
	}
	ft_printf("\n\033[?25h%sTransfert success\n%s", GREEN, END);
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
		get_cmd(socket, split);
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
	long		total_read;

	fstat(fd, &st);
	total_read = 0;
	size = st.st_size;
	send_file_size(socket, size);
	ft_printf("\033[?25l");
	while ((ret_read = read(fd, buffer, sizeof(buffer))) > 0)
	{
		send(socket, buffer, (size_t)ret_read, 0);
		total_read += ret_read;
		progress_bar(size, (long)total_read);
		ft_printf("\r");
	}
	ft_printf("\033[?25h");
	ft_printf(GREEN"\nTransfert success\n"END);
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

static void	exec_lls(char **split, uint8_t flag)
{
	int		child_pid;
	char	buff[BUFF_SIZE + 1];

	child_pid = fork();
	if (child_pid ==1)
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

static void	exec_lcd(char **split)
{
	int8_t	ret;

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

static int8_t	local_cmd(char **split)
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

static int8_t	cmd_exist(char **split)
{
	int8_t	incr;
	const char	*cmd[] = {"cd", "pwd", "quit", "mkdir", "rmdir", "rm", "ls",
	"get", "put", "lls", "lpwd", "lcd", NULL};
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
			else if (incr <= 8)
				level = HARD;
			else
				level = LOCAL;
		}
		++incr;
	}
	return (level);
}

# define OLD term[0]
# define NEW term[1]

static  uint8_t	authentification_pass()
{
	char	buff[4096];
	uint8_t	attempt;
	struct termios	term[2];

	attempt = 1;
	while (attempt <= 3)
	{
		ft_memset(buff, 0, sizeof(buff));
		ft_printf("pass : ");
		tcgetattr(STDIN_FILENO, &OLD);
		NEW = OLD;
		NEW.c_lflag &= ~(ECHO);
		tcsetattr( STDIN_FILENO, TCSANOW, &NEW);
		read(STDIN_FILENO, buff, sizeof(buff));
		tcsetattr( STDIN_FILENO, TCSANOW, &OLD);
		if (!ft_strcmp("toor\n", buff) &&
		ft_printf(GREEN"\nWelcome root !\n"END))
			return (ROOT);
		++attempt;
		ft_printf("\n");
		sleep(2);
		ft_printf(RED"Wrong password\n"END);
	}
	return (ANONYMOUS);
}

static  uint8_t	authentification()
{
	char			buff[4096];

	ft_memset(buff, 0, sizeof(buff));
	ft_printf("Username : ");
	read(STDIN_FILENO, buff, sizeof(buff));
	if (!ft_strcmp("root\n", buff))
	{
		if (authentification_pass() == ROOT)
			return (ROOT);
		ft_printf(RED"Login failure\n%slogged as anonymous\n\
		Welcome anonymous\n"END, GREEN);
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
	ft_printf("%d\n", auth);
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
		else if (level == LOCAL)
			local_cmd(split);
		else
			ft_dprintf(2, RED"Unknow command !\n"END);
		ft_2d_tab_free(split);
		ft_memset(buff, 0, sizeof(buff));
	}
	easy_cmd(socket, "quit", NULL);
}
