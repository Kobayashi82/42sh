/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shopt.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 12:38:13 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/07 16:32:27 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "hashes/builtin.h"
	#include "main/options.h"
	#include "utils/libft.h"
	#include "utils/print.h"
	#include "utils/getopt.h"

#pragma endregion

#pragma region Info

	// shopt: Muestra las opciones de configuración actuales del shell
	//
	// shopt -o: Muestra o aplica a las opciones que existen en set
	// shopt -q: No muestra nada en la salida standard
	// shopt -s <opción>: Habilita una opción
	// shopt -u <opción>: Deshabilita una opción
	// shopt -p: Muestra las opciones con la sintaxis de shopt que puedes usar para restablecer la configuración

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Help"

		int bt_shopt_help(int format, int no_print) {
			char *name = "shopt";
			char *syntax = "shopt [-pqsu] [-o] [optname ...]";
			char *description = "Set and unset shell options.";
			char *msg =
				"    Change the setting of each shell option OPTNAME.  Without any option\n"
				"    arguments, list each supplied OPTNAME, or all shell options if no\n"
				"    OPTNAMEs are given, with an indication of whether or not each is set.\n\n"

				"    Options:\n"
				"      -o        restrict OPTNAMEs to those defined for use with `set -o'\n"
				"      -p        print each shell option with an indication of its status\n"
				"      -q        suppress output\n"
				"      -s        enable (set) each OPTNAME\n"
				"      -u        disable (unset) each OPTNAME\n\n"

				"    Exit Status:\n"
				"      Returns success if OPTNAME is enabled; fails if an invalid option is\n"
				"      given or OPTNAME is disabled.\n";

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
				"shopt 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by Kobayashi82 (vzurera-).\n";

			print(STDOUT_FILENO, msg, RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Unset"

	int bt_shopt(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, "pqsuo", NULL, long_opts, "shopt [-pqsu] [-o] [optname ...]", IGNORE_OFF);
		if (!result)		return (1);
		if (result->error)	return (free_options(result), 2);

		if (find_long_option(result, "help"))		return (free_options(result), bt_shopt_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());


		int ret = 0;

		if (!result->argc) {
			print(STDOUT_FILENO, "main\n", RESET_PRINT);
		}

		return (free_options(result), ret);











		// t_opt *opts = parse_options_old(args, "pqsuo", '-', 0);

		// if (*opts->invalid) {
		// 	invalid_option("shopt", opts->invalid, "[-pqsu] [-o] [optname ...]");
		// 	return (free(opts), 1);
		// }

		// if (strchr(opts->valid, '?')) return (free(opts), bt_shopt_help(HELP_NORMAL, 0));
		// if (strchr(opts->valid, '#')) return (free(opts), version());

		// int result = 0;
		// char *values = NULL, *invalues = NULL;

		// if (!opts->args) {
		// 	values = options_print(NULL, SHOPT);
		// } else {
		// 	while (opts->args) {
		// 		char *opt_str = options_print(opts->args->value, SHOPT);
		// 		if (opt_str)
		// 			values = ft_strjoin(values, opt_str, 3);
		// 		else {
		// 			invalues = ft_strjoin_sep(invalues, "shopt: ", opts->args->value, 1);
		// 			invalues = ft_strjoin(invalues, ": invalid shell option name\n", 1);
		// 		}
		// 		opts->args = opts->args->next;
		// 	}
		// }

		// if (values) { print(STDOUT_FILENO, values, RESET_PRINT); free(values); }
		// if (invalues) { print(STDERR_FILENO, invalues, RESET_PRINT); free(invalues); }

		// return (free(opts), result);
	}

#pragma endregion
