/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/05 01:07:44 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/07 01:53:03 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_CLIENT_H
# define FT_CLIENT_H

int		create_client(char *addr, int port);
void	command(int sock);
void	send_to_server(int sock, char *buf);

#endif
