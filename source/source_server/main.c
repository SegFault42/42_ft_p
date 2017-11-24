/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_serveur.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/16 14:15:46 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/24 06:49:03 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

char	g_orig_dir[PATH_MAX] = {0};

int	main(int argc, char **argv)
{
	uint16_t			port;
	int					client_socket;
	uint32_t			client_socket_len;
	struct sockaddr_in	sin;

	if (argc != 2)
		usage(argv[0], SERVER);
	client_socket_len = 0;
	ft_memset(&sin, 0, sizeof(sin));
	port = is_port_valid(argv, SERVER);
	if (getcwd(g_orig_dir, PATH_MAX) == NULL)
	{
		ft_printf(RED"Cannot get base directory\n"END);
		return (errno);
	}
	ft_printf(GREEN"FTP_SERVER started.\n"END);
	client_socket = create_server(port);
	dup_server(client_socket, sin, client_socket_len);
	close(client_socket);
	ft_printf(GREEN"Connexion closed.\n"END);
	return (EXIT_SUCCESS);
}
