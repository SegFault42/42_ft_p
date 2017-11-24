/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/06 09:05:53 by rabougue          #+#    #+#             */
/*   Updated: 2017/11/24 04:58:09 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_DPRINTF_H
# define FT_DPRINTF_H

# include <stdarg.h>
# include <stdio.h>
# include <limits.h>
# include "../../includes/libft.h"

typedef struct	s_printf
{
	int			ret;
	int			buff_size;
	char		*buff;
	uint8_t		is_percent_d;
	uint8_t		is_percent_s;
	uint8_t		is_percent_c;
	uint8_t		is_percent_p;
	uint8_t		is_percent_x;
	uint8_t		is_percent_o;
	int			i;
}				t_printf;

int				ft_dprintf(int fd, const char *format, ...);
int				ft_printf(const char *format, ...);
void			percent_d(t_printf *print, va_list pa, char format);
void			percent_s(t_printf *print, va_list pa);
void			percent_c(t_printf *print, va_list pa);
void			percent_l(t_printf *print, va_list pa);
void			percent_p(t_printf *print, va_list pa);
void			percent_o(t_printf *print, va_list pa);
void			percent_x(t_printf *print, va_list pa, int upper_lower);
/*
** tools.c
*/
void			print_buff(t_printf *print, int fd);
void			error(int error);

void			count_buff_size(t_printf *print, const char *forma, va_list pa);
void			write_string(t_printf *print, const char *format, va_list pa);

#endif
