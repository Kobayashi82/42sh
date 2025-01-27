/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:40:36 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/27 15:53:19 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//	Caracteres octal y hexadecimal
//	libft array.c usa printf

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
	#include "parser/parser.h"
	#include "parser/args.h"
	#include "builtins/builtins.h"
	#include "main/options.h"
	#include "main/shell.h"
	#include "main/clean.h"
	#include "tests/tests.h"

#pragma endregion

#pragma region "Varios"

	int read_input() {
		signals_set();

		if (!(terminal.input = readinput(NULL))) return (1);

		// check syntax
		history_add(terminal.input, false);

		ft_printf(1, "%s\n", terminal.input);
		first_step();

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
			if (ft_strlen(terminal.input)) ft_printf(1, "inline\n");
		} else {
			t_arg arg = { .value = "banner" }; builtin_exec(&arg);
			while (!shell.exit && !read_input()) ;
		}

		if (nsignal) shell.exit_code = 128 + nsignal;
		exit_error(END, 0, NULL, true);
	}

#pragma endregion
