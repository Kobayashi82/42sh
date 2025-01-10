/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:40:36 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/10 14:22:31 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

//	Caracteres octal y hexadecimal

#pragma region Variables

	t_token	tokens;

#pragma endregion

#pragma region Varios

	int read_input() {
		signals_set();
		if (!(terminal.input = readinput(NULL))) return (1);
		if (fake_segfault) { write(2, "Segmentation fault (core dumped)\n", 34); fake_segfault = false; }
		else if (terminal.input) {
			history_add(terminal.input, false);
			printf("%s\n", terminal.input);
		}
		first_step();
		safe_free(terminal.input);

		return (0);
	}

#pragma endregion

#pragma region Main

	int main(int argc, const char **argv, const char **envp) {
		if (initialize(argc, argv, envp)) return (1);
		if (tests(argc, argv, envp)) return (shell.exit_code);

		if (argc == 2 && !ft_strcmp(argv[1], "-c"))
			exit_error(START_ARGS, 2, NULL, true);
		else if (argc > 1 && ft_strcmp(argv[1], "-c")) ;
			//argument_error(&data, argv[1]);
		else if (argc > 2 && !ft_strcmp(argv[1], "-c")) {
			signals_set();
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
