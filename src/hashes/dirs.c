/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dirs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 10:43:27 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/17 19:09:41 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "main/shell.h"
	#include "utils/utils.h"

#pragma endregion

#pragma region "Add"

	#pragma region "Rotate"

		char *dirs_rotate(int offset) {
			(void) offset;
			return (NULL);
		}

	#pragma endregion

	#pragma region "Add"

		char *dirs_add(char *path) {
			if (!path) return (NULL);
			
			errno = 0;
			char *fullpath = (path[0] == '/') ? ft_strdup(path) : ft_strjoin_sep(shell.dirs.cwd, "/", path, 0);
			if (!fullpath) {
				errno = E_NO_MEMORY;
				return (NULL);
			}

			fullpath = normalize_path(fullpath, J_FREE_VAL_1);
			if (!fullpath) return (NULL);

			t_dir_stack *new_dir = malloc(sizeof(t_dir_stack));
			if (!new_dir) {
				free(fullpath);
				errno = E_NO_MEMORY;
				return (NULL);
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

		char *dirs_remove(int offset) {
			t_dir_stack *current = shell.dirs.stack;
			if (!current) return (NULL);

			if (offset < 0) {
				while (current->next) current = current->next;
				while (offset < -1 && current) {
					current = current->prev;
					offset++;
				}
			} else {
				while (offset > 0 && current) {
					current = current->next;
					offset--;
				}
			}

			if (offset < -1 || offset > 0 || !current) return (NULL);

			if (current->prev)	current->prev->next = current->next;
			else				shell.dirs.stack = current->next;

			if (current->next) current->next->prev = current->prev;
			char *path = current->path;
			free(current);

			return (path);
		}

	#pragma endregion

	#pragma region "Clear"

		void dirs_clear() {
			while (shell.dirs.stack) {
				t_dir_stack *current = shell.dirs.stack;
				shell.dirs.stack = shell.dirs.stack->next;
				free(current->path);
				free(current);
			}
		}

	#pragma endregion

#pragma endregion

#pragma region "Initialize"

	int dirs_print(int offset, int line_mode, int no_tilde, int no_offset) {
		int index = 0;

		char home[PATH_MAX];
		int	 home_len = 0;
		if (!no_tilde) {
			char *home_tmp = get_home();
			if (!home_tmp && errno == E_NO_MEMORY) return (1);
			if (home_tmp) {
				snprintf(home, sizeof(home), "%s", home_tmp);
				free(home_tmp);
				home_len = ft_strlen(home);
			} else no_tilde = 1;
		}

		if (!no_offset && offset) {
			t_dir_stack *current = shell.dirs.stack;
			if (!current) return (errno = E_DIRS_EMPTY, 1);

			if (offset < 0) {
				index++;
				while (current->next) {
					index++;
					current = current->next;
				}
				while (offset < -1 && current) {
					current = current->prev;
					offset++;
					index--;
				}
			} else {
				while (offset > 0 && current) {
					index++;
					current = current->next;
					offset--;
				}
			}

			if (offset < -1 || offset > 0 || !current) return (errno = E_DIRS_RANGE, 1);

			print(STDOUT_FILENO, NULL, P_RESET);

			if (line_mode == 2) {
				char buffer[16];
				snprintf(buffer, sizeof(buffer), " %d  ", index);
				print(STDOUT_FILENO, buffer, P_JOIN);
			}

			if (!no_tilde && !strncmp(home, current->path, home_len) && (current->path[home_len] == '/' || current->path[home_len] == '\0')) {
				char buffer[PATH_MAX];
				snprintf(buffer, sizeof(buffer), "~%s", current->path + home_len);
				print(STDOUT_FILENO, buffer, P_JOIN);
			}
			else print(STDOUT_FILENO, current->path, P_JOIN);

			print(STDOUT_FILENO, "\n", P_PRINT);

			return (0);
		}

		if (!no_offset && !offset) {
			print(STDOUT_FILENO, NULL, P_RESET);

			if (line_mode == 2) print(STDOUT_FILENO, " 0  ", P_JOIN);

			if (!no_tilde && !strncmp(home, shell.dirs.cwd, home_len) && (shell.dirs.cwd[home_len] == '/' || shell.dirs.cwd[home_len] == '\0')) {
				char buffer[PATH_MAX];
				snprintf(buffer, sizeof(buffer), "~%s", shell.dirs.cwd + home_len);
				print(STDOUT_FILENO, buffer, P_JOIN);
			}
			else print(STDOUT_FILENO, shell.dirs.cwd, P_JOIN);

			print(STDOUT_FILENO, "\n", P_PRINT);

			return (0);
		}


		int count = 0;
		t_dir_stack *current = shell.dirs.stack;
		while (current) {
			count++;
			current = current->next;
		}
		current = shell.dirs.stack;

		int width = 0;
		if		(count < 10)	width = 1;
		else if (count < 100)	width = 2;
		else if (count < 1000)	width = 3;
		else					width = 4;
		count = 0;

		print(STDOUT_FILENO, NULL, P_RESET);

		if (line_mode == 2) {
			char buffer[16];
			snprintf(buffer, sizeof(buffer), " %*d  ", width, count++);
			print(STDOUT_FILENO, buffer, P_JOIN);
		}

		if (!no_tilde && !strncmp(home, shell.dirs.cwd, home_len) && (shell.dirs.cwd[home_len] == '/' || shell.dirs.cwd[home_len] == '\0')) {
			char buffer[PATH_MAX];
			snprintf(buffer, sizeof(buffer), "~%s", shell.dirs.cwd + home_len);
			print(STDOUT_FILENO, buffer, P_JOIN);
		}
		else print(STDOUT_FILENO, shell.dirs.cwd, P_JOIN);

		if (current) {
			if (line_mode > 0)	print(STDOUT_FILENO, "\n", P_JOIN);
			else				print(STDOUT_FILENO, " ", P_JOIN);
		}

		current = shell.dirs.stack;
		while (current) {
			if (line_mode == 2) {
				char buffer[16];
				snprintf(buffer, sizeof(buffer), " %*d  ", width, count++);
				print(STDOUT_FILENO, buffer, P_JOIN);
			}

			if (!no_tilde && !strncmp(home, current->path, home_len) && (current->path[home_len] == '/' || current->path[home_len] == '\0')) {
				char buffer[PATH_MAX];
				snprintf(buffer, sizeof(buffer), "~%s", current->path + home_len);
				print(STDOUT_FILENO, buffer, P_JOIN);
			}
			else print(STDOUT_FILENO, current->path, P_JOIN);

			if (current->next) {
				if (line_mode > 0)	print(STDOUT_FILENO, "\n", P_JOIN);
				else				print(STDOUT_FILENO, " ",  P_JOIN);
			}

			current = current->next;
		}

		print(STDOUT_FILENO, "\n", P_PRINT);

		return (0);
	}

#pragma endregion

#pragma region "Initialize"

	void dirs_initialize() {
		
	}

#pragma endregion
