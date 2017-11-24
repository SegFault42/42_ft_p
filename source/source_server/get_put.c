/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_put.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 06:25:03 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/24 06:31:54 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

int8_t	check_right_client(int socket)
{
	char	buffer[BUFFER_SIZE];

	recv(socket, buffer, sizeof(buffer), 0);
	if (!ft_strcmp(buffer, "ERROR"))
		return (false);
	else
		return (true);
}

void	exec_get(int socket, int fd)
{
	char		buffer[BUFFER_SIZE];
	ssize_t		ret_read;
	ssize_t		ret_send;
	struct stat	st;

	ret_send = 0;
	if (check_right_client(socket) == false)
		return ;
	fstat(fd, &st);
	send_file_size(socket, st.st_size);
	while ((ret_read = read(fd, buffer, sizeof(buffer))) > 0)
		ret_send = send(socket, buffer, (size_t)ret_read, 0);
}

void	server_get(int socket, char **split)
{
	char	buffer[BUFFER_SIZE];
	int		fd;

	if (check_right(get_directory(split), buffer) != 1 ||
		(fd = check_file_exist(split[1], buffer)) == -1)
	{
		send(socket, buffer, sizeof(buffer), 0);
		return ;
	}
	else
	{
		ft_strcpy(buffer, "SUCCESS");
		send(socket, buffer, sizeof(buffer), 0);
		exec_get(socket, fd);
		close(fd);
	}
}

void	server_put(int socket, char **split)
{
	char	buffer[BUFFER_SIZE];
	int		fd;
	long	size;
	ssize_t	ret_recv;
	char	*file;

	file = extract_name_from_path(split[1]);
	if ((fd = open(file, O_RDONLY)) != -1)
	{
		ft_printf(RED"File exist\n"END);
		close(fd);
		ft_strcpy(buffer, "ERROR");
		send(socket, buffer, sizeof(buffer), 0);
		return ;
	}
	if ((fd = check_right_writing(socket, split[1])) == -1)
		return ;
	recv(socket, buffer, BUFFER_SIZE, 0);
	size = ft_atol(buffer);
	while (size > 0)
	{
		ret_recv = recv(socket, buffer, sizeof(buffer), 0);
		write(fd, &buffer, (size_t)ret_recv);
		size -= ret_recv;
	}
}
