/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enable.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 17:46:30 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/30 14:31:13 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "hashes/builtin.h"
	#include "main/shell.h"
	#include "utils/libft.h"
	#include "utils/print.h"
	#include "utils/getopt2.h"

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
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, "anps", NULL, long_opts, "enable [-a] [-nps] [name ...]");
		if (!result)		return (1);
		if (result->error)	return (free_options(result), 1);

		if (find_long_option(result, "help"))		return (free_options(result), help());
		if (find_long_option(result, "version"))	return (free_options(result), version());


		if (has_option(result, 'a'))																				return (free_options(result), builtin_print(2, 0, 1), 0);
		if (has_option(result, 'n') && has_option(result, 's'))														return (free_options(result), builtin_print(1, 1, 1), 0);
		if (has_option(result, 's'))																				return (free_options(result), builtin_print(2, 1, 1), 0);
		if ((has_option(result, 'n') && (!result->args)) || (has_option(result, 'n') && has_option(result, 'p')))	return (free_options(result), builtin_print(1, 0, 1), 0);
		if (has_option(result, 'p'))																				return (free_options(result), builtin_print(0, 0, 1), 0);

		int		ret = 0;
		char	*invalues = NULL;
		if (result->args) {
			for (int i = 0; result->args[i]; ++i) {
				t_builtin *builtin = builtin_find(result->args[i]);
				if (builtin) {
					if (has_option(result, 'n'))	builtin->disabled = 1;
					else 							builtin->disabled = 0;
				} else {
					char *value = ft_strjoin_sep(": enable: ", result->args[i], ": not a shell builtin\n", 0);
					if (value) invalues = ft_strjoin_sep(invalues, shell.arg0, value, 6);
					ret = 1;
				}
			}
		} else builtin_print(0, 0, 1);

		if (invalues) print(STDERR_FILENO, invalues, FREE_RESET_PRINT);

		return (free_options(result), ret);
	}

#pragma endregion
