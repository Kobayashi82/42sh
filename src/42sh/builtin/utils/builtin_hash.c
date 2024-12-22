/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_hash.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 12:49:17 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/22 13:39:02 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Variables

	t_builtin *builtin_table[HASH_SIZE];

#pragma endregion

#pragma region Import

	#pragma region Builtin

		int builtin_add(const char *name, int disabled, int special, int (*execute)(t_arg *args)) {
			if (!name) return (0);

			t_builtin *new_builtin = builtin_find(name);
			if (new_builtin) {
				if (disabled != -1) new_builtin->disabled = disabled;
				if (special != -1) new_builtin->special = special;
				if (execute) new_builtin->execute = execute;
				return (0);
			}

			unsigned int index = hash_index(name);
			new_builtin = safe_calloc(1, sizeof(t_builtin));
			new_builtin->name = safe_strdup(name);
			if (disabled != -1) new_builtin->disabled = disabled;
			if (special != -1) new_builtin->special = special;
			if (execute) new_builtin->execute = execute;
			new_builtin->next = builtin_table[index];
			builtin_table[index] = new_builtin;

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region Export

	#pragma region Builtin

		t_builtin *builtin_find(const char *name) {
			if (!name) return (NULL);

			unsigned int index = hash_index(name);
			t_builtin *builtin = builtin_table[index];

			while (builtin) {
				if (!ft_strcmp(builtin->name, name)) return (builtin);
				builtin = builtin->next;
			}

			return (NULL);
		}

	#pragma endregion

	#pragma region Array

		char **builtin_to_array(bool sort) {
			size_t i = 0;

			for (unsigned int index = 0; index < HASH_SIZE; index++) {
				t_builtin *builtin = builtin_table[index];
				while (builtin) {
					if (builtin->name) i++;
					builtin = builtin->next;
				}
			}

			if (i == 0) return (NULL);
			char **array = safe_malloc((i + 1) * sizeof(char *));

			i = 0;
			for (unsigned int index = 0; index < HASH_SIZE; index++) {
				t_builtin *builtin = builtin_table[index];
				while (builtin) {				
					if (builtin->name) {
						array[i] = ft_strdup(builtin->name);
						if (!array[i]) {
							array_free(array);
							exit_error(NO_MEMORY, 1, NULL, true);
						}
						i++;
					}
					builtin = builtin->next;
				}
			} array[i] = NULL;

			if (sort) array_sort(array);
			return (array);
		}

	#pragma endregion

	#pragma region Print

		int builtin_print(bool sort) {
			char **array = builtin_to_array(sort);
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

		size_t builtin_length() {
			size_t i = 0;

			for (unsigned int index = 0; index < HASH_SIZE; index++) {
				t_builtin *builtin = builtin_table[index];
				while (builtin) {
					if (builtin->name) i++;
					builtin = builtin->next;
				}
			}

			return (i);
		}

	#pragma endregion

#pragma endregion

#pragma region Delete

	#pragma region Builtin

		int builtin_delete(const char *name) {
			if (!name) return (1);

			unsigned int index = hash_index(name);
			t_builtin *builtin = builtin_table[index];
			t_builtin *prev = NULL;

			while (builtin) {
				if (!ft_strcmp(builtin->name, name)) {
					if (prev)	prev->next = builtin->next;
					else		builtin_table[index] = builtin->next;
					free(builtin->name); free(builtin);
					return (0);
				}
				prev = builtin;
				builtin = builtin->next;
			}
			
			return (1);
		}

	#pragma endregion

	#pragma region Clear

		void builtin_clear() {
			for (unsigned int index = 0; index < HASH_SIZE; index++) {
				if (builtin_table[index]) {
					t_builtin *builtin = builtin_table[index];
					while (builtin) {
						t_builtin *next = builtin->next;
						free(builtin->name);
						free(builtin);
						builtin = next;
					}
					builtin_table[index] = NULL;
				}
			}
		}

	#pragma endregion

#pragma endregion

#pragma region Initialize

	int builtin_initialize() {
		builtin_add("alias", 0, 0, &alias);
		// builtin_add("bg", 0, 0, &bg);
		// builtin_add("builtin", 0, 0, &builtin);
		// builtin_add("cd", 0, 0, &cd);
		builtin_add("declare", 0, 0, &declare);
		// builtin_add("echo", 0, 0, &echo);
		// builtin_add("exec", 0, 0, &exec);
		// builtin_add("exit", 0, 0, &blt_exit);
		builtin_add("export", 0, 0, &export);
		// builtin_add("fc", 0, 0, &fc);
		// builtin_add("fg", 0, 0, &fg);
		// builtin_add("hash", 0, 0, &hash);
		// builtin_add("help", 0, 0, &help);
		// builtin_add("history", 0, 0, &history);
		// builtin_add("jobs", 0, 0, &jobs);
		// builtin_add("kill", 0, 0, &kill);
		// builtin_add("let", 0, 0, &let);
		// builtin_add("pwd", 0, 0, &pwd);
		builtin_add("readonly", 0, 0, &readonly);
		// builtin_add("set", 0, 0, &set);
		// builtin_add("test", 0, 0, &test);
		// builtin_add("type", 0, 0, &type);
		builtin_add("unalias", 0, 0, &unalias);
		builtin_add("unset", 0, 0, &unset);
		// builtin_add("wait", 0, 0, &wait);
		return (0);
	}

#pragma endregion
