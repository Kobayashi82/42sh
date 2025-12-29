/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enable.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 17:46:30 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/29 14:07:06 by vzurera-         ###   ########.fr       */
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
				"enable: enable [-a] [-nps] [name ...]\n"
				"    Enable and disable shell builtins.\n\n"

				"    Enables and disables builtin shell commands.\n"
				"    Disabling allows you to execute a disk command which has the same name as a\n"
				"    shell builtin without using a full pathname.\n\n"

				"    Options:\n"
				"      -a        print a list of builtins showing whether or not each is enabled\n"
				"      -n        disable each NAME or display a list of disabled builtins\n"
				"      -p        print the list of builtins\n"
				"      -s        print only the names of Posix `special' builtins\n\n"

				"    Without options, each NAME is enabled.\n\n"

				"    Exit Status:\n"
				"      Returns success unless NAME is not a shell builtin or an error occurs.\n";

			print(STDOUT_FILENO, msg, RESET_PRINT);

			return (0);
		}

	#pragma endregion

	#pragma region "Version"

		static int version() {
			char *msg =
				"enable 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by Kobayashi82 (vzurera-).\n";

			print(STDOUT_FILENO, msg, RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Enable"

	int bt_enable(int argc, char **argv) {
		int		opt, option_index = optind = opterr = 0;
		char	opts[32]; opts[0] = '\0';
		int 	opts_len = 0;

		struct option long_options[] = {
			{"help",	no_argument, 0, 0},
			{"version",	no_argument, 0, 0},
			{0, 0, 0, 0}
		};

		while ((opt = getopt_long(argc, argv, "anps", long_options, &option_index)) != -1) {
			switch (opt) {
				case 0  :
					if (!strcmp(long_options[option_index].name, "help"))		return (help());
					if (!strcmp(long_options[option_index].name, "version"))	return (version());
				case '?': {
					char buf[2] = {optopt, '\0'};
					print(STDERR_FILENO, shell.arg0, RESET);
					print(STDERR_FILENO, ft_strjoin_sep(": enable: -", buf, ": invalid option\n", 0), FREE_JOIN);
					print(STDERR_FILENO, "enable: usage: enable [-a] [-nps] [name ...]\n\n", JOIN);
					print(STDERR_FILENO, "Try 'enable --help' for more information\n", PRINT);
					return (1);
				}

				default:
					if (!strchr(opts, opt)) {
						opts[opts_len++] = opt;
						opts[opts_len] = '\0';
					}
			}
		}

		if (strchr(opts, 'a'))																	return (builtin_print(2, 0, 1), 0);
		if (strchr(opts, 'n') && strchr(opts, 's'))												return (builtin_print(1, 1, 1), 0);
		if (strchr(opts, 's'))																	return (builtin_print(2, 1, 1), 0);
		if ((strchr(opts, 'n') && !argv[optind]) || (strchr(opts, 'n') && strchr(opts, 'p')))	return (builtin_print(1, 0, 1), 0);
		if (strchr(opts, 'p'))																	return (builtin_print(0, 0, 1), 0);

		int		result = 0;
		char	*invalues = NULL;
		if (argv[optind]) {
			while (argv[optind]) {
				t_builtin *builtin = builtin_find(argv[optind]);
				if (builtin) {
					if (strchr(opts, 'n'))	builtin->disabled = 1;
					else 					builtin->disabled = 0;
				} else {
					char *value = ft_strjoin_sep(": enable: ", argv[optind], ": not a shell builtin\n", 0);
					if (value) invalues = ft_strjoin_sep(invalues, shell.arg0, value, 6);
					result = 1;
				}
				optind++;
			}
		} else builtin_print(0, 0, 1);

		if (invalues) { print(STDERR_FILENO, invalues, RESET_PRINT); free(invalues); }

		return (result);
	}

#pragma endregion
