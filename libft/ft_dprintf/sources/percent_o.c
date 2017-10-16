/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/29 15:44:19 by rabougue          #+#    #+#             */
/*   Updated: 2017/02/23 21:59:20 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_dprintf.h"

void	percent_o(t_printf *print, va_list pa)
{
	long	o;
	char	*ltoa_base;

	o = va_arg(pa, long);
	if (print->is_percent_x == 1)
	{
		ltoa_base = ft_ltoa_base(o, 8);
		ft_strcat(print->buff, ltoa_base);
		print->i += ft_strlen(ltoa_base);
	}
	else
	{
		ltoa_base = ft_ltoa_base(o, 8);
		print->buff_size += ft_strlen(ltoa_base);
	}
	ft_strdel(&ltoa_base);
}
