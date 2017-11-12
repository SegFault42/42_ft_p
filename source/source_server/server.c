/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/06 00:26:50 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/08 04:58:27 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

/*
** bind() liason du socket au client
** listen() ecoute les conexion entrante
*/

int	create_server(uint16_t port)
{
	int					sock;
	char				addr[INET_ADDRSTRLEN];
	uint32_t			client_socket_len;
	struct protoent		*proto;
	struct sockaddr_in	sin;

	if ((proto = getprotobyname("tcp")) == NULL)
		ft_error(FT_GET_PROTO_BY_NAME_ERROR);
	if ((sock = socket(PF_INET, SOCK_STREAM, proto->p_proto)) == -1)
		ft_error(FT_SOCKET_ERROR);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if ((bind(sock, (const struct sockaddr *)&sin, sizeof(sin))) == -1)
		ft_error(FT_BIND_ERROR);
	if ((listen(sock, 42)) == -1)
		ft_error(FT_LISTEN_ERROR);
	sock = accept(sock, (struct sockaddr *)&sin, &client_socket_len);

	inet_ntop(AF_INET, &(sin.sin_addr), addr, INET_ADDRSTRLEN);
	ft_printf(PURPLE"Connection from : %s\n"END, addr);

	return (sock);
}

void	stock_in_file(int client_socket)
{
	ssize_t					r;
	char					buf[1024];
	int						fd;

	if ((fd = open("./file", O_RDWR | O_CREAT | O_TRUNC, 0644)) == -1)
		error(FT_OPEN_ERROR);
	while ((r = read(client_socket, buf, 1023)) > 0)
	{
		buf[r] = 0;
		ft_dprintf(fd, "%s", buf);
	}
	ft_printf(GREEN"Success\n"END);
	close(fd);
}

/*
** Une fois le client connecter au serveur, la fonction recv bloque le
** programme jusqu'a qu'elle recoie une donner (via send cote client).
*/

#define BUFF_LEN 1024

static bool	exec_simple_command(char *buff)
{
	char	**split;
	bool	i;

	i = false;
	split = ft_strsplit_blank(buff);
	if (!ft_strcmp(COMMAND, "pwd"))
	{
		if (getcwd(buff, BUFF_LEN) == NULL)
			ft_strcpy(buff, "Gerer le cas ou getcw a un long path"); // Ne pas oublier
		i = true;
	}
	else if (!ft_strcmp(COMMAND, "cd"))
	{
		if (chdir(split[1]))
			ft_strcpy(buff, "Invalid directory");
		else
			ft_strcpy(buff, "Directory changed");
		i = true;
	}
	ft_2d_tab_free(split);
	return (i);
}

static void	exec_advanced_cmd(char *buff, int client_socket)
{
	char	**split;
	int		child_pid;
	ssize_t	ret_send;
	ssize_t	ret_read;
	int	fd;
	char	buf[4096];

	split = ft_strsplit_blank(buff);
	child_pid = fork();
	if (child_pid == -1)
	{
		perror("can't fork");
		exit(errno);
	}
	else if (child_pid == 0)
	{
		fd = open("./file", O_RDWR | O_CREAT | O_TRUNC, 0644);
		if (dup2(client_socket, STDOUT_FILENO) == -1)
			ft_error(FT_DUP2_ERROR);
		if (dup2(client_socket, STDERR_FILENO) == -1)
			ft_error(FT_DUP2_ERROR);
		close(client_socket);
		execv("/bin/ls", split);
		while ((ret_read = read(client_socket, buf, 4096)) > 0)
		{
			ret_send = send(client_socket, buff, 4096, 0);
			if (ret_send == -1)
				ft_error(FT_SEND_ERROR);
			ft_memset(buff, 0, 4096);
		}
		exit(0);
	}
	else
		wait4(child_pid, 0, 0, 0);
	ft_2d_tab_free(split);
}


void	recv_from_client(int client_socket)
{
	char	buff[BUFF_LEN];
	ssize_t	ret_recv;
	ssize_t	ret_send;
	/*ssize_t	ret_read;*/

	while (true)
	{
		ret_recv = BUFF_LEN;
		ft_memset(&buff, 0, sizeof(buff));
		while (ret_recv == BUFF_LEN)
		{
			if ((ret_recv = recv(client_socket, buff, sizeof(buff) -1, 0)) < 0)
				ft_error(FT_RECV_ERROR);
			buff[ret_recv] = 0;
		}
		ft_printf(YELLOW"%s received\n"END, buff);
		if (!ft_strcmp(buff, "quit") || !ft_strlen(buff))
			break ;

		if (exec_simple_command(buff) == true)
		{
			if ((ret_send = send(client_socket, buff, ft_strlen(buff), 0) < 0))
				ft_error(FT_SEND_ERROR);
		}
		else
		{
			exec_advanced_cmd(buff, client_socket);
			/*ret_read = read(client_socket, buff, BUFF_LEN -1);*/
			
			/*ft_printf("ret_read = %d\n", ret_read);*/
			/*buff[ret_read] = 0;*/
			/*if ((ret_send = send(client_socket, buff, ft_strlen(buff), 0) < 0))*/
				/*ft_error(FT_SEND_ERROR);*/
			/*ft_printf("ret_send = %d\n", ret_send);*/
			
			/*if (ret_read == -1)*/
				/*perror("ret_read()");*/
		}
	}
}
