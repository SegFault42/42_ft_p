/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/16 14:15:46 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/07 23:22:31 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

/*static void	prompt(int signal)*/
/*{*/
	/*if (signal == SIGINT)*/
		/*ft_printf(YELLOW"\nftp> "END);*/
/*}*/

int	main(int argc, char **argv)
{
	uint16_t	port;
	int			sock;

	if (argc != 3)
		usage(argv[0], CLIENT);

	/*signal(SIGINT, prompt);*/
	port = is_port_valid(argv, CLIENT);
	sock = create_client(argv[1], port);

	send_to_server(sock);

	close(sock);
	return (EXIT_SUCCESS);
}
