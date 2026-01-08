/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 21:00:29 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/08 23:08:47 by vzurera-         ###   ########.fr       */
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

		int bt_help_help(int format, int no_print) {
			char *name = "help";
			char *syntax = "help [-dms] [pattern ...]";
			char *description = "Display information about builtin commands.";
			char *msg =
				"    Displays brief summaries of builtin commands.  If PATTERN is\n"
				"    specified, gives detailed help on all commands matching PATTERN,\n"
				"    otherwise the list of help topics is printed.\n\n"

				"    Options:\n"
				"      -d        output short description for each topic\n"
				"      -m        display usage in pseudo-manpage format\n"
				"      -s        output only a short usage synopsis for each topic matching\n"
				"                PATTERN\n\n"

				"    Arguments:\n"
				"      PATTERN   Pattern specifying a help topic\n\n"

				"    Exit Status:\n"
				"      Returns success unless PATTERN is not found or an invalid option is given.\n";

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
				"help 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by Kobayashi82 (vzurera-).\n";

			print(STDOUT_FILENO, msg, RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Help"

	int bt_help(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, "dms", NULL, long_opts, "help [-dms] [pattern ...]", IGNORE_OFF);
		if (!result)		return (1);
		if (result->error)	return (free_options(result), 2);

		if (find_long_option(result, "help"))		return (free_options(result), bt_help_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());


		int ret = 0;

		if (!result->argc) {
			print(STDOUT_FILENO, "main\n", RESET_PRINT);
		}

		if (result->argv) {
			int add_newline = 0;
			print(STDOUT_FILENO, NULL, RESET);
			print(STDERR_FILENO, NULL, RESET);
			for (int i = 0; i < result->argc; ++i) {
				t_builtin *builtin = builtin_find(result->argv[i]);
				if (builtin) {
					if		(has_option(result, 'd'))	builtin->help(HELP_DESCRIPTION, 1);
					else if	(has_option(result, 'm')) {
						if (add_newline) print(STDOUT_FILENO, "\n", JOIN);
						builtin->help(HELP_MANPAGE, 1);
					} else if	(has_option(result, 's'))	builtin->help(HELP_SYNTAX, 1);
					else {
						if (add_newline) print(STDOUT_FILENO, "\n", JOIN);
						builtin->help(HELP_NORMAL, 1);
					}
					add_newline = 1;
				} else {
					print(STDERR_FILENO, shell.name,                                                                  JOIN);
					print(STDERR_FILENO, ft_strjoin_sep(": help: no help topics match `", result->argv[i], "'.", 0),  FREE_JOIN);
					print(STDERR_FILENO, ft_strjoin_sep("  Try `help help' or `man -k ", result->argv[i], "'.\n", 0), FREE_JOIN);
				}
			}
			print(STDOUT_FILENO, NULL, PRINT);
			print(STDERR_FILENO, NULL, PRINT);
		}

		return (free_options(result), ret);
	}

#pragma endregion

// 42sh (16) + 2 alias
//
// banner
// builtin
// declare, typeset
// dirs
// disown
// enable
// help
// history
// let
// local
// logout
// mapfile, readarray
// popd
// pushd
// shopt
// suspend

// Regular (22) + 1 alias
//
// alias
// bg
// cd
// command
// echo
// false
// fc
// fg
// getopts
// hash
// jobs
// kill
// printf
// pwd
// read
// test, [
// true
// type
// ulimit
// umask
// unalias
// wait

// Special (15) + 1 alias
//
// break
// continue
// ., source
// :
// eval
// exec
// exit
// export
// readonly
// return
// set
// shift
// times
// trap
// unset

//	Total: 53 + 4 alias

// Not Implemented
//
// bind
// caller
// compgen
// complete
// compopt
