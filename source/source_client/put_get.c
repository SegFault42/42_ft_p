/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 04:37:22 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/24 04:54:23 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

extern char	*g_ft_errno[];

void		get_cmd(int socket, char **split)
{
	char		buffer[BUFFER_SIZE];
	int			fd;
	ssize_t		ret_recv;
	long		size[2];
	long double	size_recv;

	size_recv = 0;
	if ((fd = check_right_writing(socket, split[1])) == -1 &&
	ft_printf(RED"%s\n"END, ERRNO))
		return ;
	recv(socket, buffer, sizeof(buffer), 0);
	size[0] = ft_atol(buffer);
	size[1] = size[0];
	ft_printf("\033[?25l");
	while (size[0] > 0)
	{
		ret_recv = recv(socket, buffer, sizeof(buffer), 0);
		write(fd, &buffer, (size_t)ret_recv);
		ft_memset(buffer, 0, sizeof(buffer));
		size[0] -= ret_recv;
		size_recv += ret_recv;
		progress_bar(size[1], (long)size_recv);
		ft_printf("\r");
	}
	ft_printf("\n\033[?25h%sTransfert success\n%s", GREEN, END);
}

int8_t		client_get(int socket, char *comp_cmd, char **split)
{
	char	buffer[BUFFER_SIZE];
	ssize_t	ret_send;
	int		fd;
	char	*file;

	file = extract_name_from_path(split[1]);
	if ((fd = open(file, O_RDONLY)) != -1)
	{
		ft_printf(RED"File exist\n"END);
		close(fd);
		return (-1);
	}
	if ((ret_send = send(socket, comp_cmd, ft_strlen(comp_cmd), 0)) == -1)
	{
		ft_printf(RED"%s\n"END, ERRNO);
		return (-1);
	}
	recv(socket, buffer, sizeof(buffer), 0);
	if (!ft_strcmp(buffer, "SUCCESS"))
		get_cmd(socket, split);
	else
		ft_printf("%s\n", buffer);
	return (0);
}

void		exec_put(int socket, int fd)
{
	char		buffer[BUFFER_SIZE];
	struct stat	st;
	long		size;
	ssize_t		ret_read;
	long		total_read;

	fstat(fd, &st);
	total_read = 0;
	size = st.st_size;
	send_file_size(socket, size);
	ft_printf("\033[?25l");
	while ((ret_read = read(fd, buffer, sizeof(buffer))) > 0)
	{
		send(socket, buffer, (size_t)ret_read, 0);
		total_read += ret_read;
		progress_bar(size, (long)total_read);
		ft_printf("\r");
	}
	ft_printf("\033[?25h");
	ft_printf(GREEN"\nTransfert success\n"END);
}

int8_t		client_put(int socket, char *comp_cmd, char **split)
{
	int		fd;
	char	buffer[BUFFER_SIZE];

	if ((fd = check_file_exist(split[1], buffer)) == -1)
	{
		ft_printf("%s\n", buffer);
		return (-1);
	}
	send(socket, comp_cmd, ft_strlen(comp_cmd), 0);
	recv(socket, buffer, BUFFER_SIZE, 0);
	if (!ft_strcmp(buffer, "ERROR"))
	{
		ft_dprintf(2, RED"Unable to write file in server side\n"END);
		return (-1);
	}
	exec_put(socket, fd);
	close(fd);
	return (0);
}
