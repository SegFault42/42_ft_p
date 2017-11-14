/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/05 01:07:44 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/08 01:17:05 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_CLIENT_H
# define FT_CLIENT_H
/*
** setup.c
*/
int		create_client(char *addr, uint16_t port);
/*
** command.c
*/
void	command(int sock);
/*
** client_to_server.c
*/
void	send_to_server(int sock);
/*
** exec_cmd.c
*/
int8_t	exec_quit(char **split);
int	exec_get(char *comp_cmd, int sock);

#endif
