/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pattern.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 13:35:55 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/26 14:06:56 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"
	#include "main/options.h"
	#include "parser/globbing.h"

#pragma endregion

#pragma region "Pattern"

	#pragma region "Optimize"

		//	Delete duplicates of *
		static void pattern_optimize(char *pattern) {
			int i = 0;

			if (pattern && options.globstar && (!ft_strcmp(pattern, "**") || !ft_strcmp(pattern, "**/"))) return;
			while (pattern && pattern[i]) {
				if (pattern[i] == '*' && pattern[i + 1] == '*')
					ft_memmove(&pattern[i], &pattern[i + 1], ft_strlen(&pattern[i]));
				else i++;
			}
		}

	#pragma endregion

	#pragma region "Is Escaped"

		static bool pattern_isescaped(char *str, int i, char c) {
			int		j = 0;

			while (str && str[j] && j < i && i >= 0) {
				if (str[j] == '\\' && (!c || str[j + 1] == c)) {
					ft_memmove(&str[j], &str[j + 1], ft_strlen(&str[j + 1]) + 1);
					if (j == --i) return (true);
				} j++;
			}

			return (false);
		}

	#pragma endregion

	#pragma region "Create"
		
		t_pattern *pattern_create(char *pattern) {
			if (!pattern) return (NULL);
			pattern = ft_strdup(pattern);

			char *value, *start = pattern, *tmp_pattern = pattern;
			t_pattern *patterns = NULL, *tmp_list = NULL;

			while (tmp_pattern && *tmp_pattern) {
				if ((value = ft_strchr(tmp_pattern, '/'))) {

					if (pattern_isescaped(tmp_pattern, value - tmp_pattern, '/')) {
						tmp_pattern = value;
						continue;
					}

					value = ft_substr(start, 0, (value - start) + 1);
					tmp_pattern = start + ft_strlen(value);
					start = tmp_pattern;

					if (!ft_strcmp(value, "/") && tmp_list && tmp_list->value && *tmp_list->value && tmp_list->value[ft_strlen(tmp_list->value) - 1] == '/') { sfree(value); continue; }
					if (!ft_strcmp(value, "**/") && tmp_list && tmp_list->value && !ft_strcmp(tmp_list->value, "**/")) { sfree(value); continue; }
				} else if (*tmp_pattern) {
					value = ft_strdup(tmp_pattern);
					tmp_pattern = NULL;
				}

				pattern_optimize(value);
				if (value) {
					t_pattern *new_pattern = ft_calloc(1, sizeof(t_pattern));
					new_pattern->value = value;

					if (!patterns) {
						patterns = new_pattern;
						tmp_list = patterns;
					} else {
						tmp_list->next = new_pattern;
						tmp_list = new_pattern;
					}
				}
			}

			return (sfree(pattern), patterns);
		}

	#pragma endregion

	#pragma region "Clear"

		void pattern_clear(t_pattern **patterns) {
			t_pattern *next, *current = *patterns;

			while (current) {
				next = current->next;
				sfree(current->value);
				sfree(current);
				current = next;
			} *patterns = NULL;
		}

	 #pragma endregion

 #pragma endregion
