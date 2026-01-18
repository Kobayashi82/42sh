/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readonly.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:06:39 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/18 11:32:05 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"
	
	#include "main/shell.h"
	#include "utils/utils.h"
	#include "utils/getopt.h"

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Help"

		int bt_readonly_help(int format, int no_print) {
			char *name = "readonly";
			char *syntax = "readonly [-aAfn] [name[=value] ...] or readonly [-fp]";
			char *description = "Mark shell variables as unchangeable.";
			char *msg =
				"    Mark each NAME as read-only; the values of these NAMEs may not be\n"
				"    changed by subsequent assignment.  If VALUE is supplied, assign VALUE\n"
				"    before marking as read-only.\n\n"

				"    Options:\n"
				"      -a        refer to indexed array variables\n"
				"      -A        refer to associative array variables\n"
				"      -f        refer to shell functions\n"
				"      -n        refer to reference variables\n"
				"      -p        display a list of all readonly variables or functions,\n"
				"                depending on whether or not the -f option is given\n\n"

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
				"readonly 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by "DEVELOPER" ("LOGIN42").\n";

			print(STDOUT_FILENO, msg, P_RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Variable"

	static int variable_readonly(t_parse_result *result, char *arg) {
		if (!arg) return (0);

		int reference = 1;
		int type = VAR_READONLY;
		if (has_option(result, 'A')) type = (VAR_ASSOCIATIVE | VAR_READONLY);
		if (has_option(result, 'a')) type = (VAR_ARRAY       | VAR_READONLY);
		if (has_option(result, 'n')) {
			type = (VAR_REFERENCE   | VAR_READONLY);
			reference = 0;
		}

		int		ret = 0, append = 0, no_value = 1;
		char	*key = NULL, *value = NULL;
		errno = 0;

		if (strchr(arg, '=')) {
			no_value = 0;
			get_key_value(arg, &key, &value, '=');
			int len = ft_strlen(key);
			if (key && len > 0 && key[len - 1] == '+') {
				key[len - 1] = '\0'; append = 1;
			}
		} else key = ft_strdup(arg);

		if ((!key || !value) && errno == E_NO_MEMORY)	return (free(key), free(value), exit_error(E_NO_MEMORY, 1, "readonly", NULL, EE_FREE_NONE, EE_RETURN));

		t_var *var = variable_get(shell.env, key, reference);
		if (!var) {
			if (errno) { free(value); value = NULL; }
			if (errno == E_NO_MEMORY)					return (free(key), exit_error(E_NO_MEMORY, 1,      "readonly",   NULL, EE_FREE_NONE, EE_RETURN));
			if (errno == E_VAR_IDENTIFIER)				return (exit_error(E_VAR_IDENTIFIER, 1,            "readonly: ", key,  EE_FREE_VAL2, EE_RETURN));
			if (errno == E_VAR_MAX_REFERENCES)			return (exit_error(E_VAR_MAX_REFERENCES, 1,        "readonly: ", key,  EE_FREE_VAL2, EE_RETURN));
			if (errno == E_VAR_CYCLE_REFERENCE)			return (exit_error(E_VAR_CYCLE_REFERENCE, 1,       "readonly: ", key,  EE_FREE_VAL2, EE_RETURN));
		}
		if (var && no_value)							return (free(key), free(value), var->flags |= type, 0);
		if (var && var->flags & VAR_READONLY)			return (free(value), exit_error(E_VAR_READONLY, 1, "readonly: ", key,  EE_FREE_VAL2, EE_RETURN));

		int create_scalar = 1;
		if (!no_value) {
			if		((type & VAR_REFERENCE)   || (var && var->flags & VAR_REFERENCE)) {
				create_scalar = 0;
				// ret = variable_reference_set(shell.env, key, value, append, type, 0);
			}
			else if ((type & VAR_ASSOCIATIVE) || (var && var->flags & VAR_ASSOCIATIVE)) {
				// Obtener key, o valor =() o usar indice 0. Si falla no ejecuta este if
				create_scalar = 0;
				// ret = variable_assoc_set(shell.env, key, index, value, append, type, 0);
			}
			else if ((type & VAR_ARRAY)       || (var && var->flags & VAR_ARRAY)) {
				// Obtener indice, o valor =() o usar indice 0. Si falla no ejecuta este if
				create_scalar = 0;
				// ret = variable_array_set(shell.env, key, index, value, append, type, 0);
			}
		}

		if (create_scalar) {
			ret = variable_scalar_set(shell.env, key, value, append, type, 0);
		}

		if (ret) {
			if (errno) { free(value); value = NULL; }
			if (errno == E_NO_MEMORY)					return (free(key), exit_error(E_NO_MEMORY, 1,      "readonly",   NULL, EE_FREE_NONE, EE_RETURN));
			if (errno == E_VAR_IDENTIFIER)				return (exit_error(E_VAR_IDENTIFIER, 1,            "readonly: ", key,  EE_FREE_VAL2, EE_RETURN));
		}

		free(key);
		free(value);
		return (ret);
	}

#pragma endregion

#pragma region "Funcion"

	static int funcion_readonly(t_parse_result *result, char *arg) {
		if (!arg) return (0);

		(void) result;
		int ret = 0;

		return (ret);
	}

#pragma endregion

#pragma region "Readonly"

	int bt_readonly(int argc, char **argv) {
		int ret = 0;
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL,		NO_ARGUMENT, 0}
		};
		
		t_parse_result *result = parse_options(argc, argv, "aAfnp", NULL, long_opts, "readonly [-aAfn] [name[=value] ...] or readonly [-fp]", IGNORE_OFF);
		if (errno || !result) return (free_options(result), (errno == E_OPT_MAX) ? 2 : 1);

		if (find_long_option(result, "help"))		return (free_options(result), bt_readonly_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());

		if (!result->argc) {
			// if (has_option(result, 'f'))	function_print(shell.env, VAR_EXPORTED, SORT_NORMAL, 0);
			// else							variable_print(shell.env, VAR_READONLY, SORT_NORMAL, 0);
											variable_print(shell.env, VAR_READONLY, SORT_NORMAL, 0);
			return (free_options(result), ret);
		}

		for (int i = 0; i < result->argc; ++i) {
			if (has_option(result, 'f'))	ret = funcion_readonly(result, result->argv[i]);
			else							ret = variable_readonly(result, result->argv[i]);
		}

		return (free_options(result), ret);
	}

#pragma endregion
