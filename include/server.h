/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveur.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/05 01:08:22 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/24 06:34:00 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SERVER_H
# define FT_SERVER_H

int			create_server(uint16_t port);
void		stock_in_file(int client_socket);
void		recv_from_client(int client_socket);
void		dup_server(int client_socket, struct sockaddr_in sin,
uint32_t	client_socket_len);

void		exec_pwd(int socket, char **split);
void		exec_cd(int socket, char **split);
void		exec_mkdir(int socket, char **split);
void		exec_rmdir(int socket, char **split, uint8_t flag);
int8_t		exec_easy_cmd(int socket, char **split);

int8_t		get_level_cmd(char *str);
int8_t		check_right(char *path, char *buff);
int8_t		mkdir_directory(char **split, char *buff);
ssize_t		prepare_ls(int socket, char **split, struct stat *st);
void		exec_ls(int socket, char **split);

void		server_put(int socket, char **split);
void		server_get(int socket, char **split);
void		exec_get(int socket, int fd);
int8_t		check_right_client(int socket);

char		*get_directory(char **split);
void		exec_medium_cmd(int socket, char **split);
void		exec_hard_cmd(int socket, char **split);

#endif
