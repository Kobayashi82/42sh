/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 17:39:40 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/26 15:34:19 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "internal/shell.h"
	#include "utils/utils.h"

#pragma endregion

#pragma region "Index"

	static unsigned int hash_index(const char *key) {
		unsigned int hash = 0;

		while (*key) hash = (hash * 31) + *key++;
		return (hash % HASH_SIZE);
	}

#pragma endregion

#pragma region "Validate"

	int alias_validate(const char *key) {
		if (!key) return (1);

		size_t len = ft_strlen(key);

		if (!len || (!isalpha(key[0]) && key[0] != '_')) return (1);

		for (size_t i = 1; i < len; ++i)
			if (!isalnum(key[i]) && key[i] != '_') return (1);

		return (0);
	}

#pragma endregion

#pragma region "Get"

	t_alias *alias_get(const char *key) {
		shell.error = 0;
		if (!key) return (NULL);

		if (alias_validate(key)) return (shell.error = E_ALIAS_INVALID, NULL);

		unsigned int index = hash_index(key);
		t_alias *alias = shell.alias[index];

		while (alias) {
			if (!strcmp(alias->name, key)) return (alias);
			alias = alias->next;
		}

		return (NULL);
	}

	char *alias_get_value(const char *key) {
		if (!key) return (NULL);

		unsigned int index = hash_index(key);
		t_alias *alias = shell.alias[index];

		while (alias) {
			if (!strcmp(alias->name, key)) return (alias->value);
			alias = alias->next;
		}

		return (NULL);
	}

#pragma endregion

#pragma region "Add"

	int alias_add(const char *key, const char *value) {
		if (!key) return (1);

		if (alias_validate(key)) return (shell.error = E_ALIAS_INVALID, 1);

		t_alias *new_alias = alias_get(key);
		if (new_alias) {
			free(new_alias->value);
			if (value)	new_alias->value = ft_strdup(value);
			else		new_alias->value = ft_strdup("");
			if (!new_alias->value) return (shell.error = E_NO_MEMORY, 1);

			return (0);
		}

		unsigned int index = hash_index(key);
		new_alias = calloc(1, sizeof(t_alias));
		if (!new_alias) return (shell.error = E_NO_MEMORY, 1);

		new_alias->name = ft_strdup(key);
		new_alias->value = NULL;
		if (value)	new_alias->value = ft_strdup(value);
		else		new_alias->value = ft_strdup("");
		if (!new_alias->name || !new_alias->value) {
			free(new_alias->name);
			free(new_alias->value);
			free(new_alias);
			return (shell.error = E_NO_MEMORY, 1);
		}

		new_alias->next = shell.alias[index];
		shell.alias[index] = new_alias;

		return (0);
	}

#pragma endregion

#pragma region "Delete"

	#pragma region "Delete"

		int alias_delete(const char *key) {
			if (!key) return (1);

			unsigned int index = hash_index(key);
			t_alias *alias = shell.alias[index];
			t_alias *prev = NULL;

			while (alias) {
				if (!strcmp(alias->name, key)) {
					if (prev)	prev->next = alias->next;
					else		shell.alias[index] = alias->next;
					free(alias->name);
					free(alias->value);
					free(alias);
					return (0);
				}
				prev = alias;
				alias = alias->next;
			}
			
			return (1);
		}

	#pragma endregion

	#pragma region "Clear"

		void alias_clear() {
			for (int i = 0; i < HASH_SIZE; ++i) {
				if (shell.alias[i]) {
					t_alias *alias = shell.alias[i];
					while (alias) {
						t_alias *next = alias->next;
						free(alias->name);
						free(alias->value);
						free(alias);
						alias = next;
					}
					shell.alias[i] = NULL;
				}
			}
		}

	#pragma endregion

#pragma endregion

#pragma region "Print"

	int alias_print(int sort) {
		size_t count = shell.error = 0;

		for (unsigned int index = 0; index < HASH_SIZE; index++) {
			t_alias *alias = shell.alias[index];
			while (alias) {
				if (alias->name && alias->value) count++;
				alias = alias->next;
			}
		}

		if (!count) return (1);
		char **array = malloc((count + 1) * sizeof(char *));
		if (!array) return (shell.error = E_NO_MEMORY, 1);

		int i = 0;
		for (unsigned int index = 0; index < HASH_SIZE; ++index) {
			t_alias *alias = shell.alias[index];
			while (alias) {
				array[i] = ft_strjoin_sep("alias ", alias->name, "='", J_FREE_NONE);
				if (array[i]) array[i] = ft_strjoin_sep(array[i], alias->value, "'", J_FREE_VAL_1);
				if (!array[i]) {
					array_free(array);
					return (shell.error = E_NO_MEMORY, 1);
				}
				i++;
				alias = alias->next;
			}
		}
		array[i] = NULL;

		if (sort) array_nsort(array, sort, 6);

		if (array && array[0]) {

			print(STDOUT_FILENO, NULL, P_RESET);

			for (size_t i = 0; array[i]; ++i) {
				print(STDOUT_FILENO, array[i], P_JOIN);
				print(STDOUT_FILENO, "\n", P_JOIN);
			}

			print(STDOUT_FILENO, NULL, P_PRINT);
		}

		array_free(array);

		return (0);
	}

#pragma endregion
