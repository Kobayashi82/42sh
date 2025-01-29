/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_value.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 22:47:08 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/29 16:35:30 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"
	#include "key_value.h"

#pragma endregion

#pragma region "Value"

	const char *get_value(const char *key, const char sep) {
		if (!key) return (NULL);

		char *delimiter = ft_strchr(key, sep);
		if (!delimiter) return (NULL);

		return (ft_strdup(delimiter + 1));
	}

#pragma endregion

#pragma region "Key / Value"

	int get_key_value(const char *line, char **key, char **value, char sep) {
		if (*key)	{ sfree(*key); *key = NULL; }
		if (*value) { sfree(*value); *value = NULL; }
		if (!line) 	{ return (2); }

		char *delimiter = ft_strchr(line, sep);
		if (!delimiter) {
			*key = ft_strdup(line);
			return (1);
		}

		size_t key_length = delimiter - line;
		*key = smalloc(key_length + 1);

		ft_strncpy(*key, line, key_length);
		(*key)[key_length] = '\0';

		*value = ft_strdup(delimiter + 1);

		size_t value_len = ft_strlen(*value);
		while (value_len > 0 && ((*value)[value_len - 1] == '\n' || (*value)[value_len - 1] == '\r')) {
			(*value)[--value_len] = '\0';
		}

		return (0);
	}

#pragma endregion

#pragma region "Format for Shell"

	char *format_for_shell(const char *value, char quote_type) {
		if (!value || (quote_type != '\'' && quote_type != '\"')) return (NULL);

		size_t length = ft_strlen(value), j = 0;
		char *escaped = smalloc(length * 6 + 3);

		if (quote_type == '\'') escaped[j++] = '\'';
		if (quote_type == '\"') escaped[j++] = '\"';

		for (size_t i = 0; i < length; i++) {
			if (quote_type == '\'' && value[i] == '\'') {
				escaped[j++] = '\'';
				escaped[j++] = '\\';
				escaped[j++] = '\'';
				escaped[j++] = '\'';
			} else if (quote_type == '\"' && value[i] == '\"') {
				escaped[j++] = '\\';
				escaped[j++] = '\"';
			} else if (quote_type == '\"' && (value[i] == '$' || value[i] == '`' || value[i] == '\\')) {
				escaped[j++] = '\\';
				escaped[j++] = value[i];
			} else escaped[j++] = value[i];
		}

		if (quote_type == '\'') escaped[j++] = '\'';
		if (quote_type == '\"') escaped[j++] = '\"';
		escaped[j] = '\0';

		return (escaped);
	}

#pragma endregion

#pragma region "Rejected"

	//	READ Y WRITE DE OPTIONS NO SE IMPLEMENTAN, PERO EL CODIGO PUEDE SER UTIL PARA LUEGO

	// //	Guarda las opciones en disco
	// int options_write() {
	// 	char *filename = ft_strjoin(get_home_path(), "/opciones", 0);

	// 	int fd = sopen(filename, O_CREAT | O_TRUNC | O_WRONLY, 0644); sfree(filename);
	// 	if (fd == -1) return (1);

	// 	print(fd, ft_strjoin_sep("input_mode=", ft_itoa(options.input_mode), "\n", 2), FREE_RESET);
	// 	print(fd, ft_strjoin_sep("hist_on=", ft_itoa(options.hist_on), "\n", 2), FREE_JOIN);
	// 	print(fd, ft_strjoin_sep("hist_local=", ft_itoa(options.hist_local), "\n", 2), FREE_PRINT);

	// 	sclose(fd);
	// 	return (0);
	// }

	// //	Lee las opciones del disco
	// int options_read() {
	// 	char *filename = ft_strjoin(get_home_path(), "/.42sh", 0);

	// 	options_default();
	// 	int fd = sopen(filename, O_RDONLY, -1); sfree(filename);
	// 	if (fd == -1) { options_write(); return (1); }

	// 	char *line = NULL, *key = NULL, *value = NULL;
	// 	while ((line = get_next_line(fd))) {
	// 		get_key_value(line, &key, &value, '=');
	// 		if (key && key[0] && value && value[0]) options_set(key, value);
	// 		sfree(line);
	// 	} sfree(key); sfree(value);

	// 	sclose(fd); options_write();

	// 	return (0);
	// }

#pragma endregion
