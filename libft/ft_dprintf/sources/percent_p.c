/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   percent_p.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/22 13:43:11 by rabougue          #+#    #+#             */
/*   Updated: 2017/02/23 21:59:21 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_dprintf.h"

void	percent_p(t_printf *print, va_list pa)
{
	char	*p;
	char	*hexa_ltoa;

	p = va_arg(pa, char*);
	hexa_ltoa = ft_hexa_ltoa((unsigned long long)p, 0);
	if (print->is_percent_p == 1)
	{
		ft_strcat(print->buff, "0x");
		ft_strcat(print->buff, hexa_ltoa);
		print->i += ft_strlen(hexa_ltoa) + 2;
	}
	else
		print->buff_size += ft_strlen(hexa_ltoa) + 2;
	ft_strdel(&hexa_ltoa);
}
