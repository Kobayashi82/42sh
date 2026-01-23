/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 13:50:43 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/23 21:41:06 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "internal/shell.h"
	#include "utils/utils.h"

#pragma endregion

#pragma region "Index"

	static unsigned int hash_index(const char *key) {
		unsigned int hash = 0;

		while (key && *key) hash = (hash * 31) + *key++;
		return (hash % HASH_SIZE);
	}

#pragma endregion

#pragma region "Get"

	t_hash *hash_find(const char *name, int no_hit) {
		if (!name) return (NULL);

		t_hash *hash = shell.hash[hash_index(name)];
		for (; hash; hash = hash->next) {
			if (!strcmp(hash->name, name)) {
				if (!no_hit) hash->hits++;
				return (hash);
			}
		}

		return (NULL);
	}

	char *hash_get(const char *name, int no_hit) {
		t_hash *hash = hash_find(name, no_hit);

		return ((hash) ? hash->path : NULL);
	}

#pragma endregion

#pragma region "Add"

	int hash_add_path(const char *name, const char *path) {
		shell.error = 0;
		if (!name || !path) return (1);

		t_hash *hash = hash_find(name, 1);
		if (hash) {
			free(hash->path);
			hash->path = ft_strdup(path);
			if (!hash->path) {
				hash_delete(name);
				return (shell.error = E_NO_MEMORY, 1);
			}

			return (0);
		}

		t_hash *new_hash = calloc(1, sizeof(t_hash));
		if (!new_hash) return (shell.error = E_NO_MEMORY, 1);
		
		new_hash->name = ft_strdup(name);
		new_hash->path = ft_strdup(path);
		if (!new_hash->name || !new_hash->path) {
			free(new_hash->name);
			free(new_hash->path);
			free(new_hash);
			return (shell.error = E_NO_MEMORY, 1);
		}
		
		unsigned int index = hash_index(name);
		new_hash->next = shell.hash[index];
		shell.hash[index] = new_hash;

		return (0);
	}

	int hash_add(const char *path) {
		shell.error = 0;
		if (!path) return (1);

		char *final_path = path_find_first(path, NULL);
		if (!final_path) return (shell.error = E_HASH_NOT_FOUND, 1);

		if (access(final_path, X_OK) == -1 || is_directory(final_path)) {
			free(final_path);
			return (shell.error = E_HASH_NOT_FOUND, 1);
		}

		char *name = strrchr(final_path, '/');
		if (!name || !*(name++)) {
			free(final_path);
			return (1);
		}

		t_hash *hash = hash_find(name, 1);
		if (hash) {
			free(hash->path);
			hash->path = final_path;
			return (0);
		}

		t_hash *new_hash = calloc(1, sizeof(t_hash));
		if (!new_hash) {
			free(final_path);
			return (shell.error = E_NO_MEMORY, 1);
		}
		
		new_hash->name = ft_strdup(name);
		new_hash->path = final_path;
		if (!new_hash->name) {
			free(new_hash->name);
			free(new_hash->path);
			free(new_hash);
			return (shell.error = E_NO_MEMORY, 1);
		}
		
		unsigned int index = hash_index(name);
		new_hash->next = shell.hash[index];
		shell.hash[index] = new_hash;

		return (0);
	}

#pragma endregion

#pragma region "Delete"

	#pragma region "Delete"

		int hash_delete(const char *name) {
			if (!name) return (1);

			unsigned int index = hash_index(name);
			t_hash *hash = shell.hash[index];
			t_hash *prev = NULL;

			while (hash) {
				if (!strcmp(hash->name, name)) {
					if (prev)	prev->next = hash->next;
					else		shell.hash[index] = hash->next;
					free(hash->name);
					free(hash->path);
					free(hash);
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
				if (shell.hash[index]) {
					t_hash *hash = shell.hash[index];
					while (hash) {
						t_hash *next = hash->next;
						free(hash->name);
						free(hash->path);
						free(hash);
						hash = next;
					}
					shell.hash[index] = NULL;
				}
			}
		}

	#pragma endregion

#pragma endregion

#pragma region "Print"

	#pragma region "Contains Spaces"

		static int contains_spaces(const char *value) {
			if (!value) return (0);

			for (const char *c = value; *c; ++c) {
				if (isspace((unsigned char)*c)) return (1);
			}

			return (0);
		}

	#pragma endregion

	#pragma region "Max Name Len"

		static int hash_max_name_len() {
			int max_len = 0;

			for (unsigned int index = 0; index < HASH_SIZE; ++index) {
				t_hash *hash = shell.hash[index];
				for (; hash; hash = hash->next) {
					if (hash->name) {
						int len = ft_strlen(hash->name);
						if (len > max_len) max_len = len;
					}
				}
			}

			return (max_len);
		}

	#pragma endregion

	#pragma region "Print"

		int hash_print(char *name, int reusable, int print_name) {
			int printed = shell.error = 0;
			print(STDOUT_FILENO, NULL, P_RESET);

			if (name) {
				if (reusable) {
					t_hash *hash = hash_find(name, 1);
					if (hash) {
						print(STDOUT_FILENO, "hash -p ", P_JOIN);
						if (contains_spaces(hash->path))	print(STDOUT_FILENO, ft_strjoin_sep("'", hash->path, "'", J_FREE_NONE), P_FREE_JOIN);
						else								print(STDOUT_FILENO, hash->path, P_JOIN);
						print(STDOUT_FILENO, " ", P_JOIN);
						if (contains_spaces(hash->name))	print(STDOUT_FILENO, ft_strjoin_sep("'", hash->name, "'", J_FREE_NONE), P_FREE_JOIN);
						else								print(STDOUT_FILENO, hash->name, P_JOIN);
						print(STDOUT_FILENO, "\n", P_JOIN);
						printed = 1;
					}
				} else {
					t_hash *hash = hash_find(name, 1);
					if (hash) {
						if (print_name) {
							int max_len = hash_max_name_len();
							int len = ft_strlen(hash->name);
							int spaces = ((max_len > len) ? (max_len - len) : 0) + 4;

							print(STDOUT_FILENO, hash->name, P_JOIN);
							while (spaces--) print(STDOUT_FILENO, " ", P_JOIN);
							print(STDOUT_FILENO, hash->path, P_JOIN);
							print(STDOUT_FILENO, "\n", P_JOIN);
						} else {
							print(STDOUT_FILENO, hash->path, P_JOIN);
							print(STDOUT_FILENO, "\n", P_JOIN);
						}
						printed = 1;
					}
				}
			} else {
				if (reusable) {
					for (unsigned int index = 0; index < HASH_SIZE; ++index) {
						t_hash *hash = shell.hash[index];
						for (; hash; hash = hash->next) {
							print(STDOUT_FILENO, "hash -p ", P_JOIN);
							if (contains_spaces(hash->path))	print(STDOUT_FILENO, ft_strjoin_sep("'", hash->path, "'", J_FREE_NONE), P_FREE_JOIN);
							else								print(STDOUT_FILENO, hash->path, P_JOIN);
							print(STDOUT_FILENO, " ", P_JOIN);
							if (contains_spaces(hash->name))	print(STDOUT_FILENO, ft_strjoin_sep("'", hash->name, "'", J_FREE_NONE), P_FREE_JOIN);
							else								print(STDOUT_FILENO, hash->name, P_JOIN);
							print(STDOUT_FILENO, "\n", P_JOIN);
							printed = 1;
						}
					}
				} else {
					int title = 0;
					for (unsigned int index = 0; index < HASH_SIZE; ++index) {
						t_hash *hash = shell.hash[index];
						for (; hash; hash = hash->next) {
							if (hash->name && hash->path) {
								if (!title) {
									title = 1;
									print(STDOUT_FILENO, "hits    command\n", P_JOIN);
								}
								char hit_buf[20];
								snprintf(hit_buf, sizeof(hit_buf), "%4d    ", hash->hits);
								print(STDOUT_FILENO, hit_buf, P_JOIN);
								print(STDOUT_FILENO, hash->path, P_JOIN);
								print(STDOUT_FILENO, "\n", P_JOIN);
								printed = 1;
							}
						}
					}
				}
			}

			if (printed) {
				print(STDOUT_FILENO, NULL, P_PRINT);
				return (0);
			}
				
			return (1);
		}

	#pragma endregion

#pragma endregion
