/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_serveur.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/16 14:15:46 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/06 00:27:26 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

int	main(int argc, char **argv)
{
	int					port;
	int					sock;
	int					client_socket;
	uint32_t			client_socket_len;
	struct sockaddr_in	client_socket_in;

	if (argc != 2)
		usage(argv[0], SERVER);
	ft_printf(GREEN"FTP_SERVER started.\n"END);
	port = is_port_valid(argv, SERVER);
	sock = create_server(port);
	client_socket = accept(sock, (struct sockaddr *)&client_socket_in, &client_socket_len);
	stock_in_file(client_socket);
	close(client_socket);
	close(sock);
	ft_printf(GREEN"FTP_SERVER closed.\n"END);
	return (EXIT_SUCCESS);
}
