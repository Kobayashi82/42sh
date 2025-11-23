/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pattern.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 13:35:55 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/23 11:57:53 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "utils/libft.h"
	#include "main/options.h"
	#include "expansion/globbing.h"

#pragma endregion

#pragma region "Pattern"

	#pragma region "Optimize"

		//	Delete duplicates of *
		static void pattern_optimize(char *pattern) {
			int i = 0;

			while (pattern && pattern[i]) {
				if (pattern[i] == '*' && pattern[i + 1] == '*')
					memmove(&pattern[i], &pattern[i + 1], ft_strlen(&pattern[i]));
				else i++;
			}
		}

	#pragma endregion

	#pragma region "Is Escaped"

		static bool pattern_isescaped(char *str, int i, char c) {
			int		j = 0;

			while (str && str[j] && j < i && i >= 0) {
				if (str[j] == '\\' && (!c || str[j + 1] == c)) {
					memmove(&str[j], &str[j + 1], ft_strlen(&str[j + 1]) + 1);
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

			char *start = pattern, *tmp_pattern = pattern;

			t_pattern *patterns = NULL, *tmp_list = NULL;

			while (tmp_pattern && *tmp_pattern) {
				char *value = NULL;
				bool is_dir = false;

				if ((value = strchr(tmp_pattern, '/'))) {

					if (pattern_isescaped(tmp_pattern, value - tmp_pattern, '/')) {
						tmp_pattern = value;
						continue;
					}

					if (value - start == 0) {
						tmp_pattern = start + 1;
						start = tmp_pattern;
						continue;
					}
					value = ft_substr(start, 0, value - start);
					tmp_pattern = start + ft_strlen(value) + 1;
					start = tmp_pattern;
					is_dir = true;

					if (!strcmp(value, "/") && tmp_list && tmp_list->value && *tmp_list->value && tmp_list->value[ft_strlen(tmp_list->value) - 1] == '/') { free(value); continue; }
					if (!strcmp(value, "**/") && tmp_list && tmp_list->value && !strcmp(tmp_list->value, "**/")) { free(value); continue; }
				} else if (*tmp_pattern) {
					value = ft_strdup(tmp_pattern);
					tmp_pattern = NULL;
				}

				pattern_optimize(value);
				if (value) {
					t_pattern *new_pattern = calloc(1, sizeof(t_pattern));
					new_pattern->value = value;
					new_pattern->is_dir = is_dir;

					if (!patterns) {
						patterns = new_pattern;
						tmp_list = patterns;
					} else {
						tmp_list->next = new_pattern;
						tmp_list = new_pattern;
					}
				}
			}

			return (free(pattern), patterns);
		}

	#pragma endregion

	#pragma region "Clear"

		void pattern_clear(t_pattern **patterns) {
			t_pattern *next, *current = *patterns;

			while (current) {
				next = current->next;
				free(current->value);
				free(current);
				current = next;
			} *patterns = NULL;
		}

	 #pragma endregion

 #pragma endregion
