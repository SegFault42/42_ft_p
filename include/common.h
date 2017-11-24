/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_p.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/16 14:16:13 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/24 05:10:19 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_COMMON_H
# define FT_COMMON_H

# include "../libft/includes/libft.h"
# include <sys/socket.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <netdb.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <errno.h>
# include "./client.h"
# include "./server.h"
# include <termios.h>
# include "./curses.h"

# define FT_SOCKET_ERROR			-1
# define FT_GET_PROTO_BY_NAME_ERROR	-2
# define FT_BIND_ERROR				-3
# define FT_LISTEN_ERROR			-4
# define FT_OPEN_ERROR				-5
# define FT_CONNECT_ERROR			-6
# define FT_SEND_ERROR				-7
# define FT_RECV_ERROR				-7
# define FT_DUP2_ERROR				-8

# define SERVER	0
# define CLIENT	1
# define QUIT	-1

# define QUIET	-1
# define ABORT	-2

# define BUFF_LEN		4096
# define BUFFER_SIZE	4096 * 4
# define MAX_CMD_LEN	4096

# define COMMAND split[0]

# define EASY	1
# define MEDIUM	2
# define HARD	3
# define LOCAL	4

# define RMDIR	0
# define RM		1

# define ROOT		1
# define ANONYMOUS	0

typedef size_t	t_s;

# define L_C(X, x1, x2, y1, y2) ((t_s)(((X - x1) * (y2 - y1)) / (x2 - x1)) + y1)

# define KEY "4UxEc64YRrtCxP5qadBgmB4hL5g99uEZceeiKzQmDHbT3D6UkZAQJJBBgkdgVff7"

int			usage(char *argv, bool type);
int			ft_error(int error);
uint16_t	is_port_valid(char **argv, bool type);
char		*extract_name_from_path(char *path);
int			check_file_exist(char *file, char *buff);
int			check_right_writing(int socket, char *split);
void		send_file_size(int socket, off_t st_size);

#endif
