/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:40:36 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/28 22:21:32 by vzurera-         ###   ########.fr       */
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

	#include "utils/libft.h"
	#include "terminal/terminal.h"
	#include "terminal/readinput/prompt.h"
	#include "terminal/readinput/history.h"
	#include "terminal/signals.h"
	#include "hashes/alias.h"
	#include "hashes/variables.h"
	#include "hashes/builtin.h"
	#include "parser/lexer.h"
	#include "parser/args.h"
	#include "expansion/globbing.h"
	#include "builtins/builtins.h"
	#include "main/options.h"
	#include "main/shell.h"
	#include "main/error.h"
	#include "tests/tests.h"

	#include "parser/parser.h"
	#include "terminal/colors.h"

#pragma endregion

#pragma region "Read Input"

	int read_input() {
		signals_set();

		if (interactive_input()) return (1);

		if (!shell.ast || shell.ast->type == TOKEN_EOF) {
			lexer_free();							// esto sobra
			ast_free(&shell.ast);
		}

		if (lexer.input) {
			if (!strcmp(lexer.input, "$?"))	printf("Exit code: %d\n", shell.exit_code);
			else							printf("Input: %s\n", lexer.input);

			t_arg *args = test_create_args(lexer.input);
			lexer_free();							// temporal porque no se usa el arbol AST
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

		if (no_interactive_input(value))	return (1);
		if (!lexer.input)					return (1);		// esto sobra

		if (!shell.ast || shell.ast->type == TOKEN_EOF) {
			lexer_free();							// esto sobra
			ast_free(&shell.ast);
		}

		if (lexer.input) {
			if (!strcmp(lexer.input, "$?"))	printf("Exit code: %d\n", shell.exit_code);
			else							printf("Input: %s\n", lexer.input);
			
			t_arg *args = test_create_args(lexer.input);
			lexer_free();							// temporal porque no se usa el arbol AST
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

#pragma region "Initialize"

	static int initialize(int argc, const char **argv, const char **envp) {
		(void) argc; (void) argv;

		//	uid, euid
		//	PS1, PS2
		//	column, row
		builtin_initialize();
		options_initialize();
		alias_initialize();
		variables_initialize(vars_table, envp);
		prompt_initialize();
		shell_initialize();
		//	Execute ~/.42shrc
		history_initialize();
		return (0);
	}

#pragma endregion

#pragma region "Main"

	int main(int argc, const char **argv, const char **envp) {
		if (initialize(argc, argv, envp))	exit_error(NOTHING, 1, NULL, true);
		if (tests(argc, argv, envp))		exit_error(NOTHING, 0, NULL, true);

		if (argc == 2 && !strcmp(argv[1], "-c"))
			exit_error(START_ARGS, 2, NULL, true);
		else if (argc > 1 && strcmp(argv[1], "-c")) {
			//argument_error(&data, argv[1]);
			exit_error(END, 0, NULL, true);
		} else if (argc > 2 && !strcmp(argv[1], "-c")) {
			signals_set();
			shell.source = SRC_ARGUMENT;
			read_input_args((char *)argv[2]);
			// Aqui tiene que crear el arbol AST sin abrir contextos, error si estan sin cerrar
			// terminal.input = expand_input(ft_strdup(argv[2]));
			// execute_commands(terminal.input);
			// free(terminal.input);
		} else {
			shell.source = SRC_INTERACTIVE;
			if (!isatty(STDIN_FILENO)) {
				// error, no stdin
				exit_error(END, 0, NULL, true);
			}
			//t_arg arg = { .value = "banner" }; builtin_exec(&arg);
			while (!shell.exit && !read_input()) ;
		}

		if (nsignal) shell.exit_code = 128 + nsignal;
		exit_error(END, 0, NULL, true);
	}

#pragma endregion
