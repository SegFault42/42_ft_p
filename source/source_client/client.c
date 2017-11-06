/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/06 00:25:03 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/06 05:21:11 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

void	command(int sock)
{
	ssize_t	r;
	char	buf[1024];

	while((r = read(STDIN_FILENO, &buf, 1023)) > 0)
	{
		buf[r -1] = '\0';
		if (ft_strcmp("quit", buf) == 0)
			return ;
		else if (ft_strcmp("pwd", buf) == 0)
			system("/bin/pwd") ;
		ft_dprintf(1, YELLOW"ftp> "END);
	}
}

int	create_client(char *addr, int port)
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
	sin.sin_addr.s_addr = inet_addr(addr);
	if ((connect(sock, (const struct sockaddr *)&sin, sizeof(sin))) == -1)
		ft_error(FT_CONNECT_ERROR);
	return (sock);
}

