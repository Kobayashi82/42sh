/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:11:49 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/06 16:45:40 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "hashes/builtin.h"
	#include "hashes/alias.h"
	#include "main/shell.h"
	#include "utils/libft.h"
	#include "utils/print.h"
	#include "utils/getopt2.h"

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
				"alias 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by Kobayashi82 (vzurera-).\n";

			print(STDOUT_FILENO, msg, RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Add"

	static int add_alias(char *arg) {
		if (!arg) return (0);

		char *key = NULL, *value = NULL;
		get_key_value(arg, &key, &value, '=');
		if (alias_validate(key, 1)) return (free(key), free(value), 1);
		alias_add(key, value);
		return (free(key), free(value), 0);
	}

#pragma endregion

#pragma region "Print"

	static int print_alias(char *arg, char **values, char **invalues) {
		if (!arg) return (0);

		t_alias *alias = alias_find(arg);
		if (alias && alias->name) {
			char *value = ft_strjoin_sep("alias ", alias->name, "='", 0);
			if (alias->value) value = ft_strjoin_sep(value, alias->value, "'\n", 1);
			if (value) *values = ft_strjoin(*values, value, 3);
		}

		if (!alias) {
			char *value = ft_strjoin(shell.arg0, ": alias: ", 0);
			value = ft_strjoin_sep(value, arg, ": not found\n", 1);
			if (value) *invalues = ft_strjoin(*invalues, value, 3);
		}

		return (0);
	}

#pragma endregion

#pragma region "Alias"

	int bt_alias(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, "p", NULL, long_opts, "alias [-p] [name[=value] ... ]", 0);
		if (!result)		return (1);
		if (result->error)	return (free_options(result), 2);

		if (find_long_option(result, "help"))		return (free_options(result), bt_alias_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());


		int ret = 0;

		char *values = NULL, *invalues = NULL;

		if (!result->argc || has_option(result, 'p')) {
			alias_print(1);
			return (free_options(result), 0);
		}

		for (int i = 0; i < result->argc; ++i) {
			if (result->argv[i][0] != '=' && strchr(result->argv[i], '=')) {
				if (add_alias(result->argv[i])) ret = 1;
			} else {
				print_alias(result->argv[i], &values, &invalues);
			}
		}

		if (values)		{ print(STDOUT_FILENO, values, RESET_PRINT); free(values); }
		if (invalues)	{ print(STDERR_FILENO, invalues, RESET_PRINT); free(invalues); }

		return (free_options(result), ret);
	}

#pragma endregion
