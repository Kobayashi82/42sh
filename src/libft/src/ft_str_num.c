/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_num_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 21:33:44 by vzurera-          #+#    #+#             */
/*   Updated: 2024/03/19 21:33:44 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

int		ft_max(int n1, int n2) { if (n1 > n2) { return (n1); } return (n2); }
int		ft_min(int n1, int n2) { if (n1 < n2) { return (n1); } return (n2); }
int		ft_abs(int n) { if (n < 0) { n = -n; } return (n); }
void	*ft_max_ptr(void *s1, void *s2) { if (s1 < s2) { return (s2); } return (s1); }

int		ft_atoi(const char *str) {
	int sign = 1, num = 0;

	if (!str) return (0);
	while ((*str >= 9 && *str <= 13) || *str == 32) ++str;
	if ((*str == '+' || *str == '-') && *str++ == '-') sign = -sign;
	while (*str >= '0' && *str <= '9') num = (num * 10) + (*str++ - '0');
	return (num * sign);
}

long	ft_atol(char *str) {
	long result = 0;
	int sign = 1, num = 0;

	while (str && ft_isspace(*str)) str++;
	if (str && (*str == '-' || *str == '+') && *str++ == '-') sign = -1;
	while (str && ft_isdigit(*str)) {
		num = *str++ - '0';
		if (sign > 0 && result > (LONG_MAX - num) / 10)		return (errno = ERANGE, LONG_MAX);
		if (sign < 0 && (-result) < (LONG_MIN + num) / 10)	return (errno = ERANGE, LONG_MIN);
		result = result * 10 + num;
	}
	return (sign * result);
}

char	*ft_itoa(int n) {
	char *str;
	int len = 0, sign = 1;

	if (n == 0) return (ft_strdup("0"));
	if (n < 0) { sign = -1; len++; }
	while (n != 0) { n /= 10; len++; }
	str = malloc((len + 1));
	if (!str) return (NULL);
	str[len] = '\0';
	while (n != 0) {
		str[--len] = '0' + (sign * (n % 10));
		n /= 10;
	}
	if (sign == -1) str[0] = '-';
	return (str);
}
