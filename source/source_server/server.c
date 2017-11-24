/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/06 00:26:50 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/24 06:45:55 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

extern char		*g_ft_errno[];
extern uint8_t	g_auth;

/*
** bind() liason du socket au client
** listen() ecoute les conexion entrante
*/

int			create_server(uint16_t port)
{
	int					sock;
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

void		dup_server(int client_socket, struct sockaddr_in sin, uint32_t csl)
{
	int	child_pid;
	int	sock;

	while (true)
	{
		sock = accept(client_socket, (struct sockaddr *)&sin,
		&csl);
		child_pid = fork();
		if (child_pid == -1)
		{
			ft_dprintf(2, "%s\n", ERRNO);
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

static void	auth_server(int socket)
{
	char	complete_cmd[4096];

	ft_memset(complete_cmd, 0, sizeof(complete_cmd));
	recv(socket, complete_cmd, sizeof(complete_cmd), 0);
	if (!ft_strcmp(complete_cmd, "root"))
		g_auth = ROOT;
}

void		recv_from_client(int socket)
{
	char	complete_cmd[MAX_CMD_LEN + 1];
	ssize_t	ret_recv;
	int8_t	level_cmd;
	char	**split;

	ft_memset(complete_cmd, 0, sizeof(complete_cmd));
	auth_server(socket);
	while (true)
	{
		if ((ret_recv = recv(socket, complete_cmd, MAX_CMD_LEN, 0)) == -1)
			ft_printf("recv failure\n");
		complete_cmd[ret_recv] = 0;
		split = ft_strsplit_blank(complete_cmd);
		level_cmd = get_level_cmd(split[0]);
		ft_printf(CYAN"%s receveid\n"END, complete_cmd);
		if (level_cmd == EASY)
			if (exec_easy_cmd(socket, split) == QUIT && !ft_2d_tab_free(split))
				break ;
		if (level_cmd == MEDIUM)
			exec_medium_cmd(socket, split);
		if (level_cmd == HARD)
			exec_hard_cmd(socket, split);
		ft_2d_tab_free(split);
	}
}
