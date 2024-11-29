/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_next_line.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 02:19:22 by vzurera-          #+#    #+#             */
/*   Updated: 2024/03/29 16:10:47 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*get_nline(char *line, char *newline, char *buffer)
{
	int	len;
	int	blen;

	len = ft_strlen(line);
	blen = newline - buffer;
	line = ft_realloc_2(line, len + blen + 2);
	if (line == NULL)
		return (NULL);
	ft_memcpy((line + len), buffer, blen);
	line[len + blen] = '\n';
	line[len + blen + 1] = '\0';
	ft_memcpy(buffer, newline + 1, ft_strlen(newline + 1) + 1);
	return (line);
}

static char	*get_buffer(char *line, char *buffer)
{
	int	len;
	int	blen;

	len = ft_strlen(line);
	blen = ft_strlen(buffer);
	line = ft_realloc_2(line, len + blen + 1);
	if (line == NULL)
		return (NULL);
	ft_memcpy(line + len, buffer, blen);
	line[len + blen] = '\0';
	ft_memset(buffer, '\0', GNL_BUFFER + 1);
	return (line);
}

char	*ft_get_next_line(int fd)
{
	static char	buffer[GNL_BUFFER + 1];
	char		*line;
	char		*newline;
	int			readed;

	if (fd < 0)
		return (NULL);
	line = NULL;
	while (1)
	{
		if (!buffer[0])
		{
			ft_memset(buffer, '\0', GNL_BUFFER + 1);
			readed = read(fd, buffer, GNL_BUFFER);
			if (readed == 0 && line != NULL)
				return (line);
			if (readed < 1)
				return (free(line), NULL);
		}
		newline = ft_memchr(buffer, '\n', ft_strlen(buffer));
		if (newline != NULL)
			return (get_nline(line, newline, buffer));
		line = get_buffer(line, buffer);
	}
}
