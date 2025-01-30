/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:40:36 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/30 14:06:40 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//	Caracteres octal y hexadecimal
//	libft array.c usa printf
//	Command -v no es identico a bash
//	Hash no se como añadir manualmente
//	Export readonly variable intentando cambiarla muestra mensaje, pero debe exportarla igual
//	Realizar testeo completo de builtins (todos las opciones)

#pragma region "Includes"

	#include "libft.h"
	#include "terminal/terminal.h"
	#include "terminal/readinput/termcaps.h"
	#include "terminal/readinput/readinput.h"
	#include "terminal/readinput/history.h"
	#include "terminal/signals.h"
	#include "hashes/alias.h"
	#include "hashes/variables.h"
	#include "hashes/builtin.h"
	#include "parser/args.h"
	#include "parser/globbing.h"
	#include "parser/token.h"
	#include "builtins/builtins.h"
	#include "main/options.h"
	#include "main/shell.h"
	#include "main/error.h"
	#include "tests/tests.h"

#pragma endregion

#pragma region "Varios"

	void execute_commands(char *input) {
		if (!input || ft_isspace_s(terminal.input)) return;

		t_arg *args = test_create_args(input);
		t_arg *current = args;
		t_arg *next_command = NULL;
		t_arg *subcommand_args = NULL;

		while (current) {
			if (current->value && current->value[0] == ';') {
				next_command = current->next;
				current->next = NULL;
				args_clear(&args);
				args = next_command;
				current = next_command;
			} else if (current->next && current->next->value && current->next->value[0] == ';') {
				next_command = current->next;
				current->next = NULL;

				globbing(args);
				subcommand_args = args;
				builtin_exec(subcommand_args);
				args_clear(&subcommand_args);
				args = next_command;
				current = next_command;
			} else	current = current->next;
		}

		if (args) {
			globbing(args);
			builtin_exec(args);
			args_clear(&args);
		}
	}

	int read_input() {
		signals_set();

		if (!(terminal.input = readinput(NULL))) return (1);

		history_add(terminal.input, false);

		execute_commands(terminal.input);
		sfree(terminal.input);
		//first_step();

		return (0);
	}

#pragma endregion

#pragma region "Initialize"

	static int initialize(int argc, const char **argv, const char **envp) {
		(void) argc; (void) argv;

		//	uid, euid
		//	PS1, PS2
		//	column, row
		terminal_initialize();
		builtin_initialize();
		options_initialize();
		alias_initialize();
		variables_initialize(vars_table, envp);
		shell_initialize();
		//	Execute ~/.42shrc
		history_initialize();
		return (0);
	}

#pragma endregion

#pragma region "Main"

	int main(int argc, const char **argv, const char **envp) {
		if (initialize(argc, argv, envp)) exit_error(NOTHING, 1, NULL, true);
		if (tests(argc, argv, envp)) exit_error(NOTHING, 0, NULL, true);

		if (argc == 2 && !ft_strcmp(argv[1], "-c"))
			exit_error(START_ARGS, 2, NULL, true);
		else if (argc > 1 && ft_strcmp(argv[1], "-c")) ;
			//argument_error(&data, argv[1]);
		else if (argc > 2 && !ft_strcmp(argv[1], "-c")) {
			signals_set();
			shell._inline = true;
			terminal.input = ft_strdup(argv[2]);
			execute_commands(terminal.input);
			sfree(terminal.input);
		} else {
			t_arg arg = { .value = "banner" }; builtin_exec(&arg);
			while (!shell.exit && !read_input()) ;
		}

		if (nsignal) shell.exit_code = 128 + nsignal;
		exit_error(END, 0, NULL, true);
	}

#pragma endregion
