/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   percent_x.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/22 13:43:11 by rabougue          #+#    #+#             */
/*   Updated: 2017/02/23 21:59:21 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_dprintf.h"

void	percent_x(t_printf *print, va_list pa, int upper_lower)
{
	int		x;
	char	*hexa_itoa;

	x = va_arg(pa, int);
	if (print->is_percent_x == 1)
	{
		if (upper_lower == 0)
			hexa_itoa = ft_hexa_itoa(x, 0);
		else
			hexa_itoa = ft_hexa_itoa(x, 1);
		ft_strcat(print->buff, hexa_itoa);
		print->i += ft_strlen(hexa_itoa);
	}
	else
	{
		hexa_itoa = ft_hexa_itoa(x, 0);
		print->buff_size += ft_strlen(hexa_itoa);
	}
	ft_strdel(&hexa_itoa);
}
