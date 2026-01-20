/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:40:36 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/20 21:55:57 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "terminal/terminal.h"
	#include "parser/parser.h"
	#include "main/shell.h"
	#include "utils/utils.h"

#pragma endregion

#pragma region "Read Input"

	int read_input(char *value) {
		signals_set();

		if (shell.mode == MD_INTERACTIVE && interactive_input())			return (1);
		if (shell.mode != MD_INTERACTIVE && no_interactive_input(value))	return (1);
 
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

			int		argc = 0;
			char	**argv = get_argv_from_ast(shell.ast, &argc);
			ast_free(&shell.ast);

			if (argc) {
				int ret = builtin_exec(argc, argv);
				if (!ret) {
					char *resolved_path = resolve_path(argv[0]);
					if (shell.options.autocd && is_directory(resolved_path)) {
						print(STDOUT_FILENO, ft_strjoin_sep("cd -- ", argv[0], "\n", J_FREE_NONE), P_FREE_RESET_PRINT);
						char *cd_argv[4] = {"cd", "--", argv[0], NULL};
						ret = builtin_exec(3, cd_argv);
						if (!ret) shell.exit_code = exit_error(E_CMD_NOT_FOUND, 127, cd_argv[0], NULL, EE_FREE_NONE, EE_RETURN);
					} else {
						shell.exit_code = exit_error(E_CMD_NOT_FOUND, 127, argv[0], NULL, EE_FREE_NONE, EE_RETURN);
					}
					free(resolved_path);
				}

				// printf("$? = %d\n", shell.exit_code % 255);
			}
			array_free(argv);
		} else {
			free(terminal.input);
			terminal.input = NULL;
		}

		return (0);
	}

#pragma endregion

#pragma region "Main"

	int main(int argc, const char **argv, const char **envp) {
		initialize(argc, argv, envp);

		if (argc == 2 && !strcmp(argv[1], "-c")) {
			exit_error(E_START_ARGS, 2, NULL, NULL, EE_FREE_NONE, EE_EXIT);
		} else if (argc > 2 && !strcmp(argv[1], "-c")) {
			shell.mode = MD_ARGUMENT;
			read_input((char *)argv[2]);
		} else if (argc > 1 && strcmp(argv[1], "-c")) {
			shell.mode = MD_FILE;
			read_input((char *)argv[1]);
		} else if (!isatty(STDIN_FILENO)) {
			shell.mode = MD_STDIN;
			read_input(NULL);
		} else {
			options_set("history", 1);
			options_set("histexpand", 1);
			options_set("expand_aliases", 1);
			shell.mode = MD_INTERACTIVE;
			while (!shell.exit && !read_input(NULL)) ;
		}

		if (terminal.signal) shell.exit_code = 128 + terminal.signal;
		exit_error(E_END, 0, NULL, NULL, EE_FREE_NONE, EE_EXIT);
	}

#pragma endregion
