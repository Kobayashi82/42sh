/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:40:36 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/30 12:48:45 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "TODO"

	// https://www.drawio.com/

	//	Caracteres octal y hexadecimal
	//	Command -v no es identico a bash (ni lo tiene que ser, pero lo miraré)
	//	Hash no se como añadir manualmente
	//	TMOUT close shell after prompt
	//	interctive variable
	//	ignoreeof of set use "logout" to leave the shell." en stderr
	//	datos por stdin a 42sh se considera script
	//	cdspell effect only in interactive
	//	Argumentos con "" no llegan a los builtins
	// ./42sh -c -cccc "command"

#pragma endregion

#pragma region "Includes"

	#include "terminal/terminal.h"
	#include "main/shell.h"
	#include "utils/libft.h"

	#include "tests/args.h"
	#include "tests/tests.h"

#pragma endregion

#pragma region "Read Input"

	#include "parser/lexer.h"
	#include "expansion/globbing.h"
	#include "builtins/builtins.h"
	#include <stdio.h>

	int read_input() {
		signals_set();

		if (interactive_input()) return (1);
 
		if (shell.ast) {
			if (!strcmp(terminal.input, "$?"))	printf("Exit code: %d\n", shell.exit_code);
			else								printf("Input: %s\n", terminal.input);

			t_arg *args = test_create_args(terminal.input);
			free(terminal.input);
			terminal.input = NULL;
			ast_free(&shell.ast);

			if (args) {
				globbing(args);
				builtin_exec(args);
				args_clear(&args);
			}
		}

		return (0);
	}

#pragma endregion

#pragma region "Read Input Args"

	int read_input_args(char *value) {
		signals_set();

		if (no_interactive_input(value)) return (1);

		if (shell.ast) {
			if (!strcmp(terminal.input, "$?"))	printf("Exit code: %d\n", shell.exit_code);
			else								printf("Input: %s\n", terminal.input);

			t_arg *args = test_create_args(terminal.input);
			free(terminal.input);
			terminal.input = NULL;
			ast_free(&shell.ast);

			if (args) {
				globbing(args);
				builtin_exec(args);
				args_clear(&args);
			}
		}

		return (0);
	}

#pragma endregion

#pragma region "Main"

	int main(int argc, const char **argv, const char **envp) {
		if (initialize(argc, argv, envp))	exit_error(NOTHING, 1, NULL, 1);
		if (tests(argc, argv, envp))		exit_error(NOTHING, 0, NULL, 1);

		if (argc == 2 && !strcmp(argv[1], "-c")) {
			exit_error(START_ARGS, 2, NULL, 1);
		} else if (argc > 2 && !strcmp(argv[1], "-c")) {
			signals_set();
			shell.source = SRC_ARGUMENT;
			read_input_args((char *)argv[2]);
			// Aqui tiene que crear el arbol AST sin abrir contextos, error si estan sin cerrar
			// terminal.input = expand_input(ft_strdup(argv[2]));
			// execute_commands(terminal.input);
			// free(terminal.input);
		} else if (argc > 1 && strcmp(argv[1], "-c")) {
			signals_set();
			shell.source = SRC_FILE;
			read_input_args((char *)argv[1]);
		} else {
			shell.source = SRC_INTERACTIVE;
			if (!isatty(STDIN_FILENO)) {
				// error, no stdin
				exit_error(END, 0, NULL, 1);
			}
			//t_arg arg = { .value = "banner" }; builtin_exec(&arg);
			while (!shell.exit && !read_input()) ;
		}

		if (terminal.signal) shell.exit_code = 128 + terminal.signal;
		exit_error(END, 0, NULL, 1);
	}

#pragma endregion
