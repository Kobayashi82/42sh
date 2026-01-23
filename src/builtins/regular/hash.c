/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:12:03 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/23 21:43:52 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"
	
	#include "internal/shell.h"
	#include "utils/utils.h"
	#include "utils/getopt.h"

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Help"

		int bt_hash_help(int format, int no_print) {
			char *name = "hash";
			char *syntax = "hash [-lr] [-p pathname] [-dt] [name ...]";
			char *description = "Remember or display program locations.";
			char *msg =
				"    Determine and remember the full pathname of each command NAME.\n"
				"    If no arguments are given, information about remembered commands is displayed.\n\n"

				"    Options:\n"
				"      -d               forget the remembered location of each NAME\n"
				"      -l               display in a format that may be reused as input\n"
				"      -p pathname      use PATHNAME as the full pathname of NAME\n"
				"      -r               forget all remembered locations\n"
				"      -t               print the remembered location of each NAME, preceding\n"
				"                       each location with the corresponding NAME if multiple\n"
				"                       NAMEs are given\n"
				"    Arguments:\n"
				"      NAME             Each NAME is searched for in $PATH and added to the list\n"
				"                       of remembered commands.\n\n"

				"    Exit Status:\n"
				"      Returns success unless NAME is not found or an invalid option is given.\n";

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
				"hash 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by "DEVELOPER" ("LOGIN42").\n";

			print(STDOUT_FILENO, msg, P_RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Hash"

	int bt_hash(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, "lrp:dt", NULL, long_opts, "hash [-lr] [-p pathname] [-dt] [name ...]", IGNORE_OFF);
		if (!result) return (free_options(result), (shell.error == E_OPT_MAX || shell.error == E_OPT_INVALID) ? 2 : 1);

		if (find_long_option(result, "help"))		return (free_options(result), bt_hash_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());


		int ret = 0;
		int clear		= has_option(result, 'r', 0);
		int location	= has_option(result, 't', 0);
		int pathname	= has_option(result, 'p', 0);
		int delete		= has_option(result, 'd', 0);
		int reusable	= has_option(result, 'l', 0);
		// -r > -t > -p > -d > (operación por defecto)

		if (clear) hash_clear();

		if (location) {
			if (!result->argc) {
				print(STDERR_FILENO, ft_strjoin(shell.name, ": hash: -t: option requires an argument\n", J_FREE_NONE), P_FREE_RESET);
				print(STDERR_FILENO, "hash: usage: hash [-lr] [-p pathname] [-dt] [name ...]\n",                       P_PRINT);
				ret = 2;
			} else {
				for (int i = 0; i < result->argc; ++i) {
					if (hash_print(result->argv[i], reusable, result->argc > 1)) {
						ret = exit_error(E_HASH_NOT_FOUND, 1, "hash: ", result->argv[i], EE_FREE_NONE, EE_RETURN);
					}
				}
			}
			return (free_options(result), ret);
		}

		if (pathname) {
			if (!result->argc) {
				print(STDERR_FILENO, ft_strjoin(shell.name, ": hash: -p: option requires a name\n", J_FREE_NONE), P_FREE_RESET);
				print(STDERR_FILENO, "hash: usage: hash [-lr] [-p pathname] [-dt] [name ...]\n",                  P_PRINT);
				ret = 2;
			} else {
				const char *path = get_option_value(result, 'p', 0);
				for (int i = 0; i < result->argc; ++i) {
					if (hash_add_path(result->argv[i], path)) {
						if (shell.error == E_NO_MEMORY) {
							ret = exit_error(E_NO_MEMORY, 1, "hash", NULL, EE_FREE_NONE, EE_RETURN);
							break;
						}
					}
				}
			}
			return (free_options(result), ret);
		} else if (delete) {
			if (!result->argc) {
				print(STDERR_FILENO, ft_strjoin(shell.name, ": hash: -d: option requires an argument\n", J_FREE_NONE), P_FREE_RESET);
				print(STDERR_FILENO, "hash: usage: hash [-lr] [-p pathname] [-dt] [name ...]\n",                       P_PRINT);
				ret = 2;
			} else {
				for (int i = 0; i < result->argc; ++i) {
					if (hash_delete(result->argv[i])) {
						ret = exit_error(E_HASH_NOT_FOUND, 1, "hash: ", result->argv[i], EE_FREE_NONE, EE_RETURN);
					}
				}
			}
			return (free_options(result), ret);
		}

		if (!result->argc) {
			if (hash_print(NULL, reusable, 0)) exit_error(E_HASH_EMPTY, 1, NULL, NULL, EE_FREE_NONE, EE_RETURN);
		} else {
			for (int i = 0; i < result->argc; ++i) {
				if (hash_add(result->argv[i])) {
					if (shell.error == E_NO_MEMORY) {
						ret = exit_error(E_NO_MEMORY, 1, "hash", NULL, EE_FREE_NONE, EE_RETURN);
						break;
					}
					if (shell.error == E_HASH_NOT_FOUND) ret = exit_error(E_HASH_NOT_FOUND, 1, "hash: ", result->argv[i], EE_FREE_NONE, EE_RETURN);
				}
			}
		}

		return (free_options(result), ret);
	}

#pragma endregion
