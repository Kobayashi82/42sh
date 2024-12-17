/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 17:39:40 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/17 19:25:32 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Variables

	t_alias *alias_table[HASH_SIZE];

#pragma endregion

#pragma region Import

	#pragma region Variable

		int alias_add(const char *name, const char *value) {
			if (!name) return (0);

			t_alias *new_alias = alias_find(name);
			if (new_alias) {
				free(new_alias->value);
				new_alias->value = ft_strdup(value);
				return (0);
			}

			unsigned int index = hash_index(name);
			new_alias = safe_malloc(sizeof(t_alias));

			new_alias->name = ft_strdup(name);
			new_alias->value = NULL;
			if (value) new_alias->value = ft_strdup(value);
			if (!new_alias->name || (value && !new_alias->value)) {
				free(new_alias->name); free(new_alias->value);
				exit_error(NO_MEMORY, 1, NULL, true);
			}

			new_alias->next = alias_table[index];
			alias_table[index] = new_alias;

			return (0);
		}

	#pragma endregion

	#pragma region Array

		void alias_from_array(const char **array) {
			if (!array) return;

			for (size_t i = 0; array[i]; i++) {
				char *key = NULL, *value = NULL;
				get_key_value(array[i], &key, &value, '=');
				if (!key) continue;
				alias_add(key, value);
				free(key); free(value);
			}
		}

#pragma endregion

#pragma endregion

#pragma region Export

	#pragma region Variable

		t_alias *alias_find(const char *name) {
			if (!name) return (NULL);

			unsigned int index = hash_index(name);
			t_alias *var = alias_table[index];

			while (var) {
				if (!ft_strcmp(var->name, name)) return (var);
				var = var->next;
			}

			return (NULL);
		}

	#pragma endregion

	#pragma region Array

		char **alias_to_array() {
			size_t i = 0;

			for (unsigned int index = 0; index < HASH_SIZE; index++) {
				t_alias *var = alias_table[index];
				while (var) {
					if (var->value) i++;
					var = var->next;
				}
			}

			if (i == 0) return (NULL);
			char **array = safe_malloc((i + 1) * sizeof(char *));

			i = 0;
			for (unsigned int index = 0; index < HASH_SIZE; index++) {
				t_alias *var = alias_table[index];
				while (var) {
					if (var->value) {
						size_t len = ft_strlen(var->name) + ft_strlen(var->value) + 2;
						array[i] = malloc(len);
						if (!array[i]) {
							for (size_t j = 0; j < i; j++) free(array[j]);
							free(array); exit_error(NO_MEMORY, 1, NULL, true);
						}

						ft_strcpy(array[i], var->name);
						if (var->value) {
							ft_strcat(array[i], "=");
							ft_strcat(array[i], var->value);
						}

						i++;
					}
					var = var->next;
				}
			} array[i] = NULL;

			array_sort(array);
			return (array);
		}

	#pragma endregion

#pragma endregion

#pragma region Delete

	#pragma region Variable

		int alias_delete(const char *name) {
			if (!name) return (1);

			unsigned int index = hash_index(name);
			t_alias *var = alias_table[index];
			t_alias *prev = NULL;

			while (var) {
				if (!ft_strcmp(var->name, name)) {
					if (prev)	prev->next = var->next;
					else		alias_table[index] = var->next;
					free(var->name); free(var->value); free(var);
					return (0);
				}
				prev = var;
				var = var->next;
			}
			
			return (1);
		}

	#pragma endregion

	#pragma region Clear

		void alias_clear() {
			for (unsigned int index = 0; index < HASH_SIZE; index++) {
				if (alias_table[index]) {
					t_alias *var = alias_table[index];
					while (var) {
						t_alias *next = var->next;
						free(var->name);
						free(var->value);
						free(var);
						var = next;
					}
					alias_table[index] = NULL;
				}
			}
		}

	#pragma endregion

#pragma endregion

#pragma region Initialize

	int alias_initialize() {
		return (0);
	}

#pragma endregion
