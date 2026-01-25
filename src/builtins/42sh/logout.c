/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logout.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 12:07:07 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/25 10:59:27 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "internal/shell.h"
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
				"logout 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by "DEVELOPER" ("LOGIN42").\n";

			print(STDOUT_FILENO, msg, P_RESET_PRINT);

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
		if (!result) return (free_options(result), (shell.error == E_OPT_MAX || shell.error == E_OPT_INVALID) ? 2 : 1);

		if (find_long_option(result, "help"))		return (free_options(result), bt_logout_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());


		char *endptr = NULL;
		long number = shell.exit_code;

		if (!shell.login_shell) {
			exit_error(E_EXIT_NO_LOGIN, 1, "logout", NULL, EE_FREE_NONE, EE_RETURN);
			return (free_options(result), 1);
		}

		if (shell.mode == MD_INTERACTIVE && !shell.subshell_level)	print(STDERR_FILENO, "logout\n", P_RESET_PRINT);

		if		(result->argc > 1)									number = exit_error(E_EXIT_ARGS,     1, "exit", NULL, EE_FREE_NONE, EE_RETURN);
		else if (result->argc && !ft_isdigit_s(result->argv[0]))	number = exit_error(E_EXIT_NUMERIC,  2, "exit", NULL, EE_FREE_NONE, EE_RETURN);
		else if (result->argc)										{
			number = strtol(result->argv[0], &endptr, 10);
			if		(errno == ERANGE)								number = exit_error(E_EXIT_OVERFLOW, 2, "exit", NULL, EE_FREE_NONE, EE_RETURN);
			else if (*endptr && endptr != result->argv[0])			number = exit_error(E_EXIT_NUMERIC,  2, "exit", NULL, EE_FREE_NONE, EE_RETURN);
		}

		free_argv_original(result);
		free_options(result);
		exit_error(NOTHING, number % 256, NULL, NULL, EE_FREE_NONE, EE_EXIT);

		return (number);
	}

#pragma endregion

#pragma region "Info"

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

#pragma endregion
