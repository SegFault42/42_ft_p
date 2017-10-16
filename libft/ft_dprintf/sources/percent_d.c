/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   percent_d.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/14 15:05:17 by rabougue          #+#    #+#             */
/*   Updated: 2017/04/06 19:48:33 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_dprintf.h"

static void	format_d_i(t_printf *print, va_list pa)
{
	char	*string;

	string = ft_itoa(va_arg(pa, long));
	if (print->is_percent_d == 1)
	{
		ft_strcat(print->buff, string);
		print->i += ft_strlen(string);
	}
	else
		print->buff_size += ft_strlen(string);
	free(string);
}

static void	format_d_upper(t_printf *print, va_list pa)
{
	char	*string;

	string = ft_ltoa(va_arg(pa, long));
	if (print->is_percent_d == 1)
	{
		if (ft_strcmp(string, "-9223372036854775808") == 0)
		{
			ft_strcat(print->buff, "-92233720368547758088");
			print->i += ft_strlen(string);
		}
		else
		{
			ft_strcat(print->buff, string);
			print->i += ft_strlen(string);
		}
	}
	else
		print->buff_size += ft_strlen(string);
	free(string);
}

static void	format_u(t_printf *print, va_list pa)
{
	char	*string;

	string = ft_ultoa(va_arg(pa, unsigned int));
	if (print->is_percent_d == 1)
	{
		ft_strcat(print->buff, string);
		print->i += ft_strlen(string);
	}
	else
		print->buff_size += ft_strlen(string);
	free(string);
}

static void	format_u_upper(t_printf *print, va_list pa)
{
	char	*string;

	string = ft_ultoa(va_arg(pa, unsigned long));
	if (print->is_percent_d == 1)
	{
		ft_strcat(print->buff, string);
		print->i += ft_strlen(string);
	}
	else
		print->buff_size += ft_strlen(string);
	free(string);
}

void		percent_d(t_printf *print, va_list pa, char format)
{
	if (format == 'd' || format == 'i')
		format_d_i(print, pa);
	else if (format == 'D')
		format_d_upper(print, pa);
	else if (format == 'u')
		format_u(print, pa);
	else if (format == 'U')
		format_u_upper(print, pa);
}
