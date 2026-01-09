/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:06:34 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/09 10:40:32 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "hashes/builtin.h"
	#include "hashes/variable.h"
	#include "main/shell.h"
	#include "utils/utils.h"
	#include "utils/getopt.h"

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Help"

		int bt_export_help(int format, int no_print) {
			char *name = "export";
			char *syntax = "export [-n] [name[=value] ...] or export -p";
			char *description = "Set export attribute for shell variables.";
			char *msg =
				"    Marks each NAME for automatic export to the environment of subsequently\n"
				"    executed commands.  If VALUE is supplied, assign VALUE before exporting.\n\n"

				"    Options:\n"
				"      -n        remove the export property from each NAME\n"
				"      -p        display a list of all exported variables and functions\n\n"

				"    Exit Status:\n"
				"      Returns success unless an invalid option is given or NAME is invalid.\n";

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
				"export 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by "DEVELOPER" ("LOGIN42").\n";

			print(STDOUT_FILENO, msg, RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Add"

	static int add_export(char *arg) {
		if (!arg) return (0);
		int ret = 0;

		if (!strchr(arg, '=')) {
			if (variables_validate(arg, NULL, "export", 0, 1)) return (1);
			t_var *var = variables_find(shell.env->table, arg);
			if (var) { var->exported = 1; return (0); }
		}

		char *key = NULL, *value = NULL;
		get_key_value(arg, &key, &value, '=');

		int len = ft_strlen(key);
		int	concatenate = 0;
		if (key && len > 0 && key[len - 1] == '+') { key[len - 1] = '\0'; concatenate = 1; }
		if (variables_validate(key, value, "export", 1, 1)) return (free(key), free(value), 1);

		t_var *var = variables_find(shell.env->table, key);
		if (var && var->readonly) {
			var->exported = 1;
			print(STDOUT_FILENO, NULL,                                        RESET);
			print(STDERR_FILENO, ft_strjoin(shell.name, ": ", 0),             FREE_JOIN);
			print(STDERR_FILENO, ft_strjoin(key, ": readonly variable\n", 0), FREE_PRINT);
			ret = 1;
		} else {
			if (concatenate && variables_concatenate(shell.env->table, key, value, 1, -1, -1, -1))	ret = 1;
			if (!concatenate && variables_add(shell.env->table, key, value, 1, -1, -1, -1))			ret = 1;
		}

		free(key);
		free(value);
		return (ret);
	}

#pragma endregion

#pragma region "Delete"

	static int delete_export(char *arg) {
		if (!arg) return (0);
		int ret = 0;

		if (!strchr(arg, '=')) {
			if (variables_validate(arg, NULL, "export", 0, 1)) return (1);
			t_var *var = variables_find(shell.env->table, arg);
			if (var) var->exported = 0;
			return (0);
		}

		char *key = NULL, *value = NULL;
		get_key_value(arg, &key, &value, '=');

		int len = ft_strlen(key);
		int	concatenate = 0;
		if (key && len > 0 && key[len - 1] == '+') { key[len - 1] = '\0'; concatenate = 1; }

		if (variables_validate(key, value, "export", 1, 1)) return (free(key), free(value), 1);
		t_var *var = variables_find(shell.env->table, key);
		if (var && var->readonly) {
			print(STDOUT_FILENO, NULL,                                        RESET);
			print(STDERR_FILENO, ft_strjoin(shell.name, ": ", 0),             FREE_JOIN);
			print(STDERR_FILENO, ft_strjoin(key, ": readonly variable\n", 0), FREE_PRINT);
			ret = 1;
		} else {
			if (concatenate && variables_concatenate(shell.env->table, key, value, 0, -1, -1, -1))	ret = 1;
			if (!concatenate && variables_add(shell.env->table, key, value, 0, -1, -1, -1))			ret = 1;
		}

		free(key);
		free(value);
		return (ret);
	}

#pragma endregion

#pragma region "Export"

	int bt_export(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, "np", NULL, long_opts, "export [-n] [name[=value] ...] or export -p", IGNORE_OFF);
		if (!result)		return (1);
		if (result->error)	return (free_options(result), 2);

		if (find_long_option(result, "help"))		return (free_options(result), bt_export_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());


		int ret = 0;

		if (!result->argc) {
			variables_print(shell.env->table, EXPORTED_LIST, 1);
			return (free_options(result), 0);
		}

		for (int i = 0; i < result->argc; ++i) {
			if (has_option(result, 'n')) {
				if (delete_export(result->argv[i]))	ret = 1;
			} else {
				if (add_export(result->argv[i]))	ret = 1;
			}
		}

		return (free_options(result), ret);
	}

#pragma endregion
