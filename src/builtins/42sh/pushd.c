/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pushd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:08:17 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/22 10:39:37 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "internal/shell.h"
	#include "utils/utils.h"
	#include "utils/getopt.h"

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Help"

		int bt_pushd_help(int format, int no_print) {
			char *name = "pushd";
			char *syntax = "pushd [-n] [+N | -N | dir]";
			char *description = "Add directories to stack.";
			char *msg =
				"    Adds a directory to the top of the directory stack, or rotates\n"
				"    the stack, making the new top of the stack the current working\n"
				"    directory.  With no arguments, exchanges the top two directories.\n\n"

				"    Options:\n"
				"      -n        Suppresses the normal change of directory when adding\n"
				"                directories to the stack, so only the stack is manipulated.\n\n"

				"    Arguments:\n"
				"      +N        Rotates the stack so that the Nth directory (counting\n"
				"                from the left of the list shown by `dirs', starting with\n"
				"                zero) is at the top.\n\n"

				"      -N        Rotates the stack so that the Nth directory (counting\n"
				"                from the right of the list shown by `dirs', starting with\n"
				"                zero) is at the top.\n\n"

				"      dir       Adds DIR to the directory stack at the top, making it the\n"
				"                new current working directory.\n\n"

				"    The `dirs' builtin displays the directory stack.\n\n"

				"    Exit Status:\n"
				"      Returns success unless an invalid argument is supplied or the\n"
				"      directory change fails.\n";

			if (!no_print) print(STDOUT_FILENO, NULL, P_RESET);

				if (format == HELP_SYNTAX) {
					print(STDOUT_FILENO, ft_strjoin(name, ": ",   J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin(syntax, "\n", J_FREE_NONE), P_FREE_JOIN);
				}

				if (format == HELP_DESCRIPTION) {
					print(STDOUT_FILENO, ft_strjoin(name, " - ",       J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin(description, "\n", J_FREE_NONE), P_FREE_JOIN);
				}

				if (format == HELP_NORMAL) {
					print(STDOUT_FILENO, ft_strjoin(name, ": ",                      J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin(syntax, "\n",                    J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin_sep("    ", description, "\n\n", J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin(msg, "\n",                       J_FREE_NONE), P_FREE_JOIN);
				}

				if (format == HELP_MANPAGE) {
					print(STDOUT_FILENO, "NAME\n",                                                 P_JOIN);
					print(STDOUT_FILENO, ft_strjoin_sep("    ", name, " - ",         J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin(description, "\n\n",             J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, "SYNOPSYS\n",                                             P_JOIN);
					print(STDOUT_FILENO, ft_strjoin_sep("    ", syntax, "\n\n",      J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, "DESCRIPTION\n",                                          P_JOIN);
					print(STDOUT_FILENO, ft_strjoin_sep("    ", description, "\n\n", J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin(msg, "\n\n",                     J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, "SEE ALSO\n    42sh(1)\n\n",                              P_JOIN);
				}

			if (!no_print) print(STDOUT_FILENO, NULL, P_PRINT);

			return (0);
		}

	#pragma endregion

	#pragma region "Version"

		static int version() {
			char *msg =
				"pushd 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by "DEVELOPER" ("LOGIN42").\n";

			print(STDOUT_FILENO, msg, P_RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Pushd"

	int bt_pushd(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, "n", NULL, long_opts, "pushd [-n] [+N | -N | dir]", IGNORE_NUMBER);
		if (!result) return (free_options(result), (shell.error == E_OPT_MAX || shell.error == E_OPT_INVALID) ? 2 : 1);

		if (find_long_option(result, "help"))		return (free_options(result), bt_pushd_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());


		int ret = 0;
		int offset = 0;
		int is_offset = 0;
		int is_rotate = 0;

		if (result->argc > 1) {
			exit_error(E_DIRS_ARGS, 1, "pushd", NULL, EE_FREE_NONE, EE_RETURN);
			return (free_options(result), 1);
		}

		// Path or offset
		if (result->argc == 1) {
			char *resolved_path = resolve_path(result->argv[0]);
			if (result->argv[0][0] == '-' || result->argv[0][0] == '+') {
				if (ft_isdigit_s(result->argv[0])) {
					if (result->argv[0][0] == '-')	offset = atoi(result->argv[0]) - 1;
					else							offset = atoi(result->argv[0]);
					is_offset = 1;
				} else {
					free(resolved_path);
					exit_error(E_DIRS_OFFSET, 2, "pushd", result->argv[0], EE_FREE_NONE, EE_RETURN);
					return (free_options(result), 2);
				}
			} else if (access(resolved_path, F_OK) == -1 && ft_isdigit_s(result->argv[0])) {
				offset = atoi(result->argv[0]);
				is_offset = 1;
			}
			free(resolved_path);
		}

		char *new_path = NULL;

		if (!result->argc) {
			new_path = dirs_pop(0);
			if (!new_path) {
				if (shell.error == E_DIRS_EMPTY) exit_error(E_DIRS_EMPTY_DIR, 1, "pushd", NULL, EE_FREE_NONE, EE_RETURN);
				return (free_options(result), 1);
			}
		}

		// Rotate
		if (is_offset) {
			if ((offset < -1 && (dirs_length() + (offset + 1)) == 0) || !offset) {
				if (dirs_print(0, 0, 0, 1) && shell.error == E_NO_MEMORY) ret = exit_error(E_NO_MEMORY,  1, "pushd", NULL, EE_FREE_NONE, EE_RETURN);
				return (free_options(result), ret);
			}
			is_rotate = 1;
			if (!result->argc) { offset = 1; is_offset = 1; is_rotate = 0; }
			if (offset > 0) offset--;
			new_path = dirs_rotate(offset);
			if (!new_path) {
				if (shell.error == E_NO_MEMORY)		exit_error(E_NO_MEMORY,  1, "pushd", NULL,            EE_FREE_NONE, EE_RETURN);
				if (shell.error == E_DIRS_EMPTY)		exit_error(E_DIRS_EMPTY, 1, "pushd", NULL,            EE_FREE_NONE, EE_RETURN);
				if (shell.error == E_DIRS_RANGE) {
					if (result->argc)			exit_error(E_DIRS_RANGE, 1, "pushd", result->argv[0], EE_FREE_NONE, EE_RETURN);
					else						exit_error(E_DIRS_RANGE, 1, "pushd", "0",             EE_FREE_NONE, EE_RETURN);
				}
				return (free_options(result), 1);
			}
		}

		// Path
		if (!new_path) new_path = ft_strdup(result->argv[0]);
		if (!new_path) {
			shell.error = E_NO_MEMORY;
			exit_error(E_NO_MEMORY,  1, "pushd", NULL, EE_FREE_NONE, EE_RETURN);
			return (free_options(result), 1);
		}
		char *old_path = ft_strdup(shell.dirs.cwd);
		if (!old_path) {
			free(new_path);
			shell.error = E_NO_MEMORY;
			exit_error(E_NO_MEMORY,  1, "pushd", NULL, EE_FREE_NONE, EE_RETURN);
			return (free_options(result), 1);
		}

		// Push
		if (has_option(result, 'n', 0)) {
			if (!is_offset && dirs_push(new_path)) {
				if (shell.error == E_NO_MEMORY) exit_error(E_NO_MEMORY,  1, "pushd", NULL, EE_FREE_NONE, EE_RETURN);
				ret = 1;
			}
		} else {
			char *cd_argv[4] = {"pushd", "--", new_path, NULL};
			ret = bt_cd(3, cd_argv);
			if (!ret && !is_rotate && dirs_push(old_path)) {
				if (shell.error == E_NO_MEMORY) exit_error(E_NO_MEMORY,  1, "pushd", NULL, EE_FREE_NONE, EE_RETURN);
				ret = 1;
			}
		}

		free(new_path);
		free(old_path);
		if (!ret && dirs_print(0, 0, 0, 1)) {
			if (shell.error == E_NO_MEMORY)	ret = exit_error(E_NO_MEMORY,  1, "pushd", NULL, EE_FREE_NONE, EE_RETURN);
			if (shell.error == E_DIRS_EMPTY)	ret = exit_error(E_DIRS_EMPTY, 1, "pushd", NULL, EE_FREE_NONE, EE_RETURN);
			if (shell.error == E_DIRS_RANGE) {
				if (result->argc)		ret = exit_error(E_DIRS_RANGE, 1, "pushd", result->argv[0], EE_FREE_NONE, EE_RETURN);
				else					ret = exit_error(E_DIRS_RANGE, 1, "pushd", "0",             EE_FREE_NONE, EE_RETURN);
			}
		}
		return (free_options(result), ret);
	}

#pragma endregion
