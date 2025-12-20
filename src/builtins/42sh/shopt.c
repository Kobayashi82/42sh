/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shopt.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 12:38:13 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/20 12:48:05 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "utils/libft.h"
	#include "utils/print.h"
	#include "tests/args.h"
	#include "builtins/builtins.h"
	#include "builtins/options.h"
	#include "main/options.h"

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

#pragma region "Help"

	static int print_help() {
		char *msg =
		"shopt: shopt [-pqsu] [-o] [optname ...]\n"
		"    Set and unset shell options.\n\n"

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

		print(STDOUT_FILENO, msg, RESET_PRINT);

		return (0);
	}

#pragma endregion

#pragma region "Unset"

	int bt_shopt(t_arg *args) {
		t_opt *opts = parse_options(args, "pqsuo", '-', 0);

		if (*opts->invalid) {
			invalid_option("shopt", opts->invalid, "[-pqsu] [-o] [optname ...]");
			return (free(opts), 1);
		}

		if (strchr(opts->valid, '?')) return (free(opts), print_help());
		if (strchr(opts->valid, '#')) return (free(opts), print_version("shopt", "1.0"));

		int result = 0;
		char *values = NULL, *invalues = NULL;

		if (!opts->args) {
			values = options_print(NULL, SHOPT);
		} else {
			while (opts->args) {
				char *opt_str = options_print(opts->args->value, SHOPT);
				if (opt_str)
					values = ft_strjoin(values, opt_str, 3);
				else {
					invalues = ft_strjoin_sep(invalues, "shopt: ", opts->args->value, 1);
					invalues = ft_strjoin(invalues, ": invalid shell option name\n", 1);
				}
				opts->args = opts->args->next;
			}
		}

		if (values) { print(STDOUT_FILENO, values, RESET_PRINT); free(values); }
		if (invalues) { print(STDERR_FILENO, invalues, RESET_PRINT); free(invalues); }

		return (free(opts), result);
	}

#pragma endregion
