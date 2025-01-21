/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 13:50:43 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/21 21:56:13 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "terminal/terminal.h"
	#include "hashes/cmdp.h"

#pragma endregion

#pragma region "Variables"

	t_cmdp *cmdp_table[CMDP_HASH_SIZE];

#pragma endregion

#pragma region "Index"

	static unsigned int hash_index(const char *key) {
		unsigned int hash = 0;

		while (*key) hash = (hash * 31) + *key++;
		return (hash % CMDP_HASH_SIZE);
	}

#pragma endregion

#pragma region "Import"

	#pragma region "CMDP"

		int cmdp_add(const char *path, bool check_file, bool check_exec) {
			if (!path) return (1);

			if (check_exec && access(path, X_OK) == -1) return (1);
			if (check_file && access(path, F_OK) == -1) return (1);
			char *name = ft_strrchr(path, '/');
			if (name && *name) name++;
			if (ft_isspace_s(name)) return (1);

			t_cmdp *new_cmdp = cmdp_find(name, true);
			if (new_cmdp) {
				sfree(new_cmdp->path);
				new_cmdp->path = ft_strdup(path);

				return (0);
			}

			unsigned int index = hash_index(name);
			new_cmdp = ft_calloc(1, sizeof(t_cmdp));

			new_cmdp->name = ft_strdup(name);
			new_cmdp->path = ft_strdup(path);

			new_cmdp->next = cmdp_table[index];
			cmdp_table[index] = new_cmdp;

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Export"

	#pragma region "CMDP"

		t_cmdp *cmdp_find(const char *name, bool ninja) {
			if (!name) return (NULL);

			unsigned int index = hash_index(name);
			t_cmdp *cmdp = cmdp_table[index];

			while (cmdp) {
				if (!ft_strcmp(cmdp->name, name)) {
					if (!ninja) cmdp->hits++;
					return (cmdp);
				}
				cmdp = cmdp->next;
			}

			return (NULL);
		}

		char *cmdp_find_value(const char *name, bool ninja) {
			if (!name) return (NULL);

			unsigned int index = hash_index(name);
			t_cmdp *cmdp = cmdp_table[index];

			while (cmdp) {
				if (!ft_strcmp(cmdp->name, name)) {
					if (!ninja) cmdp->hits++;
					return (cmdp->path);
				}
				cmdp = cmdp->next;
			}

			return (NULL);
		}

	#pragma endregion

	#pragma region "Array"

		char **cmdp_to_array(bool sort) {
			size_t i = 0;

			for (unsigned int index = 0; index < CMDP_HASH_SIZE; ++index) {
				t_cmdp *cmdp = cmdp_table[index];
				while (cmdp) {
					if (cmdp->name && cmdp->path) i++;
					cmdp = cmdp->next;
				}
			}

			if (i == 0) return (NULL);
			char **array = smalloc((i + 1) * sizeof(char *));

			i = 0;
			for (unsigned int index = 0; index < CMDP_HASH_SIZE; ++index) {
				t_cmdp *cmdp = cmdp_table[index];
				while (cmdp) {
					if (cmdp->name && cmdp->path) {
						array[i] = ft_strdup(cmdp->path);
						i++;
					}
					cmdp = cmdp->next;
				}
			} array[i] = NULL;

			if (sort) array_sort(array);
			return (array);
		}

	#pragma endregion

	#pragma region "Print"

		int cmdp_print(bool sort) {
			char **array = cmdp_to_array(sort);

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

		size_t cmdp_length() {
			size_t i = 0;

			for (unsigned int index = 0; index < CMDP_HASH_SIZE; ++index) {
				t_cmdp *cmdp = cmdp_table[index];
				while (cmdp) {
					if (cmdp->name && cmdp->path) i++;
					cmdp = cmdp->next;
				}
			}

			return (i);
		}

	#pragma endregion

#pragma endregion

#pragma region "Delete"

	#pragma region "CMDP"

		int cmdp_delete(const char *name) {
			if (!name) return (1);

			unsigned int index = hash_index(name);
			t_cmdp *cmdp = cmdp_table[index];
			t_cmdp *prev = NULL;

			while (cmdp) {
				if (!ft_strcmp(cmdp->name, name)) {
					if (prev)	prev->next = cmdp->next;
					else		cmdp_table[index] = cmdp->next;
					sfree(cmdp->name); sfree(cmdp->path); sfree(cmdp);
					return (0);
				}
				prev = cmdp;
				cmdp = cmdp->next;
			}
			
			return (1);
		}

	#pragma endregion

	#pragma region "Clear"

		void cmdp_clear() {
			for (unsigned int index = 0; index < CMDP_HASH_SIZE; ++index) {
				if (cmdp_table[index]) {
					t_cmdp *cmdp = cmdp_table[index];
					while (cmdp) {
						t_cmdp *next = cmdp->next;
						sfree(cmdp->name);
						sfree(cmdp->path);
						sfree(cmdp);
						cmdp = next;
					}
					cmdp_table[index] = NULL;
				}
			}
		}

	#pragma endregion

#pragma endregion

#pragma region "Initialize"

	int cmdp_initialize() {
		return (0);
	}

#pragma endregion
