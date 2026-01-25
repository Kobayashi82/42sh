/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dirs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:08:17 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/25 10:39:57 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "internal/shell.h"
	#include "utils/utils.h"
	#include "utils/getopt.h"

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Help"

		int bt_dirs_help(int format, int no_print) {
			char *name = "dirs";
			char *syntax = "dirs [-clpv] [+N] [-N]";
			char *description = "Display directory stack.";
			char *msg =
				"    Display the list of currently remembered directories.  Directories\n"
				"    find their way onto the list with the `pushd' command; you can get\n"
				"    back up through the list with the `popd' command.\n\n"

				"    Options:\n"
				"      -c        clear the directory stack by deleting all of the elements\n"
				"      -l        do not print tilde-prefixed versions of directories relative\n"
				"                to your home directory\n"
				"      -p        print the directory stack with one entry per line\n"
				"      -v        print the directory stack with one entry per line prefixed\n"
				"                with its position in the stack\n\n"

				"    Arguments:\n"
				"      +N        Displays the Nth entry counting from the left of the list\n"
				"                shown by dirs when invoked without options, starting with\n"
				"                zero.\n\n"

				"      -N        Displays the Nth entry counting from the right of the list\n"
				"                shown by dirs when invoked without options, starting with\n"
				"                zero.\n\n"

				"    Exit Status:\n"
				"      Returns success unless an invalid option is supplied or an error occurs.\n";

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
				"dirs 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by "DEVELOPER" ("LOGIN42").\n";

			print(STDOUT_FILENO, msg, P_RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Dirs"

	int bt_dirs(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, "clpv", NULL, long_opts, "dirs [-clpv] [+N] [-N]", IGNORE_OFF);
		if (!result) return (free_options(result), (shell.error == E_OPT_MAX || shell.error == E_OPT_INVALID) ? 2 : 1);

		if (find_long_option(result, "help"))		return (free_options(result), bt_dirs_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());


		int ret = 0;
		int offset = 0;

		if (has_option(result, 'c', 0)) {
			dirs_clear();
			return (free_options(result), ret);
		}

		if (result->argc > 1) {
			exit_error(E_DIRS_ARGS, 1, "dirs", NULL, EE_FREE_NONE, EE_RETURN);
			return (free_options(result), 1);
		}

		if (result->argc == 1) {
			if (!ft_isdigit_s(result->argv[0])) {
				exit_error(E_DIRS_OFFSET, 2, "dirs", result->argv[0], EE_FREE_NONE, EE_RETURN);
				return (free_options(result), 2);
			}
			if (result->argv[0][0] == '-')	offset = atoi(result->argv[0]) - 1;
			else							offset = atoi(result->argv[0]);
		}

		int line_mode = 0;
		if (has_option(result, 'p', 0)) line_mode = 1;
		if (has_option(result, 'v', 0)) line_mode = 2;

		if (dirs_print(offset, line_mode, has_option(result, 'l', 0), !result->argc)) {
			if (shell.error == E_NO_MEMORY)		ret = exit_error(E_NO_MEMORY,  1, "dirs", NULL, EE_FREE_NONE, EE_RETURN);
			if (shell.error == E_DIRS_EMPTY)	ret = exit_error(E_DIRS_EMPTY, 1, "dirs", NULL, EE_FREE_NONE, EE_RETURN);
			if (shell.error == E_DIRS_RANGE) {
				if (result->argc)		ret = exit_error(E_DIRS_RANGE, 1, "dirs", result->argv[0], EE_FREE_NONE, EE_RETURN);
				else					ret = exit_error(E_DIRS_RANGE, 1, "dirs", "0",             EE_FREE_NONE, EE_RETURN);
			}
		}

		return (free_options(result), ret);
	}

#pragma endregion
