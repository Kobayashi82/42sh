/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 11:01:35 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/29 00:33:32 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "hashes/builtin.h"
	#include "main/shell.h"
	#include "utils/libft.h"
	#include "utils/print.h"

	#include <getopt.h>

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Help"

		static int help() {
			char *msg =
				"builtin: builtin [shell-builtin [arg ...]]\n"
				"    Execute shell builtins.\n\n"

				"    Execute SHELL-BUILTIN with arguments ARGs without performing command\n"
				"    lookup.  This is useful when you wish to reimplement a shell builtin\n"
				"    as a shell function, but need to execute the builtin within the function.\n\n"

				"    Exit Status:\n"
				"      Returns the exit status of SHELL-BUILTIN, or 0 if SHELL-BUILTIN is\n"
				"      not a shell builtin.\n";

			print(STDOUT_FILENO, msg, RESET_PRINT);

			return (0);
		}

	#pragma endregion

	#pragma region "Version"

		static int version() {
			char *msg =
				"builtin 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by Kobayashi82 (vzurera-).\n";

			print(STDOUT_FILENO, msg, RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Builtin"

	int bt_builtin(int argc, char **argv) {
		int opt, option_index = optind = opterr = 0;

		struct option long_options[] = {
			{"help",    no_argument, 0, 0},
			{"version", no_argument, 0, 0},
			{0, 0, 0, 0}
		};

		while ((opt = getopt_long(argc, argv, "", long_options, &option_index)) != -1) {
			switch (opt) {
				case 0  :
					if (!strcmp(long_options[option_index].name, "help"))		return (help());
					if (!strcmp(long_options[option_index].name, "version"))	return (version());
				case '?': {
					char buf[2] = {optopt, '\0'};
					print(STDERR_FILENO, shell.arg0, RESET);
					print(STDERR_FILENO, ft_strjoin_sep(": builtin: -", buf, ": invalid option\n", 0), FREE_JOIN);
					print(STDERR_FILENO, "builtin: usage: builtin [shell-builtin [arg ...]]\n\n", JOIN);
					print(STDERR_FILENO, "Try 'builtin --help' for more information\n", PRINT);
					return (1);
				}
			}
		}

		if (argv[optind]) {
			t_builtin *builtin = builtin_find(argv[optind]);
			if (builtin && !builtin->disabled) {
				return (builtin_exec(argc - optind, &argv[optind]));
			} else {
				print(STDERR_FILENO, shell.arg0, RESET);
				print(STDERR_FILENO, ft_strjoin_sep(": builtin: ", argv[optind], ": not a shell builtin\n", 0), FREE_PRINT);
				return (1);
			}
		}

		return (0);
	}

#pragma endregion
