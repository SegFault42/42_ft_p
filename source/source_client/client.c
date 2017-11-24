/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/18 08:10:41 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/24 06:50:12 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

extern char		*g_ft_errno[];
static uint8_t	g_auth = 0;

void			progress_bar(long int end, long int current)
{
	size_t	x;

	x = L_C(current, 0, end, 0, 99);
	ft_putstr(PURPLE"[ ");
	ft_putnstr(PURPLE"─", x);
	ft_putnstr(PURPLE"🐌", 1);
	ft_putnstr(GREY"─", 99 - x);
	ft_putstr(PURPLE" ]"END);
}

#define OLD term[0]
#define NEW term[1]

static uint8_t	authentification_pass(void)
{
	char			bu[4096];
	uint8_t			attempt;
	struct termios	term[2];

	attempt = 1;
	while (attempt <= 3)
	{
		ft_memset(bu, 0, sizeof(bu));
		ft_printf("pass : ");
		tcgetattr(STDIN_FILENO, &OLD);
		NEW = OLD;
		NEW.c_lflag &= (unsigned long)~(ECHO);
		tcsetattr(STDIN_FILENO, TCSANOW, &NEW);
		read(STDIN_FILENO, bu, sizeof(bu));
		tcsetattr(STDIN_FILENO, TCSANOW, &OLD);
		if (!ft_strcmp("toor\n", bu) && ft_printf(GREEN"\nWelcome root !\n"END))
			return (ROOT);
		++attempt;
		ft_printf("\n");
		sleep(2);
		ft_printf(RED"Wrong password\n"END);
	}
	return (ANONYMOUS);
}

static uint8_t	authentification(void)
{
	char	buff[4096];

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

static int8_t	choose_cmd(int socket, char **split, char *buff)
{
	int8_t	level;

	if ((level = cmd_exist(split)) == EASY)
	{
		if (easy_cmd(socket, buff, split) == QUIT)
		{
			ft_2d_tab_free(split);
			return (false);
		}
	}
	else if (level == MEDIUM)
		medium_cmd(socket, buff);
	else if (level == HARD)
		hard_cmd(socket, buff, split);
	else if (level == LOCAL)
		local_cmd(split);
	else
		ft_dprintf(2, RED"Unknow command !\n"END);
	return (true);
}

#define PF ft_printf
#define SE send

void			send_to_server(int socket)
{
	ssize_t	read_ret;
	char	**split;
	char	buff[MAX_CMD_LEN + 1];

	g_auth = authentification();
	g_auth == ROOT ? SE(socket, "root", 4, 0) : SE(socket, "anonymous", 9, 0);
	while (1)
	{
		g_auth == ROOT ? PF(RED"🖥  root:ftp> "END) : PF(GREEN"💻  ftp> "END);
		if ((read_ret = read(STDIN_FILENO, buff, MAX_CMD_LEN)) == 0)
			break ;
		if (read_ret == -1)
			ft_printf(RED"%s"END, ERRNO);
		buff[read_ret] = '\0';
		if (buff[read_ret - 1] == '\n')
			buff[read_ret - 1] = '\0';
		if (read_ret == 1 || !(split = ft_strsplit_blank(buff)))
			continue ;
		if (choose_cmd(socket, split, buff) == false)
			return ;
		ft_2d_tab_free(split);
		ft_memset(buff, 0, sizeof(buff));
	}
	easy_cmd(socket, "quit", NULL);
}
