/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 15:15:32 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/25 20:44:17 by vzurera-         ###   ########.fr       */
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

	void lexer_init(char *input) {
		stack_init();
		if (lexer.input) free(lexer.input);
		lexer.input = ft_strdup(input);
		lexer.pos = 0;
		lexer.len = ft_strlen(input);
		lexer.more_input = false;
	}

	void lexer_append_input(char *input) {
		size_t new_len = ft_strlen(input);

		lexer.input = realloc(lexer.input, lexer.len + new_len + 2);
		lexer.input[lexer.len] = '\n';
		strcpy(lexer.input + lexer.len + 1, input);
		lexer.len += new_len + 1;
		lexer.more_input = false;
	}

	int lexer_more_input() {
		return (lexer.more_input);
	}

	void lexer_free() {
		free(lexer.input);
		free(lexer.stack);
		lexer.input = NULL;
		lexer.stack = NULL;
	}

#pragma endregion

#pragma region "Navigation"

	size_t lexer_pos() {
		return (lexer.pos);
	}

	size_t lexer_len() {
		return (lexer.len);
	}

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

	int skip_whitespace() {
		int had_space = false;

		if (stack_top() == '"' || stack_top() == '\'') return (false);

		while (peek(0) == ' ' || peek(0) == '\t') {
			had_space = true;
			advance(1);
		}

		return (had_space);
	}

	int is_space(int n) {
		if (n < 0 && (int)lexer.pos < n) return (false);
		if (lexer.pos + n >= lexer.len) return (true);

		char c = lexer.input[lexer.pos + n];
		return (c == ' ' || c == '\t' || c == '\n' || c == '\0');
	}

#pragma endregion

#pragma region "Token"

	void lexer_token_free(t_token *token) {
		if (token) {
			free(token->value);
			free(token);
		}
	}

	t_token *lexer_token_create(t_token_type type, char *value, int left_space, int right_space) {
		t_token *token = malloc(sizeof(t_token));

		token->type = type;
		token->value = value;
		token->had_left_space = left_space;
		token->had_right_space = right_space;

		return (token);
	}

	t_token *lexer_token_next() {
		// int had_left_space = skip_whitespace();

		skip_whitespace();

		char c = peek(0);
		char next = peek(1);

		if (((c == '\\' && (next == '\n' || next == '\0')) || (c == '\n' && next == '\0')) || c == '\0') {
			if (lexer.stack_size > 0 || c == '\\') {
				advance(1);
				lexer.more_input = true;
				return (lexer_token_create(TOKEN_INPUT, NULL, false, false));
			}
			if (c == '\n') advance(1);
			return (lexer_token_create(TOKEN_EOF, NULL, false, false));
		}

		t_token *token = NULL;
		// if ((token = expansion()))		return (token);
		// if ((token = grouping()))		return (token);
		// if ((token = operator()))		return (token);
		// if ((token = redirection()))	return (token);
		// if ((token = keyword()))		return (token);
		if ((token = word()))			return (token);

		// int start = lexer.pos;
		// while ((c != ' ' && c != '\t' && c != '\0')) {
		// 	advance(1);
		// 	c = peek(0);
		// }

		// return (lexer_token_create(TOKEN_WORD, ft_substr(lexer.input, start, lexer.pos - start), had_left_space, false));
		return (lexer_token_create(TOKEN_EOF, NULL, false, false));
	}

#pragma endregion
