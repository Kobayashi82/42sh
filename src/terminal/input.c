/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:02:36 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/27 23:57:09 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Sintax tests
//
//	alias echo=date c=lala d=lolo b='lele ' f=lili g=h h='j b d ' j='h'
//	c ; d ; (g) d d | f $(( (b) $(echo `b g` $((b * 3)) ) + $(b b 3 | wc -l) )) || d

#pragma region "Includes"

	#include "utils/libft.h"
	#include "terminal/readinput/prompt.h"
	#include "terminal/readinput/readinput.h"
	#include "terminal/readinput/history.h"
	#include "terminal/input.h"
	#include "terminal/signals.h"
	#include "expansion/alias.h"
	#include "expansion/history.h"
	#include "main/shell.h"
	#include "parser/syntax.h"

	#include "main/options.h"
	#include "parser/parser.h"

#pragma endregion

#pragma region "Expand History"

	static char *expand_input_history(char *input) {
		t_context ctx_history;
		memset(&ctx_history, 0, sizeof(t_context));
		expand_history(&input, &ctx_history, true);
		stack_clear(&ctx_history.stack);

		return (input);
	}

#pragma endregion

#pragma region "Expand Alias"

	static char *expand_input_alias(char *input) {
		t_context ctx_alias;
		memset(&ctx_alias, 0, sizeof(t_context));
		expand_alias(&input, &ctx_alias);
		stack_clear(&ctx_alias.stack);

		return (input);
	}

#pragma endregion

#pragma region "No Interactive"

	int no_interactive_input(char *value) {
		int fd = STDIN_FILENO;

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

		if (options.expand_aliases) input = expand_input_alias(input);
		lexer_init(input);

		t_parser_state	state;
		while ((state = parse(&shell.ast)) == PARSER_INPUT) {
			char *more = get_next_line(fd);
			if (!more) {
				lexer_free();
				ast_free(&shell.ast);
				if (shell.source != SRC_STDIN) close(fd);
				return (1);
			}
			if (options.expand_aliases) more = expand_input_alias(more);
			lexer_append_input(more);
		}

		if (state == PARSER_ERROR) {
			lexer_free();
			ast_free(&shell.ast);
			if (shell.source != SRC_STDIN) close(fd);
			return (1);
		}

		// lexer_free();	// comentado porque todavia no se usa el arbol AST
		if (shell.source != SRC_STDIN) close(fd);
		return (0);
	}

#pragma endregion

#pragma region "Interactive"

	int interactive_input() {
		char *input = NULL;

		input = readinput(prompt_PS1);
		if (!input)							return(1);
		if (ft_isspace_s(input))			return(free(input), 0);

		input = expand_input_history(input);
		input = expand_input_alias(input);
		lexer_init(input);

		t_parser_state	state;
		while ((state = parse(&shell.ast)) == PARSER_INPUT) {
			char *more = readinput(prompt_PS2);
			if (!more) {	// Usuario canceló con Ctrl+C o EOF
				lexer_free();
				ast_free(&shell.ast);
				return (1);
			}

			more = expand_input_history(more);
			more = expand_input_alias(more);
			lexer_append_input(more);
		}

		history_add(lexer.input, false);
		// lexer_free();	// comentado porque todavia no se usa el arbol AST

		if (state == PARSER_ERROR) {
			ast_free(&shell.ast);
			return (1);
		}

		ast_print(shell.ast);
		printf("\n");

		return (0);
	}

#pragma endregion
