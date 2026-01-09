/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:09:33 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/09 10:40:32 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "hashes/builtin.h"
	#include "main/shell.h"
	#include "utils/utils.h"
	#include "utils/getopt.h"

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Help"

		int bt_pwd_help(int format, int no_print) {
			char *name = "pwd";
			char *syntax = "pwd: pwd [-LP]";
			char *description = "Print the name of the current working directory.";
			char *msg =
				"    opts:\n"
				"      -L        print the value of $PWD if it names the current working\n"
				"                directory\n"
				"      -P        print the physical directory, without any symbolic links\n\n"

				"    By default, `pwd' behaves as if `-L' were specified.\n\n"

				"    Exit Status:\n"
				"      Returns 0 unless an invalid option is given or the current directory\n"
				"      cannot be read.\n";

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
				"pwd 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by "DEVELOPER" ("LOGIN42").\n";

			print(STDOUT_FILENO, msg, RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Pwd"

	int bt_pwd(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, "LP", NULL, long_opts, "pwd [-LP]", IGNORE_OFF);
		if (!result)		return (1);
		if (result->error)	return (free_options(result), 2);

		if (find_long_option(result, "help"))		return (free_options(result), bt_pwd_help(HELP_NORMAL, 0));
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
