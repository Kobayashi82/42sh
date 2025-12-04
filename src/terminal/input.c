/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:02:36 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/04 20:13:41 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Sintax tests
//
//	alias echo=date c=lala d=lolo b='lele ' f=lili g=h h='j b d ' j='h'
//	c ; d ; (g) d d | f $(( (b) $(echo `b g` $((b * 3)) ) + $(b b 3 | wc -l) )) || d

#pragma region "Includes"

	#include "terminal/terminal.h"
	#include "terminal/readinput/prompt.h"
	#include "terminal/readinput/readinput.h"
	#include "terminal/readinput/history.h"
	#include "main/options.h"
	#include "main/shell.h"
	#include "parser/parser.h"
	#include "utils/libft.h"

	#include <stdio.h>	// borrar

#pragma endregion

#pragma region "Variables"

	static int fd;

#pragma endregion

#pragma region "More Input"

	static char *more_input() {
		if (shell.source == SRC_INTERACTIVE) {
			char *input = readinput(prompt_PS2);
			expand_history(&input, 1);
			return (input);
		}
		
		return (get_next_line(fd));
	}

#pragma endregion

#pragma region "No Interactive"

	int no_interactive_input(char *value) {
		char *filename = NULL;
		fd = STDIN_FILENO;

		if (shell.source == SRC_FILE) {
			if (value) fd = open(value, O_RDONLY);
			if (!value || fd == -1) {
				// error
				return (1);
			}
			filename = value;
		}

		if (shell.source == SRC_ARGUMENT) {
			int fd_pipe[2];
			if (!value || pipe(fd_pipe) == -1) {
				// error
				return (1);
			}
			fd = fd_pipe[0];
			write(fd_pipe[1], value, ft_strlen(value));
			close(fd_pipe[1]);
		}

		char *input = get_next_line(fd);
		if (!input) {
			// error
			if (shell.source != SRC_STDIN) close(fd);
			return (1);
		}

		shell.ast = parse(input, more_input, 0, filename, 1);
		if (shell.source != SRC_STDIN) close(fd);

		ast_print(shell.ast);
		printf("\n");

		return (0);
	}

#pragma endregion

#pragma region "Interactive"

	int interactive_input() {
		char *input = NULL;

		input = readinput(prompt_PS1);
		if (!input)							return(1);
		if (ft_isspace_s(input))			return(free(input), 0);

		expand_history(&input, 1);
		shell.ast = parse(input, more_input, 1, NULL, 1);

		ast_print(shell.ast);
		printf("\n");
		
		return (0);
	}

#pragma endregion
