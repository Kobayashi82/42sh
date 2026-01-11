/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readonly.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:06:39 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/11 20:03:43 by vzurera-         ###   ########.fr       */
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
			char *syntax = "readonly [-aAf] [name[=value] ...] or readonly [-fp]";
			char *description = "Mark shell variables as unchangeable.";
			char *msg =
				"    Mark each NAME as read-only; the values of these NAMEs may not be\n"
				"    changed by subsequent assignment.  If VALUE is supplied, assign VALUE\n"
				"    before marking as read-only.\n\n"

				"    Options:\n"
				"      -a        refer to indexed array variables\n"
				"      -A        refer to associative array variables\n"
				"      -f        refer to shell functions\n"
				"      -p        display a list of all readonly variables or functions,\n"
				"                depending on whether or not the -f option is given\n\n"

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
				"readonly 1.0.\n"
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

	static int add_readonly(t_parse_result *result, char *arg) {
		if (!arg) return (0);
		int ret = 0;

		int type = VAR_READONLY;
		if (has_option(result, 'A')) type = (VAR_ASSOCIATIVE | VAR_READONLY);
		if (has_option(result, 'a')) type = (VAR_ARRAY       | VAR_READONLY);

		if (!strchr(arg, '=')) {
			if (variable_validate(arg)) return (1);
			t_var *var = variable_get(shell.env, arg, 1);
			if (var) {
				var->flags |= type;
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
			print(STDERR_FILENO, ft_strjoin_sep(shell.name, ": readonly: `", key, 3),          FREE_JOIN);
			print(STDERR_FILENO, ft_strjoin_sep("=", value, "': not a valid identifier\n", 2), FREE_JOIN);
			return (1);
		}

		t_var *var = variable_get(shell.env, key, 1);
		if (var && var->flags & VAR_READONLY) {
			print(STDERR_FILENO, ft_strjoin(shell.name, ": readonly: `", 0),   FREE_JOIN);
			print(STDERR_FILENO, ft_strjoin(key, "': readonly variable\n", 1), FREE_JOIN);
			free(value);
			return (1);
		}

		// if		((type & VAR_ASSOCIATIVE)	|| (var && var->flags & VAR_ASSOCIATIVE))	ret = variable_assoc_set(shell.env, key, value, append, type, 0);
		// else if ((type & VAR_ARRAY)			|| (var && var->flags & VAR_ARRAY))			ret = variable_array_set(shell.env, key, value, append, type, 0);
		// else																			ret = variable_scalar_set(shell.env, key, value, append, type, 0);
		ret = variable_scalar_set(shell.env, key, value, append, type, 0);

		free(key);
		free(value);

		return (ret);
	}

#pragma endregion

#pragma region "Readonly"

	int bt_readonly(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, "aAfp", NULL, long_opts, "readonly [-aAf] [name[=value] ...] or readonly [-fp]", IGNORE_OFF);
		if (!result)		return (1);
		if (result->error)	return (free_options(result), 2);

		if (find_long_option(result, "help"))		return (free_options(result), bt_readonly_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());


		int ret = 0;

		if (!result->argc) {
			if (has_option(result, 'f'))	variable_print(shell.env, VAR_EXPORTED, SORT_NORMAL, 0);	// function
			else							variable_print(shell.env, VAR_READONLY, SORT_NORMAL, 0);
			return (free_options(result), 0);
		}

		for (int i = 0; i < result->argc; ++i) {
			if (has_option(result, 'f'))	ret = add_readonly(result, result->argv[i]); // function
			else							ret = add_readonly(result, result->argv[i]);
		}

		return (free_options(result), ret);
	}

#pragma endregion
