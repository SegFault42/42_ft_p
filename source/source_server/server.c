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

/*
** bind() liason du socket au client
** listen() ecoute les conexion entrante
*/

int	create_server(uint16_t port)
{
	int					sock;
	char				addr[INET_ADDRSTRLEN];
	uint32_t			client_socket_len;
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
	if ((listen(sock, 42)) == -1)
		ft_error(FT_LISTEN_ERROR);
	sock = accept(sock, (struct sockaddr *)&sin, &client_socket_len);

	inet_ntop(AF_INET, &(sin.sin_addr), addr, INET_ADDRSTRLEN);
	ft_printf(PURPLE"Connection from : %s\n"END, addr);

	return (sock);
}

void	stock_in_file(int client_socket)
{
	ssize_t					r;
	char					buf[1024];
	int						fd;

	if ((fd = open("./file", O_RDWR | O_CREAT | O_TRUNC, 0644)) == -1)
		error(FT_OPEN_ERROR);
	while ((r = read(client_socket, buf, 1023)) > 0)
	{
		buf[r] = 0;
		ft_dprintf(fd, "%s", buf);
	}
	ft_printf(GREEN"Success\n"END);
	close(fd);
}

/*
** Une fois le client connecter au serveur, la fonction recv bloque le
** programme jusqu'a qu'elle recoie une donner (via send cote client).
*/

#define BUFF_LEN 1024

void	recv_from_client(int client_socket)
{
	char	buff[BUFF_LEN];
	ssize_t	ret_recv;

	while (true)
	{
		ret_recv = BUFF_LEN;
		ft_memset(&buff, 0, sizeof(buff));
		while (ret_recv == BUFF_LEN)
		{
			if ((ret_recv = recv(client_socket, buff, sizeof(buff) -1, 0)) < 0)
				ft_error(FT_RECV_ERROR);
			buff[ret_recv] = 0;
		}
		ft_printf(RED"%s received\n"END, buff);
		if (!ft_strcmp(buff, "quit") || ft_strlen(buff) == 0)
			break ;
		if (!ft_strcmp(buff, "pwd"))
			getcwd(buff, BUFF_LEN);
		if (send(client_socket, buff, strlen(buff), 0) < 0)
			ft_error(FT_SEND_ERROR);
	}
}
