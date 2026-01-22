/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shopt.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 12:38:13 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/22 19:47:45 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "internal/shell.h"
	#include "utils/utils.h"
	#include "utils/getopt.h"

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
				"shopt 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by "DEVELOPER" ("LOGIN42").\n";

			print(STDOUT_FILENO, msg, P_RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Shopt"

	int bt_shopt(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, "pqsuo", NULL, long_opts, "shopt [-pqsu] [-o] [optname ...]", IGNORE_OFF);
		if (!result) return (free_options(result), (shell.error == E_OPT_MAX || shell.error == E_OPT_INVALID) ? 2 : 1);

		if (find_long_option(result, "help"))		return (free_options(result), bt_shopt_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());


		int ret = 0;
		int type = has_option(result, 'o', 0) ? O_SET : O_SHOPT;
		int reusable = has_option(result, 'p', 0);
		int suppress = has_option(result, 'q', 0);
		int enable = -1;
		if (!reusable) {
			for (t_opt_value *curr = result->options; curr; curr = curr->next) {
				if (curr->opt == 'u') enable = 0;
				if (curr->opt == 's') enable = 1;
			}
		}

		if (!result->argc) {
			if (options_print(type, reusable, suppress)) {
				if (shell.error == E_NO_MEMORY) exit_error(E_NO_MEMORY, 1, "shopt", NULL, EE_FREE_NONE, EE_RETURN);
				ret = 1;
			}
			return (free_options(result), ret);
		}

		if (enable == -1) {
			for (int i = 0; i < result->argc; ++i) {
				if (option_print(result->argv[i], type, reusable, suppress)) {
					if (shell.error == E_NO_MEMORY) {
						exit_error(E_NO_MEMORY, 1, "shopt", NULL, EE_FREE_NONE, EE_RETURN);
						return (free_options(result), 1);
					}
					if (shell.error == E_SOPT_INVALID) exit_error(E_SOPT_INVALID, 1, "shopt: ", result->argv[i], EE_FREE_NONE, EE_RETURN);
					ret = 1;
				}
			}
		} else {
			for (int i = 0; i < result->argc; ++i) {
				if (option_set(result->argv[i], enable, type)) {
					if (shell.error == E_SOPT_INVALID) exit_error(E_SOPT_INVALID, 1, "shopt: ", result->argv[i], EE_FREE_NONE, EE_RETURN);
					ret = 1;
				}
			}
		}

		return (free_options(result), ret);

	}

#pragma endregion
