/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/29 15:44:19 by rabougue          #+#    #+#             */
/*   Updated: 2017/04/06 19:49:47 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_dprintf.h"

static void	count_buff_size2(t_printf *print, const char *format, va_list pa)
{
	if (*format == 'p')
		percent_p(print, pa);
	else if (*format == 'o' || *format == 'O')
		percent_o(print, pa);
	else if (*format == 'x')
		percent_x(print, pa, 0);
	else if (*format == 'X')
		percent_x(print, pa, 1);
	else
		++print->buff_size;
}

void		count_buff_size(t_printf *print, const char *format, va_list pa)
{
	while (*format)
	{
		if (*format == '%')
		{
			++format;
			while (*format == ' ')
				++format;
			if (*format == 'c')
				percent_c(print, pa);
			else if (*format == 'd' || *format == 'i' || *format == 'u' ||
			*format == 'U' || *format == 'D')
				percent_d(print, pa, *format);
			else if (*format == 's')
				percent_s(print, pa);
			else
				count_buff_size2(print, format, pa);
		}
		else
			++print->buff_size;
		++format;
	}
}

void		specifier(const char *format, va_list pa, t_printf *print)
{
	if (*format == 'c')
		percent_c(print, pa);
	else if (*format == 'd' || *format == 'i' || *format == 'u' ||
	*format == 'U' || *format == 'D')
		percent_d(print, pa, *format);
	else if (*format == 's')
		percent_s(print, pa);
	else if (*format == 'p')
		percent_p(print, pa);
	else if (*format == 'x')
		percent_x(print, pa, 0);
	else if (*format == 'o' || *format == 'O')
		percent_o(print, pa);
	else if (*format == 'X')
		percent_x(print, pa, 1);
	else
	{
		print->buff[print->i] = *format;
		++print->i;
	}
}

void		write_string(t_printf *print, const char *format, va_list pa)
{
	if ((print->buff = ft_memalloc(print->buff_size + 1)) == NULL)
		error(MALLOC_ERROR);
	while (*format)
	{
		if (*format == '%')
		{
			++format;
			while (*format == ' ')
				++format;
			if (*format == '%')
			{
				ft_strcat(print->buff, "%");
				print->i++;
			}
			else
				specifier(format, pa, print);
		}
		else
		{
			print->buff[print->i] = *format;
			++print->i;
		}
		++format;
	}
}

int			ft_dprintf(int fd, const char *format, ...)
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
	print_buff(&print, fd);
	va_end(pa);
	return (print.buff_size);
}
