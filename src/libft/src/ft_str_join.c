/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_join.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 20:55:10 by vzurera-          #+#    #+#             */
/*   Updated: 2024/04/13 21:10:39 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char *s1, char *s2, int frees)
{
	char	*new_str;
	int		len;

	len = ft_strlen(s1) + ft_strlen(s2) + 1;
	new_str = malloc(len);
	if (!new_str || !s1)
	{
		if (new_str)
			free(new_str);
		if (s1 && (frees == 1 || frees == 3))
			free(s1);
		if (s2 && (frees == 2 || frees == 3))
			free(s2);
		return (NULL);
	}
	ft_strlcpy (new_str, s1, len);
	if (s2)
		ft_strlcat(new_str, s2, len);
	if (s1 && (frees == 1 || frees == 3))
		free(s1);
	if (s2 && (frees == 2 || frees == 3))
		free(s2);
	return (new_str);
}

char	*ft_strjoin_c(char *s1, char *s2, char *c, int frees)
{
	char	*new_str;
	int		len;

	len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(c) + 1;
	new_str = malloc(len);
	if (!new_str || !s1)
	{
		if (new_str)
			free(new_str);
		if (s1 && (frees == 1 || frees == 3))
			free(s1);
		if (s2 && (frees == 2 || frees == 3))
			free(s2);
		return (NULL);
	}
	ft_strlcpy (new_str, s1, len);
	ft_strlcat(new_str, c, len);
	if (s2)
		ft_strlcat(new_str, s2, len);
	if (s1 && (frees == 1 || frees == 3))
		free(s1);
	if (s2 && (frees == 2 || frees == 3))
		free(s2);
	return (new_str);
}
