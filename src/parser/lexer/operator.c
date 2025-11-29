/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:30:57 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/29 17:27:00 by vzurera-         ###   ########.fr       */
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
	char		c = peek(lexer, 0);
	
	string_init(&string);

	if (c == '&' && peek(lexer, 1) == '&') {
		string_append(&string, advance(lexer));
		string_append(&string, advance(lexer));
		return (token_create(lexer, TOKEN_AND, string.value));
	}
	if (c == '&') {
		string_append(&string, advance(lexer));
		return (token_create(lexer, TOKEN_BACKGROUND, string.value));
	}

	if (c == '|' && peek(lexer, 1) == '|') {
		string_append(&string, advance(lexer));
		string_append(&string, advance(lexer));
		return (token_create(lexer, TOKEN_OR, string.value));
	}

	if (c == ';' && peek(lexer, 1) == ';' && peek(lexer, 1) == '&') {
		string_append(&string, advance(lexer));
		string_append(&string, advance(lexer));
		string_append(&string, advance(lexer));
		return (token_create(lexer, TOKEN_CONTINUE, string.value));
	}
	if (c == ';' && peek(lexer, 1) == '&') {
		string_append(&string, advance(lexer));
		string_append(&string, advance(lexer));
		return (token_create(lexer, TOKEN_FALLTHROUGH, string.value));
	}
	if (c == ';' && peek(lexer, 1) == ';') {
		string_append(&string, advance(lexer));
		string_append(&string, advance(lexer));
		return (token_create(lexer, TOKEN_TERMINATOR, string.value));
	}
	if (c == ';') {
		string_append(&string, advance(lexer));
		return (token_create(lexer, TOKEN_SEMICOLON, string.value));
	}

	return (NULL);
}
