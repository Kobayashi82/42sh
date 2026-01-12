/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shift.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:08:17 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/12 12:28:31 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "main/shell.h"
	#include "utils/utils.h"
	#include "utils/getopt.h"

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Help"

		int bt_shift_help(int format, int no_print) {
			char *name = "shift";
			char *syntax = "shift [n]";
			char *description = "Shift positional parameters.";
			char *msg =
				"    Rename the positional parameters $N+1,$N+2 ... to $1,$2 ...  If N is\n"
				"    not given, it is assumed to be 1.\n\n"

				"    Exit Status:\n"
				"      Returns success unless N is negative or greater than $#.\n";

			if (!no_print) print(STDOUT_FILENO, NULL, P_RESET);

			if (format == HELP_SYNTAX) {
				print(STDOUT_FILENO, ft_strjoin(name, ": ", 0),   P_FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin(syntax, "\n", 0), P_FREE_JOIN);
			}

			if (format == HELP_DESCRIPTION) {
				print(STDOUT_FILENO, ft_strjoin(name, " - ", 0),       P_FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin(description, "\n", 0), P_FREE_JOIN);
			}

			if (format == HELP_NORMAL) {
				print(STDOUT_FILENO, ft_strjoin(name, ": ", 0),                      P_FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin(syntax, "\n", 0),                    P_FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin_sep("    ", description, "\n\n", 0), P_FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin(msg, "\n", 0),                       P_FREE_JOIN);
			}

			if (format == HELP_MANPAGE) {
				print(STDOUT_FILENO, "NAME\n",                                       P_JOIN);
				print(STDOUT_FILENO, ft_strjoin_sep("    ", name, " - ", 0),         P_FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin(description, "\n\n", 0),             P_FREE_JOIN);
				print(STDOUT_FILENO, "SYNOPSYS\n",                                   P_JOIN);
				print(STDOUT_FILENO, ft_strjoin_sep("    ", syntax, "\n\n", 0),      P_FREE_JOIN);
				print(STDOUT_FILENO, "DESCRIPTION\n",                                P_JOIN);
				print(STDOUT_FILENO, ft_strjoin_sep("    ", description, "\n\n", 0), P_FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin(msg, "\n\n", 0),                     P_FREE_JOIN);
				print(STDOUT_FILENO, "SEE ALSO\n    42sh(1)\n\n",                    P_JOIN);
			}

			if (!no_print) print(STDOUT_FILENO, NULL,P_PRINT);

			return (0);
		}

	#pragma endregion

	#pragma region "Version"

		static int version() {
			char *msg =
				"shift 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by "DEVELOPER" ("LOGIN42").\n";

			print(STDOUT_FILENO, msg, P_RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Shift"

	int bt_shift(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, NULL, NULL, long_opts, "shift [n]", IGNORE_OFF);
		if (!result)		return (1);
		if (result->error)	return (free_options(result), 2);

		if (find_long_option(result, "help"))		return (free_options(result), bt_shift_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());


		if (result->argc && !ft_isdigit_s(result->argv[0])) {
			print(STDERR_FILENO, ft_strjoin(shell.name, ": shift: ", 0),                          P_FREE_RESET);
			print(STDERR_FILENO, ft_strjoin(result->argv[0], ": numeric argument required\n", 0),P_FREE_PRINT);
			return (free_options(result), 1);
		}

		if (result->argc > 1) {
			print(STDERR_FILENO, ft_strjoin(shell.name, ": shift: too many arguments\n", 0), P_FREE_RESET_PRINT);
			return (free_options(result), 1);
		}

		int number = (result->argc) ? atoi(result->argv[0]) : 1;

		if (number < 0) {
			print(STDERR_FILENO, ft_strjoin(shell.name, ": shift: ", 0),                         P_FREE_RESET);
			print(STDERR_FILENO, ft_strjoin(result->argv[0], ": shift count out of range\n", 0),P_FREE_PRINT);
			return (free_options(result), 1);
		}

		if (!number)					return (free_options(result), 0);
		if (number > shell.env->argc)	return (free_options(result), 1);

		for (int i = 0; i < number; ++i)
			free(shell.env->argv[i]);
		memmove(shell.env->argv, shell.env->argv + number, (shell.env->argc - number + 1) * sizeof(char *));
		shell.env->argc -= number;

		return (free_options(result), 0);
	}

#pragma endregion
