/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:30:57 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/30 11:47:59 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/lexer.h"
#include "utils/libft.h"

// OPERATOR
//
// &		background
// &&		and
// ||		or
// |		pipe
// ;		semicolon
// #		comentario
// ;;		case terminator
// ;&		case fallthrough
// ;;&		case continue

t_token *operator(t_lexer *lexer) {
	t_string	string;
	int			line = (lexer->input == lexer->user_buffer) ? lexer->line : -1;
	char		*full_line = (lexer->input) ? lexer->input->value : NULL;
	char		c = peek(lexer, 0);
	
	string_init(&string);

	if (c == '&' && peek(lexer, 1) == '&') {
		string_append(&string, advance(lexer));
		string_append(&string, advance(lexer));
		return (token_create(lexer, TOKEN_AND, string.value, line, full_line));
	}
	if (c == '&') {
		string_append(&string, advance(lexer));
		return (token_create(lexer, TOKEN_BACKGROUND, string.value, line, full_line));
	}

	if (c == '|' && peek(lexer, 1) == '|') {
		string_append(&string, advance(lexer));
		string_append(&string, advance(lexer));
		return (token_create(lexer, TOKEN_OR, string.value, line, full_line));
	}

	if (c == ';' && peek(lexer, 1) == ';' && peek(lexer, 1) == '&') {
		string_append(&string, advance(lexer));
		string_append(&string, advance(lexer));
		string_append(&string, advance(lexer));
		return (token_create(lexer, TOKEN_CONTINUE, string.value, line, full_line));
	}
	if (c == ';' && peek(lexer, 1) == '&') {
		string_append(&string, advance(lexer));
		string_append(&string, advance(lexer));
		return (token_create(lexer, TOKEN_FALLTHROUGH, string.value, line, full_line));
	}
	if (c == ';' && peek(lexer, 1) == ';') {
		string_append(&string, advance(lexer));
		string_append(&string, advance(lexer));
		return (token_create(lexer, TOKEN_TERMINATOR, string.value, line, full_line));
	}
	if (c == ';') {
		string_append(&string, advance(lexer));
		return (token_create(lexer, TOKEN_SEMICOLON, string.value, line, full_line));
	}

	return (NULL);
}
