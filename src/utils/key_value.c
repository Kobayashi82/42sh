/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_value.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 22:47:08 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/21 21:21:50 by vzurera-         ###   ########.fr       */
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
	char *format_for_shell(const char *value) {
		if (!value) return (NULL);

		size_t length = ft_strlen(value);
		int needs_ansi = 0;

		for (size_t i = 0; i < length; i++) {
			unsigned char c = (unsigned char)value[i];
			if (c < 32 || c == 127) {
				needs_ansi = 1;
				break;
			}
		}

		if (needs_ansi) {
			size_t j = 0;
			char *escaped = malloc(length * 6 + 4);
			if (!escaped) return (NULL);

			escaped[j++] = '$';
			escaped[j++] = '\'';

			for (size_t i = 0; i < length; i++) {
				unsigned char c = (unsigned char)value[i];
				if (c == '\\') {
					escaped[j++] = '\\';
					escaped[j++] = '\\';
				} else if (c == '\'') {
					escaped[j++] = '\\';
					escaped[j++] = '\'';
				} else if (c == '\n') {
					escaped[j++] = '\\';
					escaped[j++] = 'n';
				} else if (c == '\t') {
					escaped[j++] = '\\';
					escaped[j++] = 't';
				} else if (c == '\r') {
					escaped[j++] = '\\';
					escaped[j++] = 'r';
				} else if (c == '\v') {
					escaped[j++] = '\\';
					escaped[j++] = 'v';
				} else if (c == '\f') {
					escaped[j++] = '\\';
					escaped[j++] = 'f';
				} else if (c == '\a') {
					escaped[j++] = '\\';
					escaped[j++] = 'a';
				} else if (c == '\b') {
					escaped[j++] = '\\';
					escaped[j++] = 'b';
				} else if (c == 27) {
					escaped[j++] = '\\';
					escaped[j++] = 'e';
				} else if (c < 32 || c == 127) {
					escaped[j++] = '\\';
					escaped[j++] = (char)('0' + ((c >> 6) & 7));
					escaped[j++] = (char)('0' + ((c >> 3) & 7));
					escaped[j++] = (char)('0' + (c & 7));
				} else {
					escaped[j++] = (char)c;
				}
			}

			escaped[j++] = '\'';
			escaped[j] = '\0';
			return (escaped);
		}

		size_t j = 0;
		char *escaped = malloc(length * 6 + 3);
		if (!escaped) return (NULL);

		escaped[j++] = '\"';

		for (size_t i = 0; i < length; i++) {
			if (value[i] == '\"') {
				escaped[j++] = '\\';
				escaped[j++] = '\"';
			} else if (value[i] == '$' || value[i] == '`' || value[i] == '\\') {
				escaped[j++] = '\\';
				escaped[j++] = value[i];
			} else escaped[j++] = value[i];
		}

		escaped[j++] = '\"';
		escaped[j] = '\0';

		return (escaped);
	}

#pragma endregion
