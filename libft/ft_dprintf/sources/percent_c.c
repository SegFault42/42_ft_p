/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   percent_c.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/14 15:06:00 by rabougue          #+#    #+#             */
/*   Updated: 2017/02/21 22:33:36 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_dprintf.h"

void	percent_c(t_printf *print, va_list pa)
{
	char	c;

	c = va_arg(pa, int);
	if (print->is_percent_c == 1)
	{
		print->buff[print->i] = c;
		++print->i;
	}
	else
		++print->buff_size;
}
