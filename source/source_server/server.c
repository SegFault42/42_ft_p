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

extern char	g_orig_dir[PATH_MAX];

void	dup_server(int client_socket, struct sockaddr_in sin, uint32_t client_socket_len)
{
	int	child_pid;
	int	sock;
	int	cs;

	while (true)
	{
		sock = accept(client_socket, (struct sockaddr *)&sin, &client_socket_len);
		child_pid = fork();
		if (child_pid == -1)
		{
			/*perror("can't fork");*/
			close(sock);
			continue ;
		}
		else if (child_pid > 0)
			continue ;
		else if (child_pid == 0)
		{
			/*inet_ntop(AF_INET, &(sin.sin_addr), addr, INET_ADDRSTRLEN);*/
			ft_printf(PURPLE"New conexion\n"END);
			recv_from_client(sock);
			close(sock);
			break ;
		}
	}
}

/*
** bind() liason du socket au client
** listen() ecoute les conexion entrante
*/

int	create_server(uint16_t port)
{
	int					sock;
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
	if ((listen(sock, 2)) == -1)
		ft_error(FT_LISTEN_ERROR);
	return (sock);
}

void	stock_in_file(int client_socket)
{
	int		fd;
	ssize_t	r;
	char	buf[1024];

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

static void builtin_cd(char *dir, char *buff)
{
	char	new_dir[PATH_MAX];
	char	old_dir[PATH_MAX];

	if (getcwd(old_dir, PATH_MAX) == NULL)
	{
		ft_strcpy(buff, "Get current dir failure, cannot exec cd.");
		return ;
	}
	if (chdir(dir) == -1)
	{
		ft_strcpy(buff, "Invalid directory");
		return ;
	}
	if (getcwd(new_dir, PATH_MAX) == NULL)
	{
		ft_strcpy(buff, "Get new dir failure, cannot exec cd.");
		return ;
	}
	if (ft_count_char(new_dir, '/') < ft_count_char(g_orig_dir, '/'))
	{
		ft_strcpy(buff, "Insufficient permissions");
		if (chdir(old_dir))
			ft_strcpy(dir, "Invalid directory");
		return ;
	}
	ft_strcpy(buff, "Directory changed");
}

static void	cmd_get(char *buff, char *file, int client_socket)
{
	int	fd;
	int	ret_send;
	char buffer[4096];

	ft_memset(buffer, 0, sizeof(buffer));
	if ((fd = open(file, O_RDONLY)) == -1)
	{
		ft_strcpy(buff, "get failure !");
		perror("open");
		return ;
	}
	while (read(fd, buffer, 4096) > 0)
	{
		ret_send = send(client_socket, buffer, sizeof(buffer), 0);
		if (ret_send == -1)
			ft_strcpy(buff, "Send Failure");
		ft_memset(buffer, 0, sizeof(buffer));
	}
	close(fd);
}

static int8_t	exec_simple_command(char *buff, int client_socket)
{
	char	**split;

	split = ft_strsplit_blank(buff);
	if (!ft_strcmp(COMMAND, "pwd"))
	{
		if (getcwd(buff, BUFF_LEN) == NULL)
			ft_strcpy(buff, "Gerer le cas ou getcw a un long path"); // Ne pas oublier
		return (true);
	}
	else if (!ft_strcmp(COMMAND, "cd"))
	{
		builtin_cd(split[1], buff);
		return (true);
	}
	else if (!ft_strcmp(COMMAND, "mkdir"))
	{
		if (mkdir(split[1], S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
			ft_strcpy(buff, "mkdir failed !");
		else
			ft_strcpy(buff, "Directory created");
		return (true);
	}
	else if (!ft_strcmp(COMMAND, "get"))
	{
		cmd_get(buff, split[1], client_socket);
		return (QUIET);
	}
	ft_2d_tab_free(split);
	return (false);
}

static void	exec_advanced_cmd(char *cmd, int client_socket)
{
	char	**split;
	int		child_pid;
	ssize_t	ret_send;
	ssize_t	ret_read;
	char	buf[4096];

	split = ft_strsplit_blank(cmd);
	child_pid = fork();
	if (child_pid == -1)
	{
		perror("can't fork");
		exit(errno);
	}
	else if (child_pid == 0)
	{
		if (dup2(client_socket, STDOUT_FILENO) == -1)
			ft_error(FT_DUP2_ERROR);
		if (dup2(client_socket, STDERR_FILENO) == -1)
			ft_error(FT_DUP2_ERROR);
		close(client_socket);
		execv("/bin/ls", split);
		while ((ret_read = read(client_socket, buf, sizeof(buf))) > 0 || ret_read != -1)
		{
			ret_send = send(client_socket, buf, sizeof(buf), 0);
			if (ret_send == -1)
				ft_error(FT_SEND_ERROR);
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
	ssize_t	exec;;

	while (true)
	{
		ret_recv = BUFF_LEN;
		ft_memset(buff, 0, sizeof(buff));
		while (ret_recv == BUFF_LEN)
		{
			if ((ret_recv = recv(client_socket, buff, sizeof(buff) -1, 0)) < 0)
				ft_error(FT_RECV_ERROR);
			buff[ret_recv] = 0;
		}
		if (ft_strlen(buff))
			ft_printf(YELLOW"%s received\n"END, buff);
		if (!ft_strcmp(buff, "quit") || !ft_strlen(buff))
			break ;
		exec = exec_simple_command(buff, client_socket);
		if (exec == true)
		{
			if ((ret_send = send(client_socket, buff, ft_strlen(buff), 0) < 0))
				ft_error(FT_SEND_ERROR);
		}
		else if (exec == QUIET)
		{
			if ((ret_send = send(client_socket, "\0", 1, 0) < 0))
				ft_error(FT_SEND_ERROR);
		}
		else if (exec == false)
			exec_advanced_cmd(buff, client_socket);

	}
}
