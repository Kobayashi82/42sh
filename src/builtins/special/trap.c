/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:08:17 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/09 12:29:26 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "main/shell.h"
	#include "utils/utils.h"
	#include "utils/getopt.h"

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Help"

		int bt_trap_help(int format, int no_print) {
			char *name = "trap";
			char *syntax = "trap [-lp] [[arg] signal_spec ...]";
			char *description = "Trap signals and other events.";
			char *msg =
				"    Defines and activates handlers to be run when the shell receives signals\n"
				"    or other conditions.\n\n"

				"    ARG is a command to be read and executed when the shell receives the\n"
				"    signal(s) SIGNAL_SPEC.  If ARG is absent (and a single SIGNAL_SPEC\n"
				"    is supplied) or `-', each specified signal is reset to its original\n"
				"    value.  If ARG is the null string each SIGNAL_SPEC is ignored by the\n"
				"    shell and by the commands it invokes.\n\n"

				"    If a SIGNAL_SPEC is EXIT (0) ARG is executed on exit from the shell.  If\n"
				"    a SIGNAL_SPEC is DEBUG, ARG is executed before every simple command.  If\n"
				"    a SIGNAL_SPEC is RETURN, ARG is executed each time a shell function or a\n"
				"    script run by the . or source builtins finishes executing.  A SIGNAL_SPEC\n"
				"    of ERR means to execute ARG each time a command's failure would cause the\n"
				"    shell to exit when the -e option is enabled.\n\n"

				"    If no arguments are supplied, trap prints the list of commands associated\n"
				"    with each signal.\n"
				"\n"
				"    Options:\n"
				"      -l        print a list of signal names and their corresponding numbers\n"
				"      -p        display the trap commands associated with each SIGNAL_SPEC\n"
				"\n"
				"    Each SIGNAL_SPEC is either a signal name in <signal.h> or a signal number.\n"
				"    Signal names are case insensitive and the SIG prefix is optional.  A\n"
				"    signal may be sent to the shell with \"kill -signal $$\".\n\n"

				"    Exit Status:\n"
				"      Returns success unless a SIGSPEC is invalid or an invalid option is given.\n";

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
				"trap 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by "DEVELOPER" ("LOGIN42").\n";

			print(STDOUT_FILENO, msg, RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Trap"

	int bt_trap(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, "", NULL, long_opts, "trap [-lp] [[arg] signal_spec ...]", IGNORE_OFF);
		if (!result)		return (1);
		if (result->error)	return (free_options(result), 2);

		if (find_long_option(result, "help"))		return (free_options(result), bt_trap_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());


		int ret = 0;


		return (free_options(result), ret);
	}

#pragma endregion
