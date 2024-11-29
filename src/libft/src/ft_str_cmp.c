/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_cmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 19:19:06 by vzurera-          #+#    #+#             */
/*   Updated: 2024/03/25 22:02:03 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	if (!s1 || !s2)
		return (-1);
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

int	ft_strncmp(const char *s1, const char *s2, int n)
{
	unsigned char	*str1;
	unsigned char	*str2;
	int				i;

	if (!s1 || !s2)
		return (-1);
	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	i = 0;
	while (*str1 && (*str1 == *str2) && i < n && n > 0)
	{
		++str1;
		++str2;
		++i;
	}
	if ((!*str1 && !*str2) || i == n)
		return (0);
	return (*str1 - *str2);
}
