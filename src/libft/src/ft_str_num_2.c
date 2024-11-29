/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_num_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 21:29:40 by vzurera-          #+#    #+#             */
/*   Updated: 2024/04/22 14:42:33 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static long	process_digits(const char **str, int base, int *negative)
{
	long	result;
	int		digit;

	result = 0;
	while (1)
	{
		if (**str >= '0' && **str <= '9')
			digit = **str - '0';
		else if (**str >= 'a' && **str <= 'z')
			digit = **str - 'a' + 10;
		else if (**str >= 'A' && **str <= 'Z')
			digit = **str - 'A' + 10;
		else
			break ;
		if (digit >= base)
			break ;
		if (!*negative && result > (LONG_MAX - digit) / base)
			return (LONG_MAX);
		if (*negative && result < (LONG_MIN + digit) / base)
			return (LONG_MIN);
		result = (result * base) + digit;
		(*str)++;
	}
	return (result);
}

long	ft_atol_base(const char *str, char **endptr, int base)
{
	long	result;
	int		negative;

	result = 0;
	negative = 0;
	while (ft_isspace(*str))
		str++;
	if (*str == '+' || *str == '-')
		negative = (*str++ == '-');
	result = process_digits(&str, base, &negative);
	if (endptr)
		*endptr = (char *)str;
	if (negative)
		result = -result;
	return (result);
}

long	ft_atol(char *str)
{
	long	result;
	int		sign;
	int		digit;

	result = 0;
	sign = 1;
	while (ft_isspace(*str))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (ft_isdigit(*str))
	{
		digit = *str - '0';
		if (sign > 0 && result > (LONG_MAX - digit) / 10)
			return (errno = ERANGE, LONG_MAX);
		if (sign < 0 && (-result) < (LONG_MIN + digit) / 10)
			return (errno = ERANGE, LONG_MIN);
		result = result * 10 + digit;
		str++;
	}
	return (sign * result);
}
