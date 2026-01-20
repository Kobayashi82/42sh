/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dirs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 10:43:27 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/20 23:08:40 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"


	#include "main/shell.h"
	#include "utils/utils.h"

#pragma endregion

#pragma region "Add"

	int dirs_add(char *path) {
		errno = 0;
		if (!path) return (1);

		t_dir_stack *new_dir = malloc(sizeof(t_dir_stack));
		if (!new_dir) return (errno = E_NO_MEMORY, 1);

		new_dir->path = ft_strdup(path);
		if (!new_dir->path) return (free(new_dir), errno = E_NO_MEMORY, 1);
		new_dir->prev = NULL;
		new_dir->next = shell.dirs.stack;

		if (shell.dirs.stack) shell.dirs.stack->prev = new_dir;
		shell.dirs.stack = new_dir;

		return (0);
	}

#pragma endregion

#pragma region "Rotate"

	char *dirs_rotate(int offset) {
		t_dir_stack *current = shell.dirs.stack;
		if (!current) return (errno = E_DIRS_EMPTY, NULL);

		// Find target node
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

		if (offset < -1 || offset > 0 || !current) return (errno = E_DIRS_RANGE, NULL);

		char *path = current->path;

		t_dir_stack *new_dir = malloc(sizeof(t_dir_stack));
		if (!new_dir) return (errno = E_NO_MEMORY, NULL);
		new_dir->path = ft_strdup(shell.dirs.cwd);
		if (!new_dir->path) return (free(new_dir), errno = E_NO_MEMORY, NULL);
		new_dir->prev = NULL;
		new_dir->next = shell.dirs.stack;
		if (new_dir->next) new_dir->next->prev = new_dir;
		shell.dirs.stack = new_dir;

		t_dir_stack *last = current->next;

		if (last) {
			while (last->next) last = last->next;
			last->next = shell.dirs.stack;
			shell.dirs.stack->prev = last;
		} else {
			current->next = shell.dirs.stack;
			shell.dirs.stack->prev = current;
		}
		if (current->prev) {
			current->prev->next = NULL;
			current->prev = NULL;
		}

		shell.dirs.stack = current->next;
		if (shell.dirs.stack) shell.dirs.stack->prev = NULL;

		free(current);
		return (path);
	}

#pragma endregion

#pragma region "Push"

	int dirs_push(char *path) {
		errno = 0;

		t_dir_stack *new_dir = malloc(sizeof(t_dir_stack));
		if (!new_dir) return (errno = E_NO_MEMORY, 1);

		new_dir->path = ft_strdup(path);
		if (!new_dir->path) return (free(new_dir), errno = E_NO_MEMORY, 1);

		new_dir->prev = NULL;
		new_dir->next = shell.dirs.stack;
			
		if (shell.dirs.stack) shell.dirs.stack->prev = new_dir;
		shell.dirs.stack = new_dir;

		return (0);
	}

#pragma endregion

#pragma region "Pop"

	char *dirs_pop(int offset) {
		errno = 0;
		if (offset > 0) offset--;
		t_dir_stack *current = shell.dirs.stack;
		if (!current) return (errno = E_DIRS_EMPTY, NULL);

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

		if (offset < -1 || offset > 0 || !current) return (errno = E_DIRS_RANGE, NULL);

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

#pragma region "Print"

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

	int dirs_length() {
		t_dir_stack *current = shell.dirs.stack;
		int count = 0;

		while (current) {
			count++;
			current = current->next;
		}

		return (count);
	}

#pragma endregion
