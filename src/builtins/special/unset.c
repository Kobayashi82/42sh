/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:11:19 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/22 10:44:18 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "internal/shell.h"
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
				"unset 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by "DEVELOPER" ("LOGIN42").\n";

			print(STDOUT_FILENO, msg, P_RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Delete"

	static int delete_variable(char *arg) {
		if (!arg) return (0);

		t_var *var = variable_get(shell.env, arg, 1);
		if (var && var->flags & VAR_READONLY) {
			return (exit_error(E_VAR_READONLY, 1, "unset: cannot unset", NULL, EE_FREE_NONE, EE_RETURN));
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

		if (find_long_option(result, "help"))		return (free_options(result), bt_unset_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());


		int ret = 0;

		if (has_option(result, 'f', 0) && has_option(result, 'v', 0)) {
			print(STDERR_FILENO, ft_strjoin(shell.name, ": unset: cannot simultaneously unset a function and a variable", J_FREE_NONE), P_FREE_RESET_PRINT);
			return (free_options(result), 1);
		}

		print(STDERR_FILENO, NULL, P_RESET);

		for (int i = 0; i < result->argc; ++i) {
			if (has_option(result, 'f', 0))	ret = delete_function(result->argv[i]);
			else							ret = delete_variable(result->argv[i]);
		}

		print(STDERR_FILENO, NULL, P_PRINT);

		return (free_options(result), ret);
	}

#pragma endregion
