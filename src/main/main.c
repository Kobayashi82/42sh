/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:40:36 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/09 21:09:17 by vzurera-         ###   ########.fr       */
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
			if (!strcmp(terminal.input, "$?"))	printf("Exit code: %d\n", shell.exit_code);
			else								printf("Input: %s\n", terminal.input);

			free(terminal.input);
			terminal.input = NULL;

			if (shell.ast) {
				int		argc = 0;
				char	**argv = get_argv_from_ast(shell.ast, &argc);

				int ret = builtin_exec(argc, argv);
				if (!ret) exit_error(CMD_NOT_FOUND, 127, argv[0], 0, EE_RETURN);

				array_free(argv);
			}
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
			exit_error(START_ARGS, 2, NULL, 0, EE_EXIT);
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
		exit_error(END, 0, NULL, 0, EE_EXIT);
	}

#pragma endregion
