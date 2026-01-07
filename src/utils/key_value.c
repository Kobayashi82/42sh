/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_value.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 22:47:08 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/07 23:49:36 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "utils/utils.h"

#pragma endregion

#pragma region "Value"

	const char *get_value(const char *key, const char sep) {
		if (!key) return (NULL);

		char *delimiter = strchr(key, sep);
		if (!delimiter) return (NULL);

		return (ft_strdup(delimiter + 1));
	}

#pragma endregion

#pragma region "Key / Value"

	int get_key_value(const char *line, char **key, char **value, char sep) {
		if (*key)	{ free(*key); *key = NULL; }
		if (*value) { free(*value); *value = NULL; }
		if (!line) 	{ return (2); }

		char *delimiter = strchr(line, sep);
		if (!delimiter) {
			*key = ft_strdup(line);
			return (1);
		}

		size_t key_length = delimiter - line;
		*key = malloc(key_length + 1);

		strncpy(*key, line, key_length);
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

	// Escapes a string for safe use within shell quotes (' or ")
	char *format_for_shell(const char *value, char quote_type) {
		if (!value || (quote_type != '\'' && quote_type != '\"')) return (NULL);

		size_t length = ft_strlen(value), j = 0;
		char *escaped = malloc(length * 6 + 3);

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
