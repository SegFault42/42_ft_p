/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_serveur.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/16 14:15:46 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/04 05:16:00 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

static int	ft_error(int	error)
{
	ft_printf(RED"");
	if (error == FT_SOCKET_ERROR)
		ft_dprintf(2, "socket() failure !\n");
	else if (error == FT_GET_PROTO_BY_NAME_ERROR)
		ft_dprintf(2, "getprotobyname() failure !\n");
	else if (error == FT_BIND_ERROR)
		ft_dprintf(2, "bind() failure !\n");
	else if (error == FT_LISTEN_ERROR)
		ft_dprintf(2, "listen() failure !\n");
	ft_printf(""END);
	ft_dprintf(2, YELLOW"errno : %s\n"END, strerror(errno));
	exit(error);
}

int	usage(char *argv)
{
	ft_dprintf(2, "Usage : %s [port] (between 1024 and 65535)\n", argv);
	exit(EXIT_FAILURE);
}

int	create_server(int	port)
{
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;

<<<<<<< HEAD
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
=======
	proto = getprotobyname("tcp");
	if (proto)
	{
		ft_dprintf(2, "getprotobyname error !\n");
		return (EXIT_FAILURE);
	}
	/*sock = socket(PF_INET, SOCK_STREAM, );*/
>>>>>>> fab8da19facfcfae2370382129b8d161d47c53d8
	return (sock);
}

static void	is_port_valid(char **argv)
{
	int	port;

	port = ft_atoi(argv[1]);
	if (port < 1024 || port > 65535)
		usage(argv[0]);
}

int	main(int argc, char **argv)
{
	int			port;
	int			sock;
	int					client_socket;
	struct sockaddr_in	client_socket_in;
	uint32_t			client_socket_len;
	char				buf[1024];
	ssize_t					r;

	if (argc != 2)
		usage(argv[0]);
	port = ft_atoi(argv[1]);
	is_port_valid(argv);
	sock = create_server(port);
	client_socket = accept(sock, (struct sockaddr *)&client_socket_in, &client_socket_len);
	while ((r = read(client_socket, buf, 1023)) > 0)
	{
		buf[r] = 0;
		ft_printf("receveid %d byte(s) : %s\n", r, buf);
	}
	close(client_socket);
	close(sock);
	return (EXIT_SUCCESS);
}
