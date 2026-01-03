/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:40:36 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/03 22:53:14 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "terminal/terminal.h"
	#include "main/options.h"
	#include "main/shell.h"
	#include "utils/libft.h"
	#include "utils/paths.h"

	#include "tests/args.h"
	#include "tests/tests.h"

#pragma endregion

#pragma region "Args to Argv"

	static char **args_to_argv(t_arg *args, int *argc) {
		int count = 0;

		t_arg *tmp = args;
		while (tmp) {
			if (tmp->value) count++;
			tmp = tmp->next;
		}

		char **argv = malloc(sizeof(char *) * (count + 1));
		if (!argv) return (NULL);
		*argc = count;

		int i = 0;
		tmp = args;
		while (tmp) {
			if (tmp->value) argv[i++] = strdup(tmp->value);
			tmp = tmp->next;
		}
		argv[i] = NULL;

		return (argv);
	}

#pragma endregion

#pragma region "Free Argv"

	static void free_argv(char **argv) {
		if (!argv) return;
		for (int i = 0; argv[i]; ++i) free(argv[i]);
		free(argv);
	}

#pragma endregion

#pragma region "Read Input"

	#include "parser/lexer.h"
	#include "expansion/globbing.h"
	#include "hashes/builtin.h"
	#include <stdio.h>

	int read_input(char *value) {
		signals_set();

		if (shell.source == SRC_INTERACTIVE && interactive_input())			return (1);
		if (shell.source != SRC_INTERACTIVE && no_interactive_input(value))	return (1);
 
		if (shell.ast) {
			if (!strcmp(terminal.input, "$?"))	printf("Exit code: %d\n", shell.exit_code);
			else								printf("Input: %s\n", terminal.input);

			t_arg *args = test_create_args(terminal.input);
			free(terminal.input);
			terminal.input = NULL;
			ast_free(&shell.ast);

			if (args) {
				globbing(args);
				int argc = 0;
				char **argv = args_to_argv(args, &argc);
				builtin_exec(argc, argv);
				args_clear(&args);
				free_argv(argv);
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
		if (initialize(argc, argv, envp))	exit_error(NOTHING, 1, NULL, 1);
		if (tests(argc, argv, envp))		exit_error(NOTHING, 0, NULL, 1);

		if (argc == 2 && !strcmp(argv[1], "-c")) {
			exit_error(START_ARGS, 2, NULL, 1);
		} else if (argc > 2 && !strcmp(argv[1], "-c")) {
			shell.source = SRC_ARGUMENT;
			read_input((char *)argv[2]);
		} else if (argc > 1 && strcmp(argv[1], "-c")) {
			shell.source = SRC_FILE;
			read_input((char *)argv[1]);
		} else if (!isatty(STDIN_FILENO)) {
			shell.source = SRC_STDIN;
			read_input(NULL);
		} else {
			options_set("history", 1);
			options_set("histexpand", 1);
			options_set("expand_aliases", 1);
			options_set("histverify", 0);
			options_set("histreedit", 0);
			shell.source = SRC_INTERACTIVE;
			while (!shell.exit && !read_input(NULL)) ;
		}

		if (terminal.signal) shell.exit_code = 128 + terminal.signal;
		exit_error(END, 0, NULL, 1);
	}

#pragma endregion
