/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:09:10 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/09 10:40:32 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "hashes/builtin.h"
	#include "main/shell.h"
	#include "utils/utils.h"
	#include "utils/getopt.h"

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Help"

		int bt_exit_help(int format, int no_print) {
			char *name = "exit";
			char *syntax = "exit [n]";
			char *description = "Exit the shell.";
			char *msg =
				"    Exits the shell with a status of N.\n"
				"    If N is omitted, the exit status is that of the last command executed.\n";

			if (!no_print) print(STDOUT_FILENO, NULL, RESET);

			if (format == HELP_SYNTAX) {
				print(STDOUT_FILENO, ft_strjoin(name, ": ", 0), FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin(syntax, "\n", 0), FREE_JOIN);
			}

			if (format == HELP_DESCRIPTION) {
				print(STDOUT_FILENO, ft_strjoin(name, " - ", 0), FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin(description, "\n", 0), FREE_JOIN);
			}

			if (format == HELP_NORMAL) {
				print(STDOUT_FILENO, ft_strjoin(name, ": ", 0), FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin(syntax, "\n", 0), FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin_sep("    ", description, "\n\n", 0), FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin(msg, "\n", 0), FREE_JOIN);
			}

			if (format == HELP_MANPAGE) {
				print(STDOUT_FILENO, "NAME\n", JOIN);
				print(STDOUT_FILENO, ft_strjoin_sep("    ", name, " - ", 0), FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin(description, "\n\n", 0), FREE_JOIN);
				print(STDOUT_FILENO, "SYNOPSYS\n", JOIN);
				print(STDOUT_FILENO, ft_strjoin_sep("    ", syntax, "\n\n", 0), FREE_JOIN);
				print(STDOUT_FILENO, "DESCRIPTION\n", JOIN);
				print(STDOUT_FILENO, ft_strjoin_sep("    ", description, "\n\n", 0), FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin(msg, "\n\n", 0), FREE_JOIN);
				print(STDOUT_FILENO, "SEE ALSO\n    42sh(1)\n\n", JOIN);
			}

			if (!no_print) print(STDOUT_FILENO, NULL, PRINT);

			return (0);
		}

	#pragma endregion

	#pragma region "Version"

		static int version() {
			char *msg =
				"exit 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by "DEVELOPER" ("LOGIN42").\n";

			print(STDOUT_FILENO, msg, RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Exit"

	int bt_exit(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, NULL, NULL, long_opts, "exit [n]", IGNORE_OFF);
		if (!result)		return (1);
		if (result->error)	return (free_options(result), 2);

		if (find_long_option(result, "help"))		return (free_options(result), bt_exit_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());


		int ret = 0;

		if (shell.login_shell && shell.mode == SRC_INTERACTIVE && !shell.subshell_level)	print(STDERR_FILENO, "logout\n", RESET_PRINT);
		if (!shell.login_shell && shell.mode == SRC_INTERACTIVE && !shell.subshell_level)	print(STDERR_FILENO, "exit\n", RESET_PRINT);

		if (result->argc > 1) {
			print(STDERR_FILENO, ft_strjoin(shell.name, ": exit: too many arguments\n", 0), FREE_RESET_PRINT);
			ret = 1;
		} else if (result->argc && !ft_isdigit_s(result->argv[0])) {
			print(STDERR_FILENO, ft_strjoin(shell.name, ": exit: numeric argument required\n", 0), FREE_RESET_PRINT);
			ret = 2;
		} else if (result->argc) {
			ret = atol(result->argv[0]);
		}

		free_argv_original(result);
		free_options(result);
		exit_error(NOTHING, ret, NULL, 1);

		return (ret);
	}

#pragma endregion
