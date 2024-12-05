/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_op.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 19:19:06 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/06 00:21:02 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strcpy(char *dst, const char *src) {
	int	i = -1;

	if (!dst || !src) return (NULL);
	while (src[++i]) dst[i] = src[i];
	dst[i] = '\0';
	return (dst);
}

char	*ft_strncpy(char *dest, const char *src, int n) {
	int	i = -1;

	while (++i < n && src[i]) dest[i] = src[i];
	while (i < n) dest[i++] = '\0';
	return (dest);
}

int	ft_strlcpy(char *dst, const char *src, int dstsize) {
	int		i = 0, srclen = 0;

	if (!src) return (0);
	while (src[srclen]) ++srclen;
	if (dstsize > 0) {
		while (dstsize > 0 && src[i] && i < dstsize - 1) { dst[i] = src[i]; ++i; }
		dst[i] = '\0';
	}
	return (srclen);
}

char	*ft_strcat(char *dest, const char *src) {
	int dest_len = 0, i = -1;

	while (dest[dest_len]) dest_len++;
	while (src[++i]) dest[dest_len + i] = src[i];
	dest[dest_len + i] = '\0';
	return (dest);
}

char	*ft_strncat(char *dest, const char *src, int n) {
	int dest_len = 0, i = -1;

	while (dest[dest_len]) dest_len++;
	while (++i < n && src[i]) dest[dest_len + i] = src[i];
	dest[dest_len + i] = '\0';
	return (dest);
}

int	ft_strlcat(char *dst, const char *src, int dstsize) {
	int src_len = 0, dest_len = 0, i;

	while (dst[dest_len] && dest_len < dstsize) dest_len++;
	while (src[src_len]) src_len++;
	if (dstsize == 0 || dest_len >= dstsize) return (dest_len + src_len);
	i = dest_len - 1;
	while (++i < dstsize - 1 && src[i - dest_len]) dst[i] = src[i - dest_len];
	dst[i] = '\0';
	return (dest_len + src_len);
}

int	ft_strcmp(const char *s1, const char *s2) {
	if (!s1 || !s2) return (-1);
	while (*s1 && (*s1 == *s2)) { s1++; s2++; }
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

int	ft_strncmp(const char *s1, const char *s2, int n) {
	unsigned char	*str1, *str2;
	int				i = 0;

	if (!s1 || !s2) return (-1);
	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	while (*str1 && (*str1 == *str2) && i < n && n > 0) { ++str1; ++str2; ++i; }
	if ((!*str1 && !*str2) || i == n) return (0);
	return (*str1 - *str2);
}

int	ft_strlen(const char *str) {
	int	i = 0;

	while (str && str[i]) ++i;
	return (i);
}

int	ft_strlenl(const char *str) {
	int	lines = 0;

	if (str) lines++;
	while (str && *str) { if (*str++ == '\n') lines++; }
	return (lines);
}
