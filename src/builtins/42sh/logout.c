/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logout.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 12:07:07 by vzurera-          #+#    #+#             */
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

		int bt_logout_help(int format, int no_print) {
			char *name = "logout";
			char *syntax = "logout [n]";
			char *description = "Exit a login shell.";
			char *msg =
				"    Exits a login shell with exit status N.\n"
				"    Returns an error if not executed in a login shell.\n";

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
				"logout 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by "DEVELOPER" ("LOGIN42").\n";

			print(STDOUT_FILENO, msg, RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Logout"

	int bt_logout(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, NULL, NULL, long_opts, "logout [n]", IGNORE_OFF);
		if (!result)		return (1);
		if (result->error)	return (free_options(result), 2);

		if (find_long_option(result, "help"))		return (free_options(result), bt_logout_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());


		int ret = 0;

		if (!shell.login_shell) {
			print(STDERR_FILENO, ft_strjoin(shell.name, ": logout: not login shell: use `exit'\n", 0), FREE_RESET_PRINT);
			return (free_options(result), 1);
		}

		if (shell.mode == SRC_INTERACTIVE && !shell.subshell_level) print(STDERR_FILENO, "logout\n", RESET_PRINT);

		if (result->argc > 1) {
			print(STDERR_FILENO, ft_strjoin(shell.name, ": logout: too many arguments\n", 0), FREE_RESET_PRINT);
			ret = 1;
		} else if (result->argc && !ft_isdigit_s(result->argv[0])) {
			print(STDERR_FILENO, ft_strjoin(shell.name, ": logout: numeric argument required\n", 0), FREE_RESET_PRINT);
			ret = 2;
		} else if (result->argc) {
			ret = atol(result->argv[0]);
		}

		free_argv_original(result);
		free_options(result);
		exit_error(NOTHING, ret, NULL, 1);

		return (ret);
	}

#pragma endregion

// int login_shell = 0;
// int interactive_shell = 0; // 

// // Detectar si es login shell (llamado como "-42sh" o si pasaron -l o --login)
// if (argv[0][0] == '-' || has_flag("--login") || has_flag("-l")) login_shell = 1;

// if (login_shell) {
//     source_file("/etc/profile");	// 1º (configuración global del sistema)
// 	// El primero disponible de los siguientes
// 	source_file("~/.42sh_profile");	// 2º (preferido por 42sh)
// 	source_file("~/.42sh_login");	// 2º (archivo alternativo)
// 	source_file("~/.profile");		// 2º (POSIX, compatible con todos los shells)
// } else if (interactive_shell) {
// 	source_file("~/.bashrc");		// 1º (configuración interactiva)
// }

// // Builtin logout:
// int bt_logout() {
//     if (!login_shell) {
//         fprintf(stderr, "logout: not login shell: use 'exit'\n");
//         return (1);
//     }

//     source_file("~/.42sh_logout");	// Ejecutar si existe

// 	// Cleanup
//     exit(0);
// }
