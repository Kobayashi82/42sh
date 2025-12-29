/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:09:33 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/29 19:16:56 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "hashes/builtin.h"
	#include "main/shell.h"
	#include "utils/libft.h"
	#include "utils/print.h"
	#include "utils/paths.h"
	#include "utils/getopt2.h"

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Help"

		static int help() {
			char *msg =
				"pwd: pwd [-LP]\n"
				"    Print the name of the current working directory.\n\n"

				"    opts:\n"
				"      -L        print the value of $PWD if it names the current working\n"
				"                directory\n"
				"      -P        print the physical directory, without any symbolic links\n\n"

				"    By default, `pwd' behaves as if `-L' were specified.\n\n"

				"    Exit Status:\n"
				"      Returns 0 unless an invalid option is given or the current directory\n"
				"      cannot be read.\n";

			print(STDOUT_FILENO, msg, RESET_PRINT);

			return (0);
		}

	#pragma endregion

	#pragma region "Version"

		static int version() {
			char *msg =
				"pwd 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by Kobayashi82 (vzurera-).\n";

			print(STDOUT_FILENO, msg, RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "PWD"

	int bt_pwd(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, "LP:", NULL, long_opts, "pwd [-LP]");
		if (!result)		return (1);
		if (result->error)	return (free_options(result), 1);

		if (find_long_option(result, "help"))		return (free_options(result), help());
		if (find_long_option(result, "version"))	return (free_options(result), version());

		int ret = 0;

		if (has_option(result, 'P')) {
			// Modo físico: mostrar ruta real sin symlinks
			char *cwd = get_cwd("cwd");
			if (!cwd)	ret = 1;
			else		print(STDOUT_FILENO, ft_strjoin(cwd, "\n", 1), FREE_RESET_PRINT);
		} else {
			// Modo lógico (por defecto): mostrar PWD del shell
			if (shell.cwd) {
				print(STDOUT_FILENO, ft_strjoin(shell.cwd, "\n", 0), FREE_RESET_PRINT);
			} else {
				print(STDERR_FILENO, "pwd: no se ha encontrado nada\n", RESET_PRINT);
				ret = 1;
			}
		}
		
		return (free_options(result), ret);
	}

#pragma endregion
