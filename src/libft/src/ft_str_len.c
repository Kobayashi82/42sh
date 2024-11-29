/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 15:57:55 by vzurera-          #+#    #+#             */
/*   Updated: 2024/01/11 17:46:05 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s && s[i])
		++i;
	return (i);
}

int	ft_strlenl(char *str)
{
	int	n_lines;

	n_lines = 1;
	while (str && *str)
		if (*str++ == '\n')
			n_lines++;
	return (n_lines);
}
