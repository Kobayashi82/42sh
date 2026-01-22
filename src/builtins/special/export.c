/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:06:34 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/22 19:20:14 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "internal/shell.h"
	#include "utils/utils.h"
	#include "utils/getopt.h"

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Help"

		int bt_export_help(int format, int no_print) {
			char *name = "export";
			char *syntax = "export [-fn] [name[=value] ...] or export [-fp]";
			char *description = "Set export attribute for shell variables.";
			char *msg =
				"    Marks each NAME for automatic export to the environment of subsequently\n"
				"    executed commands.  If VALUE is supplied, assign VALUE before exporting.\n\n"

				"    Options:\n"
				"      -f        refer to shell functions"
				"      -n        remove the export property from each NAME\n"
				"      -p        display a list of all exported variables and functions\n\n"

				"    Exit Status:\n"
				"      Returns success unless an invalid option is given or NAME is invalid.\n";

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
				"export 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by "DEVELOPER" ("LOGIN42").\n";

			print(STDOUT_FILENO, msg, P_RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Add"

	#pragma region "Variable"

		static int add_export(char *arg) {
			if (!arg) return (0);

			if (!strchr(arg, '=')) {
				if (variable_validate(arg)) {
					print(STDERR_FILENO, ft_strjoin(shell.name, ": export: `",          J_FREE_NONE), P_FREE_JOIN);
					print(STDERR_FILENO, ft_strjoin(arg, "': not a valid identifier\n", J_FREE_NONE), P_FREE_JOIN);
					return (1);
				}
				t_var *var = variable_get(shell.env, arg, 1);
				if (var) {
					var->flags |= VAR_EXPORTED;
					return (0);
				}
			}

			char *key = NULL, *value = NULL;
			get_key_value(arg, &key, &value, '=');
			int len = ft_strlen(key);

			int append = 0;
			if (key && len > 0 && key[len - 1] == '+') {
				key[len - 1] = '\0'; append = 1;
			}

			if (variable_validate(key)) {
				if (append) key[len - 1] = '+';
				print(STDERR_FILENO, ft_strjoin_sep(shell.name, ": export: `", key,            J_FREE_VAL_3), P_FREE_JOIN);
				print(STDERR_FILENO, ft_strjoin_sep("=", value, "': not a valid identifier\n", J_FREE_VAL_2), P_FREE_JOIN);
				return (1);
			}

			t_var *var = variable_get(shell.env, key, 1);
			if (var && var->flags & VAR_READONLY) {
				print(STDERR_FILENO, ft_strjoin(shell.name, ": export: `",     J_FREE_NONE), P_FREE_JOIN);
				print(STDERR_FILENO, ft_strjoin(key, "': readonly variable\n", J_FREE_VAL_1), P_FREE_JOIN);
				free(value);
				return (1);
			}

			int ret = 0;

			// if		((var && var->flags & VAR_ASSOCIATIVE))	ret = variable_assoc_set(shell.env, key, value, append, VAR_EXPORTED, 0);
			// else if ((var && var->flags & VAR_ARRAY))		ret = variable_array_set(shell.env, key, value, append, VAR_EXPORTED, 0);
			// else											ret = variable_scalar_set(shell.env, key, value, append, VAR_EXPORTED, 0);
			ret = variable_scalar_set(shell.env, key, value, append, VAR_EXPORTED, 0);

			free(key);
			free(value);

			return (ret);
		}

	#pragma endregion

	#pragma region "Function"

		static int add_export_function(char *arg) {
			if (!arg) return (0);

			int ret = 0;

			return (ret);
		}

	#pragma endregion

#pragma endregion

#pragma region "Delete"

	#pragma region "Variable"

		static int delete_export(char *arg) {
			if (!arg) return (0);

			if (!strchr(arg, '=')) {
				if (variable_validate(arg)) {
					print(STDERR_FILENO, ft_strjoin(shell.name, ": export: `",          J_FREE_NONE), P_FREE_JOIN);
					print(STDERR_FILENO, ft_strjoin(arg, "': not a valid identifier\n", J_FREE_NONE), P_FREE_JOIN);
					return (1);
				}
				t_var *var = variable_get(shell.env, arg, 1);
				if (var) var->flags &= ~VAR_EXPORTED;
				return (0);
			}

			char *key = NULL, *value = NULL;
			get_key_value(arg, &key, &value, '=');
			int len = ft_strlen(key);

			int	append = 0;
			if (key && len > 0 && key[len - 1] == '+') {
				key[len - 1] = '\0'; append = 1;
			}

			if (variable_validate(key)) {
				if (append) key[len - 1] = '+';
				print(STDERR_FILENO, ft_strjoin_sep(shell.name, ": export: `", key,            J_FREE_VAL_3), P_FREE_JOIN);
				print(STDERR_FILENO, ft_strjoin_sep("=", value, "': not a valid identifier\n", J_FREE_VAL_2), P_FREE_JOIN);
				return (1);
			}

			t_var *var = variable_get(shell.env, key, 1);
			if (var && var->flags & VAR_READONLY) {
				print(STDERR_FILENO, ft_strjoin(shell.name, ": export: `",     J_FREE_NONE), P_FREE_JOIN);
				print(STDERR_FILENO, ft_strjoin(key, "': readonly variable\n", J_FREE_VAL_1), P_FREE_JOIN);
				free(value);
				return (1);
			}

			int ret = 0;

			// Esto depende del tipo de variable (se pueden crear de tipo scalar, array o associativa si ya existia de antes... creo)
			ret = variable_scalar_set(shell.env, key, value, append, VAR_NONE, 0);


			free(key);
			free(value);

			return (ret);
		}

	#pragma endregion

	#pragma region "Function"

		static int delete_export_function(char *arg) {
			if (!arg) return (0);

			int ret = 0;

			return (ret);
		}

	#pragma endregion

#pragma endregion

#pragma region "Export"

	int bt_export(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, "fnp", NULL, long_opts, "export [-fn] [name[=value] ...] or export [-fp]", IGNORE_OFF);
		if (!result)		return (1);

		if (find_long_option(result, "help"))		return (free_options(result), bt_export_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());


		int ret = 0;

		if (!result->argc) {
			if (has_option(result, 'f', 0))	variable_print(shell.env, VAR_EXPORTED, SORT_NORMAL, 0, 1);	// function
			else							variable_print(shell.env, VAR_EXPORTED, SORT_NORMAL, 0, 1);
			return (free_options(result), 0);
		}

		print(STDERR_FILENO, NULL, P_RESET);

		for (int i = 0; i < result->argc; ++i) {
			if (has_option(result, 'n', 0)) {
				if (has_option(result, 'f', 0))	ret = delete_export_function(result->argv[i]);
				else							ret = delete_export(result->argv[i]);
			} else {
				if (has_option(result, 'f', 0))	ret = add_export_function(result->argv[i]);
				else							ret = add_export(result->argv[i]);
			}
		}

		print(STDERR_FILENO, NULL, P_PRINT);

		return (free_options(result), ret);
	}

#pragma endregion

#pragma region "Info"

	// Funcion exportada
	// 
	// 42_FUNC_mi_funcion%%=() {
	//  echo esto es prueba;
	//  pwd
	// }

#pragma endregion
