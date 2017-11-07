/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_p.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/16 14:16:13 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/07 02:55:29 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_COMMON_H
# define FT_COMMON_H

# include "../libft/includes/libft.h"
# include <sys/socket.h>
# include <netdb.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <errno.h>
# include "./client.h"
# include "./server.h"

# define FT_SOCKET_ERROR			-1
# define FT_GET_PROTO_BY_NAME_ERROR	-2
# define FT_BIND_ERROR				-3
# define FT_LISTEN_ERROR			-4
# define FT_OPEN_ERROR				-5
# define FT_CONNECT_ERROR			-6
# define FT_SEND_ERROR				-7
# define FT_RECV_ERROR				-7

# define SERVER	0
# define CLIENT	1

int		usage(char *argv, bool type);
int		ft_error(int error);
uint16_t	is_port_valid(char **argv, bool type);

#endif
