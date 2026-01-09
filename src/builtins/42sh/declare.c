/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   declare.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:06:19 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/09 16:08:44 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "main/shell.h"
	#include "utils/utils.h"
	#include "utils/getopt.h"

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Typeset"

		#pragma region "Help"

			int bt_typeset_help(int format, int no_print) {
				char *name = "typeset";
				char *syntax = "typeset [-aAfFgiIlnrtux] name[=value] ... or typeset -p [-aAfFilnrtux] [name ...]";
				char *description = "Set variable values and attributes.";
				char *msg = "    A synonym for `declare'.  See `help declare'.\n";

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

			static int version_typeset() {
				char *msg =
					"typeset 1.0.\n"
					"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
					"This is free software: you are free to change and redistribute it.\n"
					"There is NO WARRANTY, to the extent permitted by law.\n\n"

					"Written by "DEVELOPER" ("LOGIN42").\n";

				print(STDOUT_FILENO, msg, RESET_PRINT);

				return (0);
			}

		#pragma endregion

	#pragma endregion

	#pragma region "Declare"

		#pragma region "Help"

			int bt_declare_help(int format, int no_print) {
				char *name = "declare";
				char *syntax = "declare [-ilrux] [name[=value] ...] or declare -p [-ilrux] [name ...]";
				char *description = "Set variable values and attributes.";
				char *msg =
					"    Declare variables and give them attributes.  If no NAMEs are given,\n"
					"    display the attributes and values of all variables.\n\n"

					"    Options:\n"
					"      -p        display the attributes and value of each NAME\n\n"

					"    Options which set attributes:\n"
					"      -i        to make NAMEs have the `integer' attribute\n"
					"      -l        to convert the value of each NAME to lower case on assignment\n"
					"      -r        to make NAMEs readonly\n"
					"      -u        to convert the value of each NAME to upper case on assignment\n"
					"      -x        to make NAMEs export\n\n"

					"    Using `+' instead of `-' turns off the given attribute.\n\n"

					"    Variables with the integer attribute have arithmetic evaluation\n"
					"    performed when the variable is assigned a value.\n\n"

					"    Exit Status:\n"
					"      Returns success unless an invalid option is supplied or a variable assignment\n"
					"      error occurs.\n";

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
					"declare 1.0.\n"
					"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
					"This is free software: you are free to change and redistribute it.\n"
					"There is NO WARRANTY, to the extent permitted by law.\n\n"

					"Written by "DEVELOPER" ("LOGIN42").\n";

				print(STDOUT_FILENO, msg, RESET_PRINT);

				return (0);
			}

		#pragma endregion

	#pragma endregion

#pragma endregion

#pragma region "Add"

	static int add_declare(char *arg) {
		if (!arg) return (0);

		if (!strchr(arg, '=')) {
			if (variables_validate(arg, NULL, 0)) {
				print(STDERR_FILENO, ft_strjoin(shell.env->argv0, ": declare: `", 0),   FREE_JOIN);
				print(STDERR_FILENO, ft_strjoin(arg, "': not a valid identifier\n", 0), FREE_JOIN);
				return (1);
			}
			t_var *var = variables_find(shell.env->table, arg);
			if (var) {
				var->exported = 1;
				return (0);
			}
		}

		char *key = NULL, *value = NULL;
		get_key_value(arg, &key, &value, '=');
		int len = ft_strlen(key);

		int concatenate = 0;
		if (key && len > 0 && key[len - 1] == '+') {
			key[len - 1] = '\0'; concatenate = 1;
		}

		if (variables_validate(key, value, 0)) {
			if (concatenate) key[len - 1] = '+';
			print(STDERR_FILENO, ft_strjoin_sep(shell.env->argv0, ": declare: `", key, 3),     FREE_JOIN);
			print(STDERR_FILENO, ft_strjoin_sep("=", value, "': not a valid identifier\n", 2), FREE_JOIN);
			return (1);
		}

		t_var *var = variables_find(shell.env->table, key);
		if (var && var->readonly) {
			print(STDERR_FILENO, ft_strjoin(shell.env->argv0, ": declare: `", 0), FREE_JOIN);
			print(STDERR_FILENO, ft_strjoin(key, "': readonly variable\n", 1),    FREE_JOIN);
			free(value);
			return (1);
		}

		int ret = 0;

		if (concatenate)	ret = variables_concatenate(shell.env->table, key, value, 1, -1, -1, -1);
		if (!concatenate)	ret = variables_add(shell.env->table, key, value, 1, -1, -1, -1);

		free(key);
		free(value);

		return (ret);
	}

#pragma endregion

#pragma region "Delete"

	static int delete_declare(char *arg) {
		if (!arg) return (0);
		int result = 0;

		if (!strchr(arg, '=')) {
			if (variables_validate(arg, NULL, "declare", 0, 1)) return (1);
			t_var *var = variables_find(shell.env->table, arg);
			if (var) var->exported = 0;
			return (0);
		}

		char *key = NULL, *value = NULL;
		get_key_value(arg, &key, &value, '=');

		int len = ft_strlen(key);
		int concatenate = 0;
		if (key && len > 0 && key[len - 1] == '+') { key[len - 1] = '\0'; concatenate = 1; }

		if (variables_validate(key, value, "declare", 1, 1)) return (free(key), free(value), 1);
		t_var *var = variables_find(shell.env->table, key);
		if (var && var->readonly) {
			print(STDOUT_FILENO, shell.name, RESET);
			print(STDERR_FILENO, ft_strjoin_sep(": ", key, ": readonly variable\n", 0), FREE_PRINT);
			result = 1;
		} else {
			if (concatenate && variables_concatenate(shell.env->table, key, value, 0, -1, -1, -1))	result = 1;
			if (!concatenate && variables_add(shell.env->table, key, value, 0, -1, -1, -1))			result = 1;
		}
		
		return (free(key), free(value), result);
	}

#pragma endregion

#pragma region "Declare"

	int bt_declare(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		char *syntax = "declare [-ilrux] [name[=value] ...] or declare -p [-ilrux] [name ...]";
		if (argc && !strcmp(argv[0], "typeset")) syntax = "typeset [-aAfFgiIlnrtux] name[=value] ... or typeset -p [-aAfFilnrtux] [name ...]";

		t_parse_result *result = parse_options(argc, argv, "pilrux", "ilrux", long_opts, syntax, IGNORE_OFF);
		if (!result)		return (1);
		if (result->error)	return (free_options(result), 2);

		if (find_long_option(result, "help") && !strcmp(argv[0], "typeset"))	return (free_options(result), bt_typeset_help(HELP_NORMAL, 0));
		if (find_long_option(result, "help"))									return (free_options(result), bt_declare_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version") && !strcmp(argv[0], "typeset"))	return (free_options(result), version_typeset());
		if (find_long_option(result, "version"))								return (free_options(result), version());

		if (!result->argv) {
			variables_print(shell.env->table, EXPORTED_LIST, 1);
			return (free_options(result), 1);
		}

		int ret = 0;
		for (int i = 0; result->argv[i]; ++i) {
			if (has_option(result, 'n')) {
				if (delete_declare(result->argv[i])) ret = 1;
			} else if (add_declare(result->argv[i])) ret = 1;
		}

		return (free_options(result), ret);
	}

#pragma endregion

// SIN ACABAR

//	if -i atoi to de operands
// declare patata		crea la variable sin atributos (si ya existe, no le hace cambios)
// declare -irx patata	crea o añade la variable con los atributos
// declare -pirx patata	ignora atributos y muestra patata si existe (prevalece -p)
// declare -pirx		muestra las variables que cumplan con los atributos indicados
// declare -irx			muestra las variables que cumplan con los atributos indicados
// declare -p			-p es opcional, muestra todas las variables
// declare +irx patata	quita los atributos a patata
