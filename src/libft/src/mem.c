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

#pragma region "Includes"

	#include "libft.h"

#pragma endregion

#pragma region "MEM"

	#pragma region "MEMCMP"

		int	ft_memcmp(const void *s1, const void *s2, int n) {
			unsigned char	*str1 = (unsigned char *)s1;
			unsigned char	*str2 = (unsigned char *)s2;
			int				i = 0;

			while ((*str1 == *str2) && i < n - 1 && n > 0) {
				str1++; str2++; i++;
			}

			if ((!*str1 && !*str2) || i == n) return (0);

			return (*str1 - *str2);
		}

	#pragma endregion

	#pragma region "MEMCPY"

		void	*ft_memcpy(void *dst, const void *src, int n) {
			unsigned char		*d = (unsigned char *)dst;
			const unsigned char	*s = (const unsigned char *)src;

			if (!n || (!d && !s && n)) return (dst);
			while (n-- > 0) *d++ = *s++;

			return (dst);
		}

	#pragma endregion

	#pragma region "MEMMOVE"

		void	*ft_memmove(void *dst, const void *src, int len) {
			unsigned char		*d = (unsigned char *)dst;
			const unsigned char	*s = (const unsigned char *)src;

			if (!d && !s && len) return (dst);
			if (d < s)
				while (len-- > 0) *d++ = *s++;
			else {
				d += len - 1;
				s += len - 1;
				while (len-- > 0) *d-- = *s--;
			}

			return (dst);
		}

	#pragma endregion

	#pragma region "MEMCHR"

		void	*ft_memchr(const void *s, int c, int n) {
			const unsigned char	*str = (const unsigned char *)s;
			int					i = -1;

			while (++i < n)
				if (str[i] == (unsigned char)c) return ((void *)&str[i]);

			return (NULL);
		}

	#pragma endregion

	#pragma region "MEMSET"

		void	*ft_memset(void *b, int c, int len) {
			unsigned char	*p = b;

			while (len--) *p++ = (unsigned char)c;

			return (b);
		}

	#pragma endregion

#pragma endregion

#pragma region "CALLOC"

	void	*ft_calloc(int count, int size) {
		int n = count * size - 1;

		char *nuller = malloc(count * size);
		if (!nuller) return (NULL);
		while (n >= 0) nuller[n--] = '\0';

		return ((void *)nuller);
	}

#pragma endregion

#pragma region "REALLOC"

	void	*ft_realloc(void *ptr, size_t old_size, size_t size) {
		size_t min_size;

		if (size == 0) return (free(ptr), NULL);

		//void *new_ptr = ft_calloc(1, size);
		void *new_ptr = malloc(size);

		if (ptr) {
			if (old_size < size)	min_size = old_size;
			else					min_size = size;

			ft_memcpy(new_ptr, ptr, min_size);
			free(ptr);
		}

		return (new_ptr);
	}

#pragma endregion
