/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   popd.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:08:17 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/17 19:07:04 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "main/shell.h"
	#include "utils/utils.h"
	#include "utils/getopt.h"

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Help"

		int bt_popd_help(int format, int no_print) {
			char *name = "popd";
			char *syntax = "popd [-n] [+N | -N]";
			char *description = "Remove directories from stack.";
			char *msg =
				"    Removes entries from the directory stack.  With no arguments, removes\n"
				"    the top directory from the stack, and changes to the new top directory.\n\n"

				"    Options:\n"
				"      -n        Suppresses the normal change of directory when removing\n"
				"                directories from the stack, so only the stack is manipulated.\n\n"

				"    Arguments:\n"
				"      +N        Removes the Nth entry counting from the left of the list\n"
				"                shown by `dirs', starting with zero.  For example: `popd +0'\n"
				"                removes the first directory, `popd +1' the second.\n\n"

				"      -N        Removes the Nth entry counting from the right of the list\n"
				"                shown by `dirs', starting with zero.  For example: `popd -0'\n"
				"                removes the last directory, `popd -1' the next to last.\n\n"

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
				"popd 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by "DEVELOPER" ("LOGIN42").\n";

			print(STDOUT_FILENO, msg, P_RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Popd"

	int bt_popd(int argc, char **argv) {
		int ret = 0;
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, "n", NULL, long_opts, "popd [-n] [+N | -N]", IGNORE_NUMBER);
		if (errno == E_NO_MEMORY)	ret = exit_error(E_NO_MEMORY, 1, "readonly", NULL, EE_FREE_NONE, EE_RETURN);
		if (errno == E_OPT_MAX)		ret = exit_error(E_OPT_MAX, 2, (argc) ? argv[0] : NULL, ft_itoa(MAX_OPTIONS), EE_FREE_VAL2, EE_RETURN);
		if (errno)					return (free_options(result), ret);

		if (find_long_option(result, "help"))		return (free_options(result), bt_popd_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());

		int offset = 0;

		if (result->argc > 1) {
			exit_error(E_CD_ARGS, 1, "popd", NULL, EE_FREE_NONE, EE_RETURN);
			return (free_options(result), 1);
		}

		if (result->argc == 1) {
			if (!ft_isdigit_s(result->argv[0])) {
				exit_error(E_DIRS_INVALID, 2, "popd", result->argv[0], EE_FREE_NONE, EE_RETURN);
				return (free_options(result), 2);
			}
			if (result->argv[0][0] == '-')	offset = atoi(result->argv[0]) - 1;
			else							offset = atoi(result->argv[0]);
		}

		if (!shell.dirs.stack) {
			exit_error(E_DIRS_EMPTY, 1, "popd", NULL, EE_FREE_NONE, EE_RETURN);
			return (free_options(result), 1);
		}

		char *path = dirs_remove(offset);
		if (!path) {
			if (result->argc)	exit_error(E_DIRS_RANGE, 1, "popd", result->argv[0], EE_FREE_NONE, EE_RETURN);
			else				exit_error(E_DIRS_RANGE, 1, "popd", "0", EE_FREE_NONE, EE_RETURN);
			return (free_options(result), 1);
		}

		if (!offset && has_option(result, 'n')) {
			char *cd_argv[3] = {"cd", path, NULL};
			if ((ret = builtin_exec(1, cd_argv))) {
				if (!dirs_add(path)) {
					ret = exit_error(E_NO_MEMORY, 1, "popd", NULL, EE_FREE_NONE, EE_RETURN);
				}
			}
		}

		free(path);
		return (free_options(result), ret);
	}

#pragma endregion
