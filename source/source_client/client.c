/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/06 00:25:03 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/07 06:56:01 by rabougue         ###   ########.fr       */
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

void	command(int sock)
{
	ssize_t	r;
	char	buf[1024];

	while(ft_printf(YELLOW"ftp> "END)&&(r = read(STDIN_FILENO, &buf, 1023)) > 0)
	{
		buf[r -1] = '\0';
		if (ft_strcmp("quit", buf) == 0 && ft_printf(YELLOW"ftp> Bye\n"END))
			return ;
		else if (ft_strcmp("pwd", buf) == 0)
			send_to_server(sock, buf);
	}
	ft_printf(YELLOW"Bye\n"END);
}

void	send_to_server(int sock, char *buf)
{
	char	buff[1024];
	ssize_t	ret_recv;

	ft_memset(&buff, 0, sizeof(buff));
	if(send(sock, buf, strlen(buf), 0) < 0)
		ft_error(FT_SEND_ERROR);

	/*while (true)*/
	/*{*/
		if ((ret_recv = recv(sock, buff, sizeof(buff) -1, 0)) < 0)
			ft_error(FT_RECV_ERROR);
		buff[ret_recv] = 0;
		ft_printf("%s\n", buff);
		ft_printf("%d\n", ret_recv);
		/*if (ret_recv == 0)*/
			/*break ;*/
	/*}*/

}
