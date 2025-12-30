/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 11:01:35 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/30 14:31:22 by vzurera-         ###   ########.fr       */
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
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, NULL, NULL, long_opts, "builtin [shell-builtin [arg ...]]");
		if (!result)		return (1);
		if (result->error)	return (free_options(result), 1);

		if (find_long_option(result, "help"))		return (free_options(result), help());
		if (find_long_option(result, "version"))	return (free_options(result), version());


		int ret = 0;

		if (result->args) {
			t_builtin *builtin = builtin_find(result->args[0]);
			if (builtin && !builtin->disabled) {
				ret = builtin_exec(result->argc, result->args);
			} else {
				print(STDERR_FILENO, shell.arg0, RESET);
				print(STDERR_FILENO, ft_strjoin_sep(": builtin: ", result->args[0], ": not a shell builtin\n", 0), FREE_PRINT);
				ret = 1;
			}
		}

		return (free_options(result), ret);
	}

#pragma endregion
