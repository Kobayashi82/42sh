/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:40:36 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/10 21:05:44 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

//	Caracteres octal y hexadecimal

#pragma region Variables

	t_shell		shell;
	t_terminal	terminal;
	t_tokens	tokens;

#pragma endregion

#pragma region Varios

	int shell_time() { return (time(NULL) - shell.started); }

	int read_input() {
		initialize_signals();

		// if (!(terminal.input = readinput(BLUE600"["GREEN600"kobayashi"BLUE600"]"GREEN600"-"RED600"42"Y"sh"BLUE600"> "NC))) return (1);
		if (!(terminal.input = readinput(NULL))) return (1);
		if (fake_segfault) { write(2, "Segmentation fault (core dumped)\n", 34); fake_segfault = false; }
		else if (terminal.input) printf("%s\n", terminal.input);
		free(terminal.input);

		return (0);
	}

#pragma endregion

#pragma region Initialize

	void shell_initialize() {
		shell.pid = getpid();
		shell.parent_pid = getppid();
		shell.subshell_level = 0;
		shell.started = time(NULL);
		srand(shell.started);
		terminal.bk_stdin = safe_dup(STDIN_FILENO);
		terminal.bk_stdout = safe_dup(STDOUT_FILENO);
		terminal.bk_stderr = safe_dup(STDERR_FILENO);
	}

	static int initialize(int argc, char **argv, char **envp) {
		(void) argc; (void) argv; (void) envp;

		//	uid, euid
		//	PS1, PS2
		//	column, row
		variables_initialize(main_table);
		return (0);
	}

#pragma endregion

#pragma region Main

	int main(int argc, char **argv, char **envp) {
		if (initialize(argc, argv, envp)) return (1);
		if (tests(argc, argv, envp)) return (shell.exit_code);

		if (argc == 2 && !ft_strcmp(argv[1], "-c"))
			exit_error(START_ARGS, 2, NULL, true);
		else if (argc > 1 && ft_strcmp(argv[1], "-c")) ;
			//argument_error(&data, argv[1]);
		else if (argc > 2 && !ft_strcmp(argv[1], "-c")) {
			initialize_signals();
			shell._inline = true;
			terminal.input = safe_strdup(argv[2]);
			if (ft_strlen(terminal.input)) printf("inline\n");
		} else {
			banner();
			while (!shell.exit && !read_input()) ;
		}

		if (terminal.signal) shell.exit_code = 128 + terminal.signal;
		return (data_free(), shell.exit_code % 256);
	}

#pragma endregion
