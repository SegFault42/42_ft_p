/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 04:44:59 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/24 04:45:00 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

/*
** create_client() : configuration pour se connecter au client.
** getprotobyname() : remplie la structure proto par les valeur pour une
** connection tcp. (voir /etc/protocols)
** sock() : cree un socket (comme open() pour un file descriptor)
** On remplie la structure sockaddr_in. AF_INET specifie qu'on utilise le
** protocole IP, htons() stock le port formatter au bonne endianless, inet_addr
** renvoie l'adresse IP correctement formatter
** La structure est remplie, on peut demander une connection avec connect()
*/

int	create_client(char *addr, uint16_t port)
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
	if (!ft_strcmp("localhost", addr))
		sin.sin_addr.s_addr = inet_addr("127.0.0.1");
	else
		sin.sin_addr.s_addr = inet_addr(addr);
	if ((connect(sock, (const struct sockaddr *)&sin, sizeof(sin))) == -1)
		ft_error(FT_CONNECT_ERROR);
	return (sock);
}
