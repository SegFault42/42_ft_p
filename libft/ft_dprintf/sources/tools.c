/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/29 15:44:19 by rabougue          #+#    #+#             */
/*   Updated: 2017/02/23 21:59:19 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_dprintf.h"

void	error(int error)
{
	if (error == MALLOC_ERROR)
		ft_putendl_fd("Memory allocation failure", STDERR_FILENO);
	exit(EXIT_FAILURE);
}

void	print_buff(t_printf *print, int fd)
{
	ft_putstr_fd(print->buff, fd);
	ft_strdel(&print->buff);
}
