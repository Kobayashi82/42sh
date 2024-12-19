/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 17:39:40 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/19 13:40:46 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Variables

	t_alias *alias_table[HASH_SIZE];

#pragma endregion

#pragma region Import

	#pragma region Alias

		int alias_add(const char *key, const char *value) {
			if (!key) return (0);

			t_alias *new_alias = alias_find(key);
			if (new_alias) {
				free(new_alias->value);
				if (value)	new_alias->value = ft_strdup(value);
				else		new_alias->value = ft_strdup("");

				return (0);
			}

			unsigned int index = hash_index(key);
			new_alias = safe_malloc(sizeof(t_alias));

			new_alias->name = ft_strdup(key);
			new_alias->value = NULL;
			if (value)	new_alias->value = ft_strdup(value);
			else		new_alias->value = ft_strdup("");
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

	#pragma region Validate

		int alias_validate(char *key, bool show_msg) {
			if (!key) return (0);
			int result = 0;

			size_t len = ft_strlen(key);

			if (!len || (!ft_isalpha(key[0]) && key[0] != '_')) result = 1;

			for (size_t i = 1; i < len; ++i)
				if (!ft_isalnum(key[i]) && key[i] != '_') { result = 1; break; }

			if (result && show_msg) {
				print(STDERR_FILENO, ft_strjoin("alias: `", key, 0), FREE_RESET);
				print(STDERR_FILENO, "': invalid alias name\n", PRINT);
			}

			return (result);
		}

	#pragma endregion

#pragma endregion

#pragma region Export

	#pragma region Alias

		t_alias *alias_find(const char *key) {
			if (!key) return (NULL);

			unsigned int index = hash_index(key);
			t_alias *alias = alias_table[index];

			while (alias) {
				if (!ft_strcmp(alias->name, key)) return (alias);
				alias = alias->next;
			}

			return (NULL);
		}

	#pragma endregion

	#pragma region Array

		char **alias_to_array(bool sort) {
			size_t i = 0;

			for (unsigned int index = 0; index < HASH_SIZE; index++) {
				t_alias *alias = alias_table[index];
				while (alias) {
					if (alias->name && alias->value) i++;
					alias = alias->next;
				}
			}

			if (i == 0) return (NULL);
			char **array = safe_malloc((i + 1) * sizeof(char *));

			i = 0;
			for (unsigned int index = 0; index < HASH_SIZE; index++) {
				t_alias *alias = alias_table[index];
				while (alias) {
					
					if (alias->name && alias->value) {
						array[i] = ft_strjoin_sep(alias->name, "=", alias->value, 0);
						if (!array[i]) {
							array_free(array);
							exit_error(NO_MEMORY, 1, NULL, true);
						}
						i++;
					}
					alias = alias->next;
				}
			} array[i] = NULL;

			if (sort) array_sort(array);
			return (array);
		}

	#pragma endregion

	#pragma region Print

		int alias_print(bool sort) {
			size_t i = 0;

			for (unsigned int index = 0; index < HASH_SIZE; index++) {
				t_alias *alias = alias_table[index];
				while (alias) {
					if (alias->name && alias->value) i++;
					alias = alias->next;
				}
			}

			if (i == 0) return (1);
			char **array = safe_malloc((i + 1) * sizeof(char *));

			i = 0;
			for (unsigned int index = 0; index < HASH_SIZE; index++) {
				t_alias *alias = alias_table[index];
				while (alias) {
					
					if (alias->name) {
						array[i] = ft_strjoin("alias ", alias->name, 0);
						if (array[i]) array[i] = ft_strjoin_sep(array[i], "=", format_for_shell(alias->value, '\''), 6);
						if (!array[i]) {
							array_free(array);
							exit_error(NO_MEMORY, 1, NULL, true);
						}
						i++;
					}
					alias = alias->next;
				}
			} array[i] = NULL;

			if (sort) array_nsort(array, 6);

			if (array && array[0]) {
				print(STDOUT_FILENO, NULL, RESET);
				for (size_t i = 0; array[i]; ++i) {
					print(STDOUT_FILENO, array[i], JOIN);
					print(STDOUT_FILENO, "\n", JOIN);
				}
				print(STDOUT_FILENO, NULL, PRINT);
			}
			if (array) array_free(array);

			return (0);
		}

	#pragma endregion

	#pragma region Length

		size_t alias_length() {
			size_t i = 0;

			for (unsigned int index = 0; index < HASH_SIZE; index++) {
				t_alias *alias = alias_table[index];
				while (alias) {
					if (alias->name) i++;
					alias = alias->next;
				}
			}

			return (i);
		}

	#pragma endregion

#pragma endregion

#pragma region Delete

	#pragma region Alias

		int alias_delete(const char *key) {
			if (!key) return (1);

			unsigned int index = hash_index(key);
			t_alias *alias = alias_table[index];
			t_alias *prev = NULL;

			while (alias) {
				if (!ft_strcmp(alias->name, key)) {
					if (prev)	prev->next = alias->next;
					else		alias_table[index] = alias->next;
					free(alias->name); free(alias->value); free(alias);
					return (0);
				}
				prev = alias;
				alias = alias->next;
			}
			
			return (1);
		}

	#pragma endregion

	#pragma region Clear

		void alias_clear() {
			for (unsigned int index = 0; index < HASH_SIZE; index++) {
				if (alias_table[index]) {
					t_alias *alias = alias_table[index];
					while (alias) {
						t_alias *next = alias->next;
						free(alias->name);
						free(alias->value);
						free(alias);
						alias = next;
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
