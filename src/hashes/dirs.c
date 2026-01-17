/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dirs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 10:43:27 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/17 14:22:52 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "main/shell.h"
	#include "utils/utils.h"

#pragma endregion

#pragma region "Add"

	#pragma region "Rotate"

		char *dirs_rotate(int n) {
			(void) n;
		}

	#pragma endregion

	#pragma region "Add"

		char *dirs_add(char *path) {
			if (!path) return (NULL);
			
			errno = 0;
			char *fullpath = (path[0] == '/') ? ft_strdup(path) : ft_strjoin_sep(shell.dirs.cwd, "/", path, 0);
			if (!fullpath) {
				errno = E_NO_MEMORY;
				return (exit_error(E_NO_MEMORY, 1, "pushd", NULL, EE_FREE_NONE, EE_RETURN), NULL);
			}

			fullpath = normalize_path(fullpath, J_FREE_VAL_1);
			if (!fullpath) return (NULL);

			t_dir_stack *new_dir = malloc(sizeof(t_dir_stack));
			if (!new_dir) {
				free(fullpath);
				errno = E_NO_MEMORY;
				return (exit_error(E_NO_MEMORY, 1, "pushd", NULL, EE_FREE_NONE, EE_RETURN), NULL);
			}

			new_dir->path = fullpath;
			new_dir->prev = NULL;
			new_dir->next = shell.dirs.stack;

			if (shell.dirs.stack) shell.dirs.stack->prev = new_dir;
			shell.dirs.stack = new_dir;

			return (fullpath);
		}

	#pragma endregion

#pragma endregion

#pragma region "Delete"

	#pragma region "Delete"

		void dirs_remove(int n) {
			(void) n;
		}

	#pragma endregion

	#pragma region "Clear"

		void dirs_clear() {
		}

	#pragma endregion

#pragma endregion

#pragma region "Initialize"

	void dirs_print(int n, int line_mode, int no_tilde) {
		(void) n;
		(void) line_mode;
		(void) no_tilde;
	}

#pragma endregion

#pragma region "Initialize"

	void dirs_initialize() {
		
	}

#pragma endregion
