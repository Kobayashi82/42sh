/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:11:49 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/26 15:18:28 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "internal/shell.h"
	#include "utils/utils.h"
	#include "utils/getopt.h"

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Help"

		int bt_alias_help(int format, int no_print) {
			char *name = "alias";
			char *syntax = "alias [-p] [name[=value] ... ]";
			char *description = "Define or display aliases.";
			char *msg =
				"    Without arguments, `alias' prints the list of aliases in the reusable\n"
				"    form `alias NAME=VALUE' on standard output.\n\n"

				"    Otherwise, an alias is defined for each NAME whose VALUE is given.\n"
				"    A trailing space in VALUE causes the next word to be checked for\n"
				"    alias substitution when the alias is expanded.\n\n"

				"    Options:\n"
				"      -p        print all defined aliases in a reusable format\n\n"

				"    Exit Status:\n"
				"      Returns success unless a NAME is supplied for which no alias has been defined.\n";

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
				"alias 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by "DEVELOPER" ("LOGIN42").\n";

			print(STDOUT_FILENO, msg, P_RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Alias"

	int bt_alias(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, "p", NULL, long_opts, "alias [-p] [name[=value] ... ]", 0);
		if (!result) return (free_options(result), (shell.error == E_OPT_MAX || shell.error == E_OPT_INVALID) ? 2 : 1);

		if (find_long_option(result, "help"))		return (free_options(result), bt_alias_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());


		int ret = 0;

		if (!result->argc || has_option(result, 'p', 0)) {
			ret = alias_print(SORT_NORMAL);
			if (shell.error == E_NO_MEMORY) ret = exit_error(E_NO_MEMORY, 1, "alias", NULL, EE_FREE_NONE, EE_RETURN);
			return (free_options(result), ret);
		}

		for (int i = 0; i < result->argc; ++i) {
			if (result->argv[i][0] != '=' && strchr(result->argv[i], '=')) {
				// Add alias
				char *key = NULL, *value = NULL;
				get_key_value(result->argv[i], &key, &value, '=');
				if (shell.error == E_NO_MEMORY) {
					exit_error(E_NO_MEMORY, 1, "alias", NULL, EE_FREE_NONE, EE_RETURN);
					return (free_options(result), 1);
				}

				if (alias_add(key, value)) {
					if (shell.error == E_NO_MEMORY) {
						exit_error(E_NO_MEMORY, 1, "alias", NULL, EE_FREE_NONE, EE_RETURN);
						return (free_options(result), 1);
					}
					if (shell.error == E_ALIAS_INVALID) exit_error(E_ALIAS_INVALID, 1, "alias: ", result->argv[i], EE_FREE_NONE, EE_RETURN);
					ret = 1;
				}

				free(key);
				free(value);
			} else {
				// Print alias
				t_alias *alias = alias_get(result->argv[i]);
				if (alias) {
					print(STDOUT_FILENO, ft_strjoin_sep("alias ", alias->name, "='", J_FREE_NONE), P_FREE_RESET);
					print(STDOUT_FILENO, ft_strjoin(alias->value, "'\n",             J_FREE_NONE), P_FREE_PRINT);
				} else {
					exit_error(E_ALIAS_NOT_FOUND, 1, "alias: ", result->argv[i], EE_FREE_NONE, EE_RETURN);
					ret = 1;
				}

			}
		}

		return (free_options(result), ret);
	}

#pragma endregion
