/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 15:15:32 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/25 00:12:36 by vzurera-         ###   ########.fr       */
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

	typedef struct s_lexer {
		char	*input;
		size_t	pos;
		size_t	len;
		int		more_input;

		char	*stack;				// ", ', ), }
		size_t	stack_size;
		size_t	stack_capacity;
	} t_lexer;

	static t_lexer lexer;

#pragma endregion

#pragma region "Stack"

	static void stack_init() {
		if (lexer.stack) free(lexer.stack);
		lexer.stack_capacity = 64;
		lexer.stack_size = 0;
		lexer.stack = malloc(lexer.stack_capacity);
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
		return (lexer.stack[--lexer.stack_size]);
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

static char peek() {
	if (lexer.pos >= lexer.len) return ('\0');
	return (lexer.input[lexer.pos]);
}

static char peek_ahead(size_t n) {
	if (lexer.pos + n >= lexer.len) return ('\0');
	return (lexer.input[lexer.pos + n]);
}

char peek_back(size_t n) {
	if (lexer.pos < n) return ('\0');
	return (lexer.input[lexer.pos - n]);
}

static void advance() {
	if (lexer.pos < lexer.len) lexer.pos++;
}

void advance_ahead(size_t n) {
	lexer.pos += (lexer.pos + n < lexer.len) ? n : (lexer.len - 1) - lexer.pos;
}

static int skip_whitespace() {
	int had_space = false;

	if (stack_top() == '"' || stack_top() == '\'') return (false);

	while (peek() == ' ' || peek() == '\t') {
		had_space = true;
		advance();
	}

	return (had_space);
}

#pragma endregion

#pragma region "Token"

	void lexer_token_free(t_token *token) {
		if (token) {
			free(token->value);
			free(token);
		}
	}

	static t_token *create_token(t_token_type type, char *value, int left_space, int is_quoted) {
		t_token *token = malloc(sizeof(t_token));

		token->type = type;
		token->value = value;
		token->had_left_space = left_space;
		token->had_right_space = (peek() == ' ' || peek() == '\t');
		token->quoted = is_quoted;

		return (token);
	}

	t_token *lexer_token_next() {
		int had_left_space = skip_whitespace();

		char c = peek();
		char next = peek_ahead(1);

		if (((c == '\\' && (next == '\n' || next == '\0')) || (c == '\n' && next == '\0')) || c == '\0') {
			if (lexer.stack_size > 0 || c == '\\') {
				advance();
				lexer.more_input = true;
				return (NULL);
			}
			if (c == '\n') advance();
			return (create_token(TOKEN_EOF, NULL, had_left_space, false));
		}

		int start = lexer.pos;
		while ((c != ' ' && c != '\t' && c != '\0')) {
			advance();
			c = peek();
		}

		return (create_token(TOKEN_WORD, ft_substr(lexer.input, start, lexer.pos - start), had_left_space, false));
	}

#pragma endregion
