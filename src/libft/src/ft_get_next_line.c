/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_next_line.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 02:19:22 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/06 00:21:36 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

#define GNL_BUFFER 256

static char	*get_line(char *line, char *newline, char *buffer) {
	int	len = ft_strlen(line);
	int	blen = newline - buffer;

	line = ft_realloc(line, ft_strlen(line), len + blen + 2);
	if (!line) return (NULL);

	ft_memcpy((line + len), buffer, blen);
	line[len + blen] = '\n';
	line[len + blen + 1] = '\0';
	ft_memcpy(buffer, newline + 1, ft_strlen(newline + 1) + 1);

	return (line);
}

static char	*get_buffer(char *line, char *buffer) {
	int	len = ft_strlen(line);
	int	blen = ft_strlen(buffer);

	line = ft_realloc(line, ft_strlen(line), len + blen + 1);
	if (!line) return (NULL);

	ft_memcpy(line + len, buffer, blen);
	line[len + blen] = '\0';
	ft_memset(buffer, '\0', GNL_BUFFER + 1);

	return (line);
}

char	*ft_get_next_line(int fd) {
	static char	buffer[GNL_BUFFER + 1];
	char		*newline, *line = NULL;

	if (fd < 0) return (NULL);
	while (1) {
		if (!buffer[0]) {
			ft_memset(buffer, '\0', GNL_BUFFER + 1);
			int readed = read(fd, buffer, GNL_BUFFER);
			if (readed == 0 && line)	return (line);
			if (readed < 1)				return (free(line), NULL);
		}
		newline = ft_memchr(buffer, '\n', ft_strlen(buffer));
		if (newline)					return (get_line(line, newline, buffer));
		line = get_buffer(line, buffer);
	}
}
