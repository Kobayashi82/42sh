/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   varios.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 22:47:08 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/11 15:50:10 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Value

	const char *get_value(const char *key) {
		if (!key) return (NULL);

		char *delimiter = ft_strchr(key, '=');
		if (!delimiter) return (NULL);

		return (safe_strdup(delimiter + 1));
	}

#pragma endregion

#pragma region Key / Value

	int get_key_value(const char *line, char **key, char **value, char sep) {
		if (*key)	{ free(*key); *key = NULL; }
		if (*value) { free(*value); *value = NULL; }
		if (!line) 	{ return (2); }

		char *delimiter = ft_strchr(line, sep);
		if (!delimiter) {
			*key = safe_strdup(line);
			return (1);
		}

		size_t key_length = delimiter - line;
		*key = safe_malloc(key_length + 1);

		ft_strncpy(*key, line, key_length);
		(*key)[key_length] = '\0';

		*value = safe_strdup(delimiter + 1);

		size_t value_len = ft_strlen(*value);
		while (value_len > 0 && ((*value)[value_len - 1] == '\n' || (*value)[value_len - 1] == '\r')) {
			(*value)[--value_len] = '\0';
		}

		return (0);
	}

#pragma endregion

#pragma region Home Path

	char *home_path(const char *user) {
		char *path = ".";

		if (user) {
			return (path);
		}

		return (path);
	}

#pragma endregion

#pragma region Rejected

	//	READ Y WRITE DE OPTIONS NO SE IMPLEMENTAN, PERO EL CODIGO PUEDE SER UTIL PARA LUEGO

	// //	Guarda las opciones en disco
	// int options_write() {
	// 	char *filename = ft_strjoin(get_home_path(), "/opciones", 0);

	// 	int fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0644); free(filename);
	// 	if (fd == -1) return (1);

	// 	print(fd, ft_strjoin_sep("input_mode=", ft_itoa(options.input_mode), "\n", 2), FREE_RESET);
	// 	print(fd, ft_strjoin_sep("hist_on=", ft_itoa(options.hist_on), "\n", 2), FREE_JOIN);
	// 	print(fd, ft_strjoin_sep("hist_local=", ft_itoa(options.hist_local), "\n", 2), FREE_PRINT);

	// 	close(fd);
	// 	return (0);
	// }

	// //	Lee las opciones del disco
	// int options_read() {
	// 	char *filename = ft_strjoin(get_home_path(), "/.42sh", 0);

	// 	options_default();
	// 	int fd = open(filename, O_RDONLY); free(filename);
	// 	if (fd == -1) { options_write(); return (1); }

	// 	char *line = NULL, *key = NULL, *value = NULL;
	// 	while ((line = ft_get_next_line(fd))) {
	// 		get_key_value(line, &key, &value, '=');
	// 		if (key && key[0] && value && value[0]) options_set(key, value);
	// 		free(line);
	// 	} free(key); free(value);

	// 	close(fd); options_write();

	// 	return (0);
	// }

#pragma endregion
