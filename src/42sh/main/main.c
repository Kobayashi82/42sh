/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:40:36 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/07 22:25:19 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

//	Caracteres octal y hexadecimal

#pragma region Variables

	t_data	data;

#pragma endregion

void testing(char **envp) {
	print_array(envp);
	printf("\n\n\n");
	variables_add_array(main_table, envp);
	variable_add(main_table, "Koba", "pollas", false, true, false);
	variable_add(main_table, "Ala", NULL, true, true, false);
	char **copia = variables_to_array(main_table, INTERNAL);
	sort_array(copia);
	print_array(copia);
	free_array(copia);
}

#pragma region Varios

	int shell_time() { return (time(NULL) - data.shell.started); }

	int read_input() {
		initialize_signals();

		// if (!(data.terminal.input = readinput(BLUE600"["GREEN600"kobayashi"BLUE600"]"GREEN600"-"RED600"42"Y"sh"BLUE600"> "NC))) return (1);
		if (!(data.terminal.input = readinput(NULL))) return (1);
		if (fake_segfault) { write(2, "Segmentation fault (core dumped)\n", 34); fake_segfault = false; }
		else if (data.terminal.input) printf("%s\n", data.terminal.input);
		free(data.terminal.input);

		return (0);
	}

#pragma endregion

#pragma region Initialize

	static int initialize(int argc, char **argv, char **envp) {
		(void) argc;
		(void) argv;
		(void) envp;
		data.shell.pid = getpid();
		data.shell.started = time(NULL);
		srand(data.shell.started);
		data.shell.bk_std.bk_stdin = safe_dup(STDIN_FILENO);
		data.shell.bk_std.bk_stdout = safe_dup(STDOUT_FILENO);
		data.shell.bk_std.bk_stderr = safe_dup(STDERR_FILENO);
		return (0);
	}

#pragma endregion

#pragma region Main

	int main(int argc, char **argv, char **envp) {
		if (initialize(argc, argv, envp)) return (1);

		testing(envp);
		return (data_free(), 0);
		if (argc == 2 && !ft_strcmp(argv[1], "-c"))
			exit_error(START_ARGS, 2, NULL, true);
		else if (argc > 1 && ft_strcmp(argv[1], "-c")) ;
			//argument_error(&data, argv[1]);
		else if (argc > 2 && !ft_strcmp(argv[1], "-c")) {
			initialize_signals();
			data.shell._inline = true;
			data.terminal.input = safe_strdup(argv[2]);
			if (ft_strlen(data.terminal.input)) printf("inline\n");
		} else {
			banner();
			while (!data.shell.exit && !read_input()) ;
		}

		if (data.terminal.signal) data.shell.exit_code = 128 + data.terminal.signal;
		return (data_free(), data.shell.exit_code % 256);
	}

#pragma endregion
