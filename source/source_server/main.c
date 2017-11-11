/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_serveur.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/16 14:15:46 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/08 01:06:38 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

int	main(int argc, char **argv)
{
	uint16_t	port;
	int			client_socket;

	if (argc != 2)
		usage(argv[0], SERVER);
	port = is_port_valid(argv, SERVER);

	ft_printf(GREEN"FTP_SERVER started.\n"END);

	client_socket = create_server(port);

	/*stock_in_file(client_socket);*/
	recv_from_client(client_socket);

	close(client_socket);
	ft_printf(GREEN"FTP_SERVER closed.\n"END);
	return (EXIT_SUCCESS);
}
