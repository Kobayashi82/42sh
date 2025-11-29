/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:02:36 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/29 14:21:14 by vzurera-         ###   ########.fr       */
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

	#include "expansion/alias.h"
	#include "expansion/history.h"

	#include "main/options.h"
	#include "main/shell.h"

	#include "parser/lexer.h"
	#include "parser/parser.h"

	#include "utils/libft.h"

	#include <stdio.h>	// borrar

#pragma endregion

static int fd;

#pragma region "Expand History"

	static char *expand_input_history(char *input) {
		if (input) {
			t_context ctx_history;
			memset(&ctx_history, 0, sizeof(t_context));
			expand_history(&input, &ctx_history, 1);
			ctx_stack_clear(&ctx_history.stack);
		}
			
		return (input);
	}

#pragma endregion

// #pragma region "Expand Alias"

// 	static char *expand_input_alias(char *input) {
// 		if (input) {
// 			t_context ctx_alias;
// 			memset(&ctx_alias, 0, sizeof(t_context));
// 			expand_alias(&input, &ctx_alias);
// 			ctx_stack_clear(&ctx_alias.stack);
// 		}

// 		return (input);
// 	}

// #pragma endregion

#pragma region "No Interactive"

	static char *no_interactive_more_input() {
		char *input = get_next_line(fd);

		return (input);
	}

	int no_interactive_input(char *value) {
		fd = STDIN_FILENO;

		if (shell.source == SRC_FILE) {
			if (value) fd = open(value, O_RDONLY);
			if (!value || fd == -1) {
				// error
				return (1);
			}
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
			return(1);
		}

		// if (options.expand_aliases) input = expand_input_alias(input);
		lexer_init(input, no_interactive_more_input);

		shell.ast = parse();

		// lexer_free();	// comentado porque todavia no se usa el arbol AST
		if (shell.source != SRC_STDIN) close(fd);

		return (!shell.ast);
	}

#pragma endregion

static char *interactive_more_input() {
	char *input = readinput(prompt_PS2);

	input = expand_input_history(input);

	return (input);
}

#pragma region "Interactive"

	int interactive_input() {
		char *input = NULL;

		input = readinput(prompt_PS1);
		if (!input)							return(1);
		if (ft_isspace_s(input))			return(free(input), 0);

		input = expand_input_history(input);
		lexer_init(input, interactive_more_input);

		shell.ast = parse();

		history_add(lexer.full_input, 0);

		ast_print(shell.ast);
		printf("\n");

		return (!shell.ast);
	}

#pragma endregion
