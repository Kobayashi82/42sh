/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 02:19:22 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/18 22:59:21 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"

#pragma endregion

#pragma region "Get Next Line"

	#pragma region "Variables"

		#define GNL_BUFFER 256

	#pragma endregion

	#pragma region "Get Line"

		static char	*get_line(char *line, char *newline, char *buffer) {
			int	len = ft_strlen(line);
			int	blen = newline - buffer;

			line = realloc(line, len + blen + 2);
			if (!line) return (NULL);

			memcpy((line + len), buffer, blen);
			line[len + blen] = '\n';
			line[len + blen + 1] = '\0';
			memcpy(buffer, newline + 1, ft_strlen(newline + 1) + 1);

			return (line);
		}

	#pragma endregion

	#pragma region "Get Buffer"

		static char	*get_buffer(char *line, char *buffer) {
			int	len = ft_strlen(line);
			int	blen = ft_strlen(buffer);

			line = realloc(line, len + blen + 1);
			if (!line) return (NULL);

			memcpy(line + len, buffer, blen);
			line[len + blen] = '\0';
			memset(buffer, '\0', GNL_BUFFER + 1);

			return (line);
		}

	#pragma endregion

	#pragma region "Get Next Line"

		char	*get_next_line(int fd) {
			static char	buffer[GNL_BUFFER + 1];
			char		*newline, *line = NULL;

			if (fd < 0) {
				memset(buffer, '\0', GNL_BUFFER + 1);
				return (NULL);
			}

			while (1) {
				if (!buffer[0]) {
					memset(buffer, '\0', GNL_BUFFER + 1);
					int readed = read(fd, buffer, GNL_BUFFER);
					if (readed == 0 && line)	return (line);
					if (readed < 1) {
						memset(buffer, '\0', GNL_BUFFER + 1);
						free(line);
						return (NULL);
					}
				}
				newline = memchr(buffer, '\n', ft_strlen(buffer));
				if (newline)					return (get_line(line, newline, buffer));
				line = get_buffer(line, buffer);
			}
		}

	#pragma endregion

#pragma endregion