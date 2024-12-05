/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:39:35 by vzurera-          #+#    #+#             */
/*   Updated: 2024/01/18 13:39:35 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <limits.h>

void	ft_putchar_fd(int fd, char c) { write (fd, &c, 1); }
void	ft_putstr_fd(int fd, char *str) { if (fd > 0 && str) write(fd, str, ft_strlen(str)); }
void	ft_putendl_fd(int fd, char *str) { if (fd > 0 && str) { write(fd, str, ft_strlen(str)); write(fd, "\n", 1); }}

void	ft_putnbr_fd(int fd, int n) {
	if (n == INT_MIN) write(fd, "-2147483648", 11);
	else {
		if (n < 0) { write (fd, "-", 1); n = -n; }
		if (n >= 10) ft_putnbr_fd(n / 10, fd);
		int c = (n % 10) + '0';
		write(fd, &c, 1);
	}
}

int		ft_putstr_fdx(int fd, int count, ...) {
	va_list	args; va_start(args, count);
	char	*str, *result;

	if (!(result = ft_strdup(""))) return (0);
	while (--count >= 0) {
		str = va_arg (args, char *);
		if (str) result = ft_strjoin(result, str, 1);
		if (!result) return (0);
	}
	va_end(args);
	count = write(fd, result, ft_strlen(result));
	return (free(result), count);
}
