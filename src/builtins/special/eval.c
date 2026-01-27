/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:08:17 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/27 22:19:20 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Aplicar redirecciones permanentes al proceso actual (sin crear un subshell).
// Reemplazar el proceso actual por otro comando (cuando se usa con un comando explícito).

#pragma region "Includes"

	#include "terminal/terminal.h"
	#include "parser/parser.h"
	#include "internal/shell.h"
	#include "utils/utils.h"
	#include "utils/getopt.h"

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Help"

		int bt_eval_help(int format, int no_print) {
			char *name = "eval";
			char *syntax = "eval [arg ...]";
			char *description = " Execute arguments as a shell command.";
			char *msg =
				"    Combine ARGs into a single string, use the result as input to the shell,\n"
				"    and execute the resulting commands.\n\n"

				"    Exit Status:\n"
				"      Returns exit status of command or success if command is null.\n";

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
				"eval 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by "DEVELOPER" ("LOGIN42").\n";

			print(STDOUT_FILENO, msg, P_RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Dirs"

	int bt_eval(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, "", NULL, long_opts, "eval [arg ...]", IGNORE_OFF);
		if (!result) return (free_options(result), (shell.error == E_OPT_MAX || shell.error == E_OPT_INVALID) ? 2 : 1);

		if (find_long_option(result, "help"))		return (free_options(result), bt_eval_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());

		char *command = NULL;
		for (int i = 0; i < result->argc; ++i) {
			command = ft_strjoin_sep(command, result->argv[i], " ", J_FREE_VAL_1);
			if (!command) {
				exit_error(E_NO_MEMORY, 1, "eval", NULL, EE_FREE_NONE, EE_RETURN);
				return (free_options(result), 1);
			}
		}

		if (command) {
			shell.is_eval = 1;
			if (no_interactive_input(command)) {
				free(command);
				return (free_options(result), 1);
			}

			free(command);

			if (shell.ast) {
				if (!strcmp(terminal.input, "$?"))	{
					printf("%d\n", shell.exit_code % 255);
					free(terminal.input);
					terminal.input = NULL;
					ast_free(&shell.ast);
					return (0);
				}
				// else								printf("Input: %s\n", terminal.input);

				free(terminal.input);
				terminal.input = NULL;

				int	 cmd_argc = 0;
				char **cmd_argv = get_argv_from_ast(shell.ast, &cmd_argc);
				ast_free(&shell.ast);

				if (cmd_argc) {
					int ret = builtin_exec(cmd_argc, cmd_argv);
					if (!ret) {
						char *resolved_path = resolve_path(argv[0]);
						if (shell.options.autocd && is_directory(resolved_path)) {
							print(STDOUT_FILENO, ft_strjoin_sep("cd -- ", cmd_argv[0], "\n", J_FREE_NONE), P_FREE_RESET_PRINT);
							char *cd_argv[4] = {"cd", "--", cmd_argv[0], NULL};
							ret = builtin_exec(3, cd_argv);
							if (!ret) shell.exit_code = exit_error(E_CMD_NOT_FOUND, 127, cd_argv[0], NULL, EE_FREE_NONE, EE_RETURN);
						} else {
							shell.exit_code = exit_error(E_CMD_NOT_FOUND, 127, cmd_argv[0], NULL, EE_FREE_NONE, EE_RETURN);
						}
						free(resolved_path);
					}

					// printf("$? = %d\n", shell.exit_code % 255);
				}
				array_free(cmd_argv);
			} else {
				free(terminal.input);
				terminal.input = NULL;
			}
		}

		return (free_options(result), 0);
	}

#pragma endregion
