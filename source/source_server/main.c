/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_serveur.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/16 14:15:46 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/05 06:11:05 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/common.h"

int	create_server(int	port)
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
	if ((listen(sock, 42)) == -1)
		ft_error(FT_LISTEN_ERROR);
	return (sock);
}

int	is_port_valid(char **argv)
{
	int	port;

	port = ft_atoi(argv[1]);
	if (port < 1024 || port > 65535)
		usage(argv[0]);
	return (port);
}

void	stock_in_file(int client_socket)
{
	ssize_t					r;
	char					buf[1024];
	int						fd;

	if ((fd = open("./file", O_RDWR | O_CREAT | O_TRUNC, 0644)) == -1)
		error(FT_OPEN_ERROR);
	ft_printf(YELLOW"Copy ...\n"END);
	while ((r = read(client_socket, buf, 1023)) > 0)
	{
		buf[r] = 0;
		ft_dprintf(fd, "%s", buf);
	}
	ft_printf(GREEN"Success\n"END);
	close(fd);
}

int	main(int argc, char **argv)
{
	int					port;
	int					sock;
	int					client_socket;
	uint32_t			client_socket_len;
	struct sockaddr_in	client_socket_in;

	if (argc != 2)
		usage(argv[0]);
	ft_printf(GREEN"FTP_SERVER started.\n"END);
	port = is_port_valid(argv);
	sock = create_server(port);
	client_socket = accept(sock, (struct sockaddr *)&client_socket_in, &client_socket_len);
	stock_in_file(client_socket);
	close(client_socket);
	close(sock);
	return (EXIT_SUCCESS);
}
