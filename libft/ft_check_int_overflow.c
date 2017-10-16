/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_int_overflow.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/19 21:40:26 by rabougue          #+#    #+#             */
/*   Updated: 2016/11/19 21:45:29 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libft.h"

bool	check_int_overflow(long nb)
{
	char	*number;

	if (nb > INT_MAX || nb < INT_MIN)
		return (EXIT_FAILURE);
	number = ft_ltoa(nb);
	if (ft_strlen(number) > 11)
	{
		free(number);
		return (EXIT_FAILURE);
	}
	free(number);
	return (EXIT_SUCCESS);
}
