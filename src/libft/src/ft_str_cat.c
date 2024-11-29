/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_cat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 17:25:58 by vzurera-          #+#    #+#             */
/*   Updated: 2024/01/18 14:41:29 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strcat(char *dest, const char *src)
{
	int	dest_len;
	int	i;

	dest_len = 0;
	i = -1;
	while (dest[dest_len])
		dest_len++;
	while (src[++i])
		dest[dest_len + i] = src[i];
	dest[dest_len + i] = '\0';
	return (dest);
}

char	*ft_strncat(char *dest, const char *src, int n)
{
	int	dest_len;
	int	i;

	dest_len = 0;
	i = -1;
	while (dest[dest_len])
		dest_len++;
	while (++i < n && src[i])
		dest[dest_len + i] = src[i];
	dest[dest_len + i] = '\0';
	return (dest);
}

int	ft_strlcat(char *dst, const char *src, int dstsize)
{
	int	src_len;
	int	dest_len;
	int	i;

	src_len = 0;
	dest_len = 0;
	while (dst[dest_len] && dest_len < dstsize)
		dest_len++;
	while (src[src_len])
		src_len++;
	if (dstsize == 0 || dest_len >= dstsize)
		return (dest_len + src_len);
	i = dest_len;
	while (i < dstsize - 1 && src[i - dest_len])
	{
		dst[i] = src[i - dest_len];
		i++;
	}
	dst[i] = '\0';
	return (dest_len + src_len);
}
