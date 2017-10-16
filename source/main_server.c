/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_serveur.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/16 14:15:46 by rabougue          #+#    #+#             */
/*   Updated: 2017/10/16 19:00:32 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

void	usage(char *argv)
{
	ft_dprintf(2, "Usage : %s [port]\n", argv);
	exit(EXIT_FAILURE);
}

int	create_server(int	port)
{
	int				sock;
	struct protoent	*proto;

	proto = getprotobyname("tcp");
	if (proto)
	{
		ft_dprint(2, "getprotobyname error !\n");
		return (EXIT_FAILURE);
	}
	sock = socket(PF_INET, SOCK_STREAM, );
	return (sock);
}

int	main(int argc, char **argv)
{
	int	port;
	int	sock;

	if (argc != 2)
		usage(argv[0]);
	port = ft_atoi(argv[1]);
	sock = create_server(port);
	if (sock)
		return (EXIT_FAILURE);
	close(sock);
	return (EXIT_SUCCESS);
}
