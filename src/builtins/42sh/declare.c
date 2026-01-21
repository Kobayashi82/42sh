/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   declare.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:06:19 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/21 21:55:08 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "internal/shell.h"
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

			static int version_typeset() {
				char *msg =
					"typeset 1.0.\n"
					"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
					"This is free software: you are free to change and redistribute it.\n"
					"There is NO WARRANTY, to the extent permitted by law.\n\n"

					"Written by "DEVELOPER" ("LOGIN42").\n";

				print(STDOUT_FILENO, msg, P_RESET_PRINT);

				return (0);
			}

		#pragma endregion

	#pragma endregion

	#pragma region "Declare"

		#pragma region "Help"

			int bt_declare_help(int format, int no_print) {
				char *name = "declare";
				char *syntax = "declare [-aAfFgiIlnrtux] [name[=value] ...] or declare -p [-aAfFilnrtux] [name ...]";
				char *description = "Set variable values and attributes.";
				char *msg =
					"    Declare variables and give them attributes.  If no NAMEs are given,\n"
					"    display the attributes and values of all variables.\n\n"

					"    Options:\n"
					"      -f        restrict action or display to function names and definitions\n"
					"      -F        restrict display to function names only (plus line number and\n"
					"                source file when debugging)\n"
					"      -g        create global variables when used in a shell function; otherwise\n"
					"                ignored\n"
					"      -I        if creating a local variable, inherit the attributes and value\n"
					"                of a variable with the same name at a previous scope\n"
					"      -p        display the attributes and value of each NAME\n\n"

					"    Options which set attributes:\n"
					"      -a        to make NAMEs indexed arrays (if supported)\n"
					"      -A        to make NAMEs associative arrays (if supported)\n"
					"      -i        to make NAMEs have the `integer' attribute\n"
					"      -l        to convert the value of each NAME to lower case on assignment\n"
					"      -n        make NAME a reference to the variable named by its value\n"
					"      -r        to make NAMEs readonly\n"
					"      -t        to make NAMEs have the `trace' attribute\n"
					"      -u        to convert the value of each NAME to upper case on assignment\n"
					"      -x        to make NAMEs export\n\n"

					"    Using `+' instead of `-' turns off the given attribute.\n\n"

					"    Variables with the integer attribute have arithmetic evaluation (see\n"
					"    the `let' command) performed when the variable is assigned a value.\n\n"

					"    When used in a function, `declare' makes NAMEs local, as with the `local'\n"
					"    command.  The `-g' option suppresses this behavior.\n\n"

					"    Exit Status:\n"
					"      Returns success unless an invalid option is supplied or a variable\n"
					"      assignment error occurs.\n";

				if (!no_print) print(STDOUT_FILENO, NULL, P_RESET);

				if (format == HELP_SYNTAX) {
					print(STDOUT_FILENO, ft_strjoin(name, ": ", 0),   P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin(syntax, "\n", 0), P_FREE_JOIN);
				}

				if (format == HELP_DESCRIPTION) {
					print(STDOUT_FILENO, ft_strjoin(name, " - ", 0),       P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin(description, "\n", 0), P_FREE_JOIN);
				}

				if (format == HELP_NORMAL) {
					print(STDOUT_FILENO, ft_strjoin(name, ": ", 0),                      P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin(syntax, "\n", 0),                    P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin_sep("    ", description, "\n\n", 0), P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin(msg, "\n", 0),                       P_FREE_JOIN);
				}

				if (format == HELP_MANPAGE) {
					print(STDOUT_FILENO, "NAME\n",                                       P_JOIN);
					print(STDOUT_FILENO, ft_strjoin_sep("    ", name, " - ", 0),         P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin(description, "\n\n", 0),             P_FREE_JOIN);
					print(STDOUT_FILENO, "SYNOPSYS\n",                                   P_JOIN);
					print(STDOUT_FILENO, ft_strjoin_sep("    ", syntax, "\n\n", 0),      P_FREE_JOIN);
					print(STDOUT_FILENO, "DESCRIPTION\n",                                P_JOIN);
					print(STDOUT_FILENO, ft_strjoin_sep("    ", description, "\n\n", 0), P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin(msg, "\n\n", 0),                     P_FREE_JOIN);
					print(STDOUT_FILENO, "SEE ALSO\n    42sh(1)\n\n",                    P_JOIN);
				}

				if (!no_print) print(STDOUT_FILENO, NULL, P_PRINT);

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

				print(STDOUT_FILENO, msg, P_RESET_PRINT);

				return (0);
			}

		#pragma endregion

	#pragma endregion

#pragma endregion

#pragma region "Add"

	static int add_declare(char *arg) {
		if (!arg) return (0);

		int ret = 0;

		return (ret);
	}

#pragma endregion

#pragma region "Delete"

	static int delete_declare(char *arg) {
		if (!arg) return (0);

		int ret = 0;

		return (ret);
	}

#pragma endregion

#pragma region "Declare"

	int bt_declare(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		char *syntax = "declare [-aAfFgiIlnrtux] [name[=value] ...] or declare -p [-aAfFilnrtux] [name ...]";
		if (argc && !strcmp(argv[0], "typeset")) syntax = "typeset [-aAfFgiIlnrtux] name[=value] ... or typeset -p [-aAfFilnrtux] [name ...]";

		t_parse_result *result = parse_options(argc, argv, "fFgIpaAilrux", "aAilrux", long_opts, syntax, IGNORE_OFF);
		if (!result)		return (1);

		if (find_long_option(result, "help") && !strcmp(argv[0], "typeset"))	return (free_options(result), bt_typeset_help(HELP_NORMAL, 0));
		if (find_long_option(result, "help"))									return (free_options(result), bt_declare_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version") && !strcmp(argv[0], "typeset"))	return (free_options(result), version_typeset());
		if (find_long_option(result, "version"))								return (free_options(result), version());

		if (!result->argv) {
			variable_print(shell.env, VAR_EXPORTED, SORT_NORMAL, 0);
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
