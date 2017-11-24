/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 05:16:28 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/24 05:17:19 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

char	*extract_name_from_path(char *path)
{
	char	*file;

	if ((file = ft_strchr(path, '/')) == NULL)
		file = path;
	else
	{
		file = ft_strrchr(path, '/');
		++file;
	}
	return (file);
}

int		check_file_exist(char *file, char *buff)
{
	int			fd;
	struct stat	st;

	if ((fd = open(file, O_RDONLY)) == -1)
	{
		if (buff)
			ft_strcpy(buff, RED"Getting file error"END);
		return (-1);
	}
	if (fstat(fd, &st) == -1)
	{
		if (buff)
			ft_strcpy(buff, RED"fstat error"END);
		return (-1);
	}
	if (!S_ISREG(st.st_mode))
	{
		if (buff)
			ft_strcpy(buff, RED"Not a regular file"END);
		return (-1);
	}
	return (fd);
}

int		check_right_writing(int socket, char *split)
{
	int		fd;
	char	buffer[BUFFER_SIZE];

	fd = open(extract_name_from_path(split), O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		ft_strcpy(buffer, "ERROR");
	else
		ft_strcpy(buffer, "SUCCESS");
	send(socket, buffer, sizeof(buffer), 0);
	return (fd);
}

void	send_file_size(int socket, off_t st_size)
{
	char	*itoa;
	ssize_t	ret_send;

	itoa = ft_ltoa(st_size);
	ret_send = send(socket, itoa, ft_strlen(itoa), 0);
	ft_strdel(&itoa);
}
