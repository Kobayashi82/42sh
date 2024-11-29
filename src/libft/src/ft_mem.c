/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mem.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:22:26 by vzurera-          #+#    #+#             */
/*   Updated: 2024/01/18 13:22:26 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, int n)
{
	unsigned char		*d;
	const unsigned char	*s;

	d = (unsigned char *)dst;
	s = (const unsigned char *)src;
	if (!d && !s && n)
		return (dst);
	if (!n)
		return (dst);
	while (n-- > 0)
		*d++ = *s++;
	return (dst);
}

void	*ft_memmove(void *dst, const void *src, int len)
{
	unsigned char		*d;
	const unsigned char	*s;

	d = (unsigned char *)dst;
	s = (const unsigned char *)src;
	if (!d && !s && len)
		return (dst);
	if (d < s)
		while (len-- > 0)
			*d++ = *s++;
	else
	{
		d += len - 1;
		s += len - 1;
		while (len-- > 0)
			*d-- = *s--;
	}
	return (dst);
}

int	ft_memcmp(const void *s1, const void *s2, int n)
{
	unsigned char	*str1;
	unsigned char	*str2;
	int				i;

	i = 0;
	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	while ((*str1 == *str2) && i < n - 1 && n > 0)
	{
		str1++;
		str2++;
		i++;
	}
	if ((!*str1 && !*str2) || i == n)
		return (0);
	return (*str1 - *str2);
}

void	*ft_memchr(const void *s, int c, int n)
{
	const unsigned char	*str;
	int					i;

	i = 0;
	str = (const unsigned char *)s;
	while (i < n)
	{
		if (str[i] == (unsigned char)c)
			return ((void *)&str[i]);
		i++;
	}
	return (NULL);
}

void	*ft_memset(void *b, int c, int len)
{
	unsigned char	*p;

	p = b;
	while (len--)
		*p++ = (unsigned char)c;
	return (b);
}
