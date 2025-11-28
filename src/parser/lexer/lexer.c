/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 15:15:32 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/29 00:18:56 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Include"

	#include "parser/lexer.h"
	#include "utils/libft.h"

#pragma endregion

// Procesar comillas abiertas
// Append del input
// Next token (o advance y peek)
// Comprobación modular de tipos de token
// Determinar si espacio antes/despues y si esta en comillas dobles
// Liberacion de estructura si es necesario
// Dentro de {} no se parsea hasta la ejecucion

#pragma region "Variables"

	t_lexer lexer;

#pragma endregion

#pragma region "Stack"

	// stack contains the token context, with the following meanings:
	//
	// "	""					double quoted
	// '	''					single quoted
	// t	$""					translatable string
	// q	$''					ANSI-C quoting
	// `	``					command substitution
	// s	$(...)				command substitution
	// a	$((...))			arithmetic substitution
	// p	${...}				parameter expression
	// S	(...)				subshell
	// A	((...)	)			arithmetic
	// B	{ ...; } or {...}	braces (command group or brace expansion)

	static void stack_init() {
		if (lexer.stack) free(lexer.stack);
		lexer.stack_capacity = 64;
		lexer.stack_size = 0;
		lexer.stack = calloc(1, lexer.stack_capacity);
	}

	void stack_push(char delim) {
		if (lexer.stack_size >= lexer.stack_capacity) {
			lexer.stack_capacity *= 2;
			lexer.stack = realloc(lexer.stack, lexer.stack_capacity);
		}
		lexer.stack[lexer.stack_size++] = delim;
	}

	char stack_pop() {
		if (!lexer.stack_size) return ('\0');
		lexer.stack[lexer.stack_size] = '\0';
		lexer.stack_size -= 1;
		return (lexer.stack[lexer.stack_size]);
	}

	char stack_top() {
		if (!lexer.stack_size) return ('\0');
		return (lexer.stack[lexer.stack_size - 1]);
	}

#pragma endregion

#pragma region "Input"

	void lexer_init(char *input, t_input_callback callback) {
		stack_init();
		if (lexer.input) free(lexer.input);
		lexer.input = input;
		lexer.pos = 0;
		lexer.len = ft_strlen(input);
		lexer.append_inline = 0;
		lexer.more_input = callback;
	}

	void lexer_append_input() {
		char *input = lexer.more_input();

		if (input) {
			size_t new_len = ft_strlen(input);

			if (lexer.append_inline) {
				lexer.append_inline = 0;
				if (lexer.input && lexer.input[lexer.len - 1] == '\\') {
					lexer.input[lexer.len - 1] = '\0';
					lexer.len--;
				}
				lexer.input = realloc(lexer.input, lexer.len + new_len + 1);
				strcpy(lexer.input + lexer.len, input);
				lexer.len += new_len;
			} else {
				lexer.input = realloc(lexer.input, lexer.len + new_len + 2);
				lexer.input[lexer.len] = '\n';
				strcpy(lexer.input + lexer.len + 1, input);
				lexer.len += new_len + 1;
			}

			free(input);
		}
	}

	void lexer_free() {
		free(lexer.input);
		free(lexer.stack);
		lexer.input = NULL;
		lexer.stack = NULL;
	}

#pragma endregion

#pragma region "Navigation"

	char peek(size_t n) {
		if (lexer.pos + n >= lexer.len) return ('\0');
		return (lexer.input[lexer.pos + n]);
	}

	char peek_back(size_t n) {
		if (lexer.pos < n) return ('\0');
		return (lexer.input[lexer.pos - n]);
	}

	char advance(size_t n) {
		lexer.pos = (lexer.pos + n <= lexer.len) ? lexer.pos + n : lexer.len;
		return (lexer.input[lexer.pos]);
	}

#pragma endregion

#pragma region "Token"

	void token_free(t_token *token) {
		if (token) {
			free(token->value);
			free(token);
		}
	}

	t_token *token_create(t_token_type type, size_t start) {
		t_token *token = malloc(sizeof(t_token));

		token->type = type;
		token->value = (type == TOKEN_EOF) ? NULL : ft_substr(lexer.input, start, lexer.pos - start);
		token->left_space = is_space(lexer.pos - start);
		token->right_space = is_space(0);

		return (token);
	}

	t_token *token_next() {

		if (!stack_top()) while (isspace(peek(0))) advance(1);

		if (peek(0)) {
			t_token *token = NULL;
			// if ((token = expansion()))		return (token);
			// if ((token = grouping()))		return (token);
			// if ((token = operator()))		return (token);
			// if ((token = redirection()))	return (token);
			// if ((token = keyword()))		return (token);
			if ((token = word()))			return (token);
		}

		return (token_create(TOKEN_EOF, 0));
	}

#pragma endregion
