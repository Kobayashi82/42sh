/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 13:50:43 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/09 17:48:46 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "main/shell.h"
	#include "utils/utils.h"

#pragma endregion

#pragma region "Index"

	static unsigned int hash_index(const char *key) {
		unsigned int hash = 0;

		while (*key) hash = (hash * 31) + *key++;
		return (hash % HASH_SIZE);
	}

#pragma endregion

#pragma region "Import"

	#pragma region "CMDP"

		int hash_add(const char *path, int check_file, int check_exec) {
			if (!path) return (1);

			if (check_exec && access(path, X_OK) == -1) return (1);
			if (check_file && access(path, F_OK) == -1) return (1);
			char *name = strrchr(path, '/');
			if (name && *name) name++;
			if (ft_isspace_s(name)) return (1);

			t_hash *new_hash = hash_find(name, 1);
			if (new_hash) {
				free(new_hash->path);
				new_hash->path = ft_strdup(path);

				return (0);
			}

			unsigned int index = hash_index(name);
			new_hash = calloc(1, sizeof(t_hash));

			new_hash->name = ft_strdup(name);
			new_hash->path = ft_strdup(path);

			new_hash->next = shell.hash_table[index];
			shell.hash_table[index] = new_hash;

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Export"

	#pragma region "CMDP"

		t_hash *hash_find(const char *name, int ninja) {
			if (!name) return (NULL);

			unsigned int index = hash_index(name);
			t_hash *hash = shell.hash_table[index];

			while (hash) {
				if (!strcmp(hash->name, name)) {
					if (!ninja) hash->hits++;
					return (hash);
				}
				hash = hash->next;
			}

			return (NULL);
		}

		char *hash_find_value(const char *name, int ninja) {
			if (!name) return (NULL);

			unsigned int index = hash_index(name);
			t_hash *hash = shell.hash_table[index];

			while (hash) {
				if (!strcmp(hash->name, name)) {
					if (!ninja) hash->hits++;
					return (hash->path);
				}
				hash = hash->next;
			}

			return (NULL);
		}

	#pragma endregion

	#pragma region "Array"

		char **hash_to_array(int sort) {
			size_t i = 0;

			for (unsigned int index = 0; index < HASH_SIZE; ++index) {
				t_hash *hash = shell.hash_table[index];
				while (hash) {
					if (hash->name && hash->path) i++;
					hash = hash->next;
				}
			}

			if (i == 0) return (NULL);
			char **array = malloc((i + 1) * sizeof(char *));

			i = 0;
			for (unsigned int index = 0; index < HASH_SIZE; ++index) {
				t_hash *hash = shell.hash_table[index];
				while (hash) {
					if (hash->name && hash->path) {
						array[i] = ft_strdup(hash->path);
						i++;
					}
					hash = hash->next;
				}
			} array[i] = NULL;

			if (sort) array_sort(array, sort);
			return (array);
		}

	#pragma endregion

	#pragma region "Print"

		int hash_print(int sort) {
			char **array = hash_to_array(sort);

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

	#pragma region "Length"

		size_t hash_length() {
			size_t i = 0;

			for (unsigned int index = 0; index < HASH_SIZE; ++index) {
				t_hash *hash = shell.hash_table[index];
				while (hash) {
					if (hash->name && hash->path) i++;
					hash = hash->next;
				}
			}

			return (i);
		}

	#pragma endregion

#pragma endregion

#pragma region "Delete"

	#pragma region "CMDP"

		int hash_delete(const char *name) {
			if (!name) return (1);

			unsigned int index = hash_index(name);
			t_hash *hash = shell.hash_table[index];
			t_hash *prev = NULL;

			while (hash) {
				if (!strcmp(hash->name, name)) {
					if (prev)	prev->next = hash->next;
					else		shell.hash_table[index] = hash->next;
					free(hash->name); free(hash->path); free(hash);
					return (0);
				}
				prev = hash;
				hash = hash->next;
			}
			
			return (1);
		}

	#pragma endregion

	#pragma region "Clear"

		void hash_clear() {
			for (unsigned int index = 0; index < HASH_SIZE; ++index) {
				if (shell.hash_table[index]) {
					t_hash *hash = shell.hash_table[index];
					while (hash) {
						t_hash *next = hash->next;
						free(hash->name);
						free(hash->path);
						free(hash);
						hash = next;
					}
					shell.hash_table[index] = NULL;
				}
			}
		}

	#pragma endregion

#pragma endregion

#pragma region "Initialize"

	int hash_initialize() {
		return (0);
	}

#pragma endregion
