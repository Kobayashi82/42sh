/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:02:36 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/26 20:44:46 by vzurera-         ###   ########.fr       */
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

	#include "parser/parser.h"

#pragma endregion

#pragma region "Expand Input"

	static char *expand_input(char *input) {
		t_context ctx_history;	memset(&ctx_history, 0, sizeof(t_context));
		t_context ctx_alias;	memset(&ctx_alias, 0, sizeof(t_context));

		expand_history(&input, &ctx_history, true);
		expand_alias(&input, &ctx_alias);

		stack_clear(&ctx_history.stack);
		stack_clear(&ctx_alias.stack);

		return (input);
	}

#pragma endregion

#pragma region "Readfile"

	static char *readfile() {
		size_t size = 1024, bytes_read = 0;
		char *value = calloc(size + 1, sizeof(char));

		while (1) {
			size_t bytes_to_read = size - bytes_read;
			ssize_t read_now = read(STDIN_FILENO, value + bytes_read, bytes_to_read);

			if (read_now == -1) { free(value); return (NULL); }

			bytes_read += read_now;
			value[bytes_read] = '\0';

			if (bytes_read == size) {
				size *= 2;
				value = realloc(value, size + 1);
			}

			if (read_now == 0) break;
		}

		return (value);
	}

#pragma endregion

#pragma region "Input"

	int get_input() {
		char *input = NULL;

		if (shell.interactive)				input = readinput(prompt_PS1);
		if (!shell.interactive)				input = readfile();
		if (!input)							return(1);
		if (ft_isspace_s(input))			return(free(input), !shell.interactive);

		lexer_init(expand_input(input));

		// Liberar AST en caso de error (en input)
		// Si AST vacío, libera AST y termina el comando
		// Manejar señales correctamente durante el parseo

		t_parser_state	state;
		while ((state = parse(&shell.ast)) == PARSER_INPUT) {
			char *more = readinput(prompt_PS2);
			if (!more) {	// Usuario canceló con Ctrl+C o EOF
				lexer_free();
				ast_free(&shell.ast);
				return (1);
			}
			lexer_append_input(expand_input(more));
		}

		if (shell.interactive) history_add(lexer.input, false);
		// lexer_free(); // comentado porque todavia no se usa el arbol AST

		if (state == PARSER_ERROR) {
			ast_free(&shell.ast);
			return (1);
		}

		ast_print(shell.ast);
		printf("\n");

		return (!shell.interactive);
	}

#pragma endregion
