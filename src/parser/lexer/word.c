/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:30:22 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/26 11:12:04 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/lexer.h"
#include "utils/libft.h"

// WORD
//
// ""		double quote (recursive parsing)
// ''		single quote
// $""		translatable string
// $''		ANSI-C quoting
// {...}	brace expansion

t_token *word() {
	size_t	start = lexer.pos;
	char	*stack_copy = malloc(lexer.stack_capacity);
	size_t	stack_size = lexer.stack_size;
	char	c = peek(0);

	strcpy(stack_copy, lexer.stack);
	while (c) {
		if (stack_top() == '\'') {
			if (c == '\'') stack_pop();
			else if (peek(1) == '\0') {
				// more input
				lexer.pos = start;
				lexer.stack_size = stack_size;
				strcpy(lexer.stack, stack_copy);
				free(stack_copy);
				return (lexer_token_create(TOKEN_INPUT, NULL, false, false));
			}
			c = advance(1); continue;
		}
		if (c == '\'') {
			stack_push('\'');
			c = advance(1); continue;
		}

		if (stack_top() == '"') {
			if (c == '"') {
				stack_pop();
				c = advance(1);
			} else if (peek(1) == '\0') {
				// more input
				lexer.pos = start;
				lexer.stack_size = stack_size;
				strcpy(lexer.stack, stack_copy);
				free(stack_copy);
				return (lexer_token_create(TOKEN_INPUT, NULL, false, false));
			} else {
				if (c == '\\') advance(1);
				c = advance(1); continue;
			}
		}
		if (c == '"') {
			stack_push('"');
			c = advance(1); continue;
		}

		if (c == '\\') {
			if (peek(1) == '\0') {
				// more input
				lexer.pos = start;
				lexer.stack_size = stack_size;
				strcpy(lexer.stack, stack_copy);
				free(stack_copy);
				return (lexer_token_create(TOKEN_INPUT, NULL, false, false));
			}
			c = advance(2); continue;
		}

		if (c == ' ' || c == '\t' || c == '\n' || c == '\0' || peek(1) == '\0') { // or operator
			if (stack_top()) {
				// more input
				lexer.pos = start;
				lexer.stack_size = stack_size;
				strcpy(lexer.stack, stack_copy);
				free(stack_copy);
				lexer.stack = ft_strdup(lexer.stack);
				return (lexer_token_create(TOKEN_INPUT, NULL, false, false));
			}
			if (peek(1) == '\0') advance(1);
			free(stack_copy);
			return (lexer_token_create(TOKEN_WORD, ft_substr(lexer.input, start, lexer.pos - start), is_space(lexer.pos - start), is_space(0)));
		}

		c = advance(1);
	}

	if (lexer.stack_size > 0 && c == '\0') return (lexer_token_create(TOKEN_INPUT, NULL, false, false));
	return (lexer_token_create(TOKEN_EOF, NULL, false, false));
}
