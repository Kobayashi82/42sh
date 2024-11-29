/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_sub.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 20:09:18 by vzurera-          #+#    #+#             */
/*   Updated: 2024/03/28 23:54:54 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(const char *s, unsigned int start, int len)
{
	char	*new_str;
	int		m_len;

	if (start > (unsigned int)ft_strlen(s))
		m_len = 0;
	else
		m_len = (unsigned int)ft_strlen(s) - start;
	if (len > m_len)
		len = m_len;
	len += 1;
	new_str = (char *) malloc(len);
	if (!new_str)
		return (NULL);
	if (len > 1)
	{
		if (!ft_strlcpy(new_str, s + start, len))
			new_str[0] = '\0';
	}
	else
		new_str[0] = '\0';
	return (new_str);
}
