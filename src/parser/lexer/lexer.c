/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 15:15:32 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/24 19:35:40 by vzurera-         ###   ########.fr       */
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
		int		needs_more;

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

#pragma region "Lexer"

	void lexer_init(char *input) {
		stack_init();
		if (lexer.input) free(lexer.input);
		lexer.input = ft_strdup(input);
		lexer.pos = 0;
		lexer.len = ft_strlen(input);
		lexer.needs_more = false;
	}

	void lexer_append_input(char *input) {
		size_t new_len = ft_strlen(input);

		lexer.input = realloc(lexer.input, lexer.len + new_len + 2);
		lexer.input[lexer.len] = '\n';
		strcpy(lexer.input + lexer.len + 1, input);
		lexer.len += new_len + 1;
		lexer.needs_more = false;
	}

	int lexer_needs_more() {
		return (lexer.needs_more);
	}

	void lexer_free() {
		free(lexer.input);
		free(lexer.stack);
		lexer.input = NULL;
		lexer.stack = NULL;
	}

#pragma endregion

#pragma region "Token"

	void lexer_token_free(t_token *token) {
		if (!token) return;
		free(token->value);
		free(token);
	}

	t_token *lexer_token_next() {
		return (NULL);
	}

#pragma endregion
