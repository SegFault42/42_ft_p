/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/06 00:25:03 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/08 04:32:14 by rabougue         ###   ########.fr       */
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

static char	*g_cmds[] = {"ls", "cd", "get", "put", "pwd", "quit", "mkdir", NULL};

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

void	command(int sock)
{
	ssize_t	read_ret;
	char	buff[1024];
	int8_t	iter;
	char	**split;

	while(ft_printf(YELLOW"ftp> "END)&&(read_ret = read(STDIN_FILENO, &buff, 1023)) > 0)
	{
		buff[read_ret -1] = '\0';
		if (read_ret == 1 || (split = ft_strsplit_blank(buff)) == NULL)
			continue;
		iter = 0;
		while (g_cmds[iter])
		{
			if (!ft_strcmp(split[0], g_cmds[iter]))
			{
				if (send_to_server(sock, buff) == QUIT)
				{
					ft_2d_tab_free(split);
					return ;
				}
				break ;
			}
			++iter;
		}
		ft_2d_tab_free(split);
		if (iter == ft_count_2d_tab(g_cmds))
			ft_dprintf(2, "Unknow command !\n");
	}
}

int	send_to_server(int sock, char *cmd)
{
	char	buff[4096 + 1];
	ssize_t	ret_recv;
	int		fd;
	char	**split;

	fd = 0;
	ft_memset(buff, 0, sizeof(buff));

	split = ft_strsplit_blank(cmd);
	if (!ft_strcmp("quit", split[0]) && ft_printf(YELLOW"ftp> Bye\n"END))
	{
		ft_2d_tab_free(split);
		return (QUIT);
	}
	else if (!ft_strcmp("get", split[0]))
	{
		if ((fd = open(split[1], O_CREAT | O_RDWR | O_TRUNC, 0644)) == -1)
			ft_dprintf(2, "Open error (get)\n");
		ft_2d_tab_free(split);
	}
	if(send(sock, cmd, strlen(cmd), 0) < 0)
		ft_error(FT_SEND_ERROR);
	while (1)
	{
		if ((ret_recv = recv(sock, buff, 4096, 0)) < 0)
			ft_error(FT_RECV_ERROR);
		buff[ret_recv] = 0;
		if (fd)
			write(fd, buff, ft_strlen(buff));
		else
			write(1, buff, (size_t)ret_recv);
		/*ft_printf(PURPLE"{%d}"END, ret_recv);*/
		if (ret_recv != sizeof(buff) -1 )
			break ;
		ft_memset(buff, 0, sizeof(buff));
	}
	if (fd)
		close(fd);
	RC;
	return (1);
}
