/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 04:56:31 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/24 04:59:43 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_dprintf.h"

int			ft_printf(const char *format, ...)
{
	t_printf	print;
	va_list		pa;

	if (*format == '%' && ft_strlen(format) == 1)
		return (0);
	ft_memset(&print, 0, sizeof(print));
	va_start(pa, format);
	count_buff_size(&print, format, pa);
	va_end(pa);
	print.is_percent_s = 1;
	print.is_percent_d = 1;
	print.is_percent_c = 1;
	print.is_percent_p = 1;
	print.is_percent_x = 1;
	print.is_percent_o = 1;
	va_start(pa, format);
	write_string(&print, format, pa);
	print_buff(&print, 1);
	va_end(pa);
	return (print.buff_size);
}
