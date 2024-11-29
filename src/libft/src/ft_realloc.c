/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 22:05:01 by vzurera-          #+#    #+#             */
/*   Updated: 2023/12/11 22:05:01 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_realloc_2(void *ptr, int size)
{
	void	*new_ptr;
	int		copy_size;

	if (size > 0)
	{
		new_ptr = malloc(size);
		if (new_ptr == NULL)
		{
			free(ptr);
			return (NULL);
		}
		if (ptr != NULL)
		{
			copy_size = size - 1;
			if (copy_size > ft_strlen(ptr))
				copy_size = ft_strlen(ptr);
			ft_memcpy(new_ptr, ptr, copy_size);
			free(ptr);
		}
		return (new_ptr);
	}
	free(ptr);
	return (NULL);
}

void	*ft_realloc(void *ptr, int old_size, int size)
{
	void	*new_ptr;
	int		min_size;

	if (size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = malloc(size);
	if (!new_ptr)
		return (NULL);
	if (ptr)
	{
		if (old_size < size)
			min_size = old_size;
		else
			min_size = size;
		ft_memcpy(new_ptr, ptr, min_size);
		free(ptr);
	}
	return (new_ptr);
}

void	*ft_calloc(int count, int size)
{
	char	*nuller;
	int		n;

	n = count * size - 1;
	nuller = (char *)malloc(count * size);
	if (!nuller)
		return (NULL);
	while (n >= 0)
		nuller[n--] = '\0';
	return ((void *)nuller);
}
