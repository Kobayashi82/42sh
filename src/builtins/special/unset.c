/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:11:19 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/11 17:44:50 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "main/shell.h"
	#include "utils/utils.h"
	#include "utils/getopt.h"

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Help"

		int bt_unset_help(int format, int no_print) {
			char *name = "unset";
			char *syntax = "unset [-f] [-v] [name ...]";
			char *description = "Unset values and attributes of shell variables.";
			char *msg =
			"    For each NAME, remove the corresponding variable.\n\n"

			"    Options:\n"
			"      -f        treat each NAME as a shell function\n"
			"      -v        treat each NAME as a shell variable\n"

			"    Without options, unset first tries to unset a variable.\n\n"

			"    Some variables cannot be unset; also see `readonly'.\n\n"

			"    Exit Status:\n"
			"      Returns success unless an invalid option is given or a NAME is read-only.\n";

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
				"unset 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by "DEVELOPER" ("LOGIN42").\n";

			print(STDOUT_FILENO, msg, RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Delete"

	static int delete_variable(char *arg) {
		if (!arg) return (0);

		t_var *var = variable_get(shell.env, arg, 1);
		if (var && var->flags & VAR_READONLY) {
			print(STDERR_FILENO, ft_strjoin(shell.name, ": unset: ", 0),                    FREE_JOIN);
			print(STDERR_FILENO, ft_strjoin(arg, ": cannot unset: readonly variable\n", 0), FREE_JOIN);
			return (1);
		} else variable_unset(shell.env, arg, 1);

		return (0);
	}

	static int delete_function(char *arg) {
		if (!arg) return (0);

		return (0);
	}

#pragma endregion

#pragma region "Unset"

	int bt_unset(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, "vn", NULL, long_opts, "unset [-f] [-v] [name ...]", IGNORE_OFF);
		if (!result)		return (1);
		if (result->error)	return (free_options(result), 2);

		if (find_long_option(result, "help"))		return (free_options(result), bt_unset_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());


		int ret = 0;

		if (has_option(result, 'f') && has_option(result, 'v')) {
			print(STDERR_FILENO, ft_strjoin(shell.name, ": unset: cannot simultaneously unset a function and a variable", 0), FREE_RESET_PRINT);
			return (free_options(result), 1);
		}

		print(STDERR_FILENO, NULL, RESET);

		for (int i = 0; i < result->argc; ++i) {
			if (has_option(result, 'f'))	ret = delete_function(result->argv[i]);
			else							ret = delete_variable(result->argv[i]);
		}

		print(STDERR_FILENO, NULL, PRINT);

		return (free_options(result), ret);
	}

#pragma endregion
