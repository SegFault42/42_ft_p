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

extern char	g_orig_dir[PATH_MAX];

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

static void	exec_pwd(int socket)
{
	char	buff[PATH_MAX];

	if (getcwd(buff, PATH_MAX) == NULL)
		ft_strcpy(buff, "getcwd Failure");
	send(socket, buff, ft_strlen(buff), 0);
}

static int8_t	exec_easy_cmd(int socket, char **split)
{
	if (!ft_strcmp(split[0], "pwd"))
		exec_pwd(socket);
	else if (!ft_strcmp(split[0], "quit"))
		return (QUIT);
	return (0);
}

void	recv_from_client(int socket)
{
	char	complete_cmd[MAX_CMD_LEN + 1];
	ssize_t	ret_recv;
	int8_t	level_cmd;
	char	**split;

	while (true)
	{
		if ((ret_recv = recv(socket, complete_cmd, MAX_CMD_LEN, 0)) == -1)
			ft_printf("recv failure\n");
		complete_cmd[ret_recv] = 0;
		split = ft_strsplit_blank(complete_cmd);
		level_cmd = get_level_cmd(split[0]);
		if (level_cmd == EASY)
			if (exec_easy_cmd(socket, split) == QUIT)
				break ;
	}
}
