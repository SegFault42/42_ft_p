/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/05 01:07:44 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/24 05:00:25 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_CLIENT_H
# define FT_CLIENT_H

int		create_client(char *addr, uint16_t port);
void	command(int sock);
void	send_to_server(int sock);
int8_t	exec_quit(char **split);

int8_t	easy_cmd(int socket, char *comp_cmd, char **split);
int8_t	medium_cmd(int socket, char *comp_cmd);
int8_t	hard_cmd(int socket, char *comp_cmd, char **split);

int8_t	client_put(int socket, char *comp_cmd, char **split);
void	exec_put(int socket, int fd);
int8_t	client_get(int socket, char *comp_cmd, char **split);
void	get_cmd(int socket, char **split);

int8_t	cmd_exist(char **split);
int8_t	local_cmd(char **split);
void	exec_lcd(char **split);
void	exec_lls(char **split, uint8_t flag);

void	progress_bar(long int end, long int current);

#endif
