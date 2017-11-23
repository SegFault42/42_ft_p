/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/16 14:15:46 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/23 01:12:01 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

char	g_orig_dir[PATH_MAX] = {0};

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

	if (getcwd(g_orig_dir, PATH_MAX) == NULL)
	{
		ft_printf(RED"Cannot get base directory\n"END);
		return (errno);
	}
	/*signal(SIGINT, prompt);*/
	port = is_port_valid(argv, CLIENT);
	sock = create_client(argv[1], port);

	send_to_server(sock);

	close(sock);
	return (EXIT_SUCCESS);
}
