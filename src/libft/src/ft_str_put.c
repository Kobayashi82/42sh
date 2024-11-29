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

void	ft_putchar_fd(char c, int fd)
{
	write (fd, &c, 1);
}

void	ft_putendl_fd(char *s, int fd)
{
	while (*s)
		write (fd, s++, 1);
	write (fd, "\n", 1);
}

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	write (fd, s, i);
}

int	ft_putstr_fdx(int fd, int count, ...)
{
	va_list	args;
	char	*str;
	char	*result;
	char	*temp;

	va_start(args, count);
	result = ft_strdup("");
	while (--count >= 0)
	{
		str = va_arg (args, char *);
		if (str != NULL)
		{
			temp = ft_strjoin(result, str, 0);
			free(result);
			result = temp;
		}
	}
	va_end(args);
	count = write(fd, result, ft_strlen(result));
	free(result);
	return (count);
}

void	ft_putnbr_fd(int n, int fd)
{
	int	c;

	if (n == -2147483648)
		write (fd, "-2147483648", 11);
	else
	{
		if (n < 0)
		{
			write (fd, "-", 1);
			n = -n;
		}
		if (n >= 10)
			ft_putnbr_fd(n / 10, fd);
		c = (n % 10) + '0';
		write (fd, &c, 1);
	}
}
