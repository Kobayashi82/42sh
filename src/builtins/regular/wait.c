/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:18:49 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/09 10:42:52 by vzurera-         ###   ########.fr       */
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

		int bt_wait_help(int format, int no_print) {
			char *name = "wait";
			char *syntax = "wait [-fn] [-p var] [id ...]";
			char *description = "Wait for job completion and return exit status.";
			char *msg =
				"    Waits for each process identified by an ID, which may be a process ID or a\n"
				"    job specification, and reports its termination status.  If ID is not\n"
				"    given, waits for all currently active child processes, and the return\n"
				"    status is zero.  If ID is a job specification, waits for all processes\n"
				"    in that job's pipeline.\n\n"

				"    If the -n option is supplied, waits for a single job from the list of IDs,\n"
				"    or, if no IDs are supplied, for the next job to complete and returns its\n"
				"    exit status.\n\n"

				"    If the -p option is supplied, the process or job identifier of the job\n"
				"    for which the exit status is returned is assigned to the variable VAR\n"
				"    named by the option argument. The variable will be unset initially, before\n"
				"    any assignment. This is useful only when the -n option is supplied.\n\n"

				"    If the -f option is supplied, and job control is enabled, waits for the\n"
				"    specified ID to terminate, instead of waiting for it to change status.\n\n"

				"    Exit Status:\n"
				"    Returns the status of the last ID; fails if ID is invalid or an invalid\n"
				"    option is given, or if -n is supplied and the shell has no unwaited-for\n"
				"    children.\n";

			if (!no_print) print(STDOUT_FILENO, NULL, RESET);

			if (format == HELP_SYNTAX) {
				print(STDOUT_FILENO, ft_strjoin(name, ": ", 0),   FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin(syntax, "\n", 0), FREE_JOIN);
			}

			if (format == HELP_DESCRIPTION) {
				print(STDOUT_FILENO, ft_strjoin(name, " - ", 0),       FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin(description, "\n", 0), FREE_JOIN);
			}

			if (format == HELP_NORMAL) {
				print(STDOUT_FILENO, ft_strjoin(name, ": ", 0),                      FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin(syntax, "\n", 0),                    FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin_sep("    ", description, "\n\n", 0), FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin(msg, "\n", 0),                       FREE_JOIN);
			}

			if (format == HELP_MANPAGE) {
				print(STDOUT_FILENO, "NAME\n",                                       JOIN);
				print(STDOUT_FILENO, ft_strjoin_sep("    ", name, " - ", 0),         FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin(description, "\n\n", 0),             FREE_JOIN);
				print(STDOUT_FILENO, "SYNOPSYS\n",                                   JOIN);
				print(STDOUT_FILENO, ft_strjoin_sep("    ", syntax, "\n\n", 0),      FREE_JOIN);
				print(STDOUT_FILENO, "DESCRIPTION\n",                                JOIN);
				print(STDOUT_FILENO, ft_strjoin_sep("    ", description, "\n\n", 0), FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin(msg, "\n\n", 0),                     FREE_JOIN);
				print(STDOUT_FILENO, "SEE ALSO\n    42sh(1)\n\n",                    JOIN);
			}

			if (!no_print) print(STDOUT_FILENO, NULL, PRINT);

			return (0);
		}

	#pragma endregion

	#pragma region "Version"

		static int version() {
			char *msg =
				"wait 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by "DEVELOPER" ("LOGIN42").\n";

			print(STDOUT_FILENO, msg, RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Wait"

	int bt_wait(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, "", NULL, long_opts, "wait [-fn] [-p var] [id ...]", IGNORE_OFF);
		if (!result)		return (1);
		if (result->error)	return (free_options(result), 2);

		if (find_long_option(result, "help"))		return (free_options(result), bt_wait_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());


		int ret = 0;


		return (free_options(result), ret);
	}

#pragma endregion
