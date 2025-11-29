/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:30:57 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/29 16:25:22 by vzurera-         ###   ########.fr       */
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

t_token *operator() {
	t_string	string;
	char		c = peek(0);
	
	string_init(&string);

	if (c == '&' && peek(1) == '&') {
		string_append(&string, advance());
		string_append(&string, advance());
		return (token_create(TOKEN_AND, string.value));
	}
	if (c == '&') {
		string_append(&string, advance());
		return (token_create(TOKEN_BACKGROUND, string.value));
	}

	if (c == '|' && peek(1) == '|') {
		string_append(&string, advance());
		string_append(&string, advance());
		return (token_create(TOKEN_OR, string.value));
	}

	if (c == ';' && peek(1) == ';' && peek(1) == '&') {
		string_append(&string, advance());
		string_append(&string, advance());
		string_append(&string, advance());
		return (token_create(TOKEN_CONTINUE, string.value));
	}
	if (c == ';' && peek(1) == '&') {
		string_append(&string, advance());
		string_append(&string, advance());
		return (token_create(TOKEN_FALLTHROUGH, string.value));
	}
	if (c == ';' && peek(1) == ';') {
		string_append(&string, advance());
		string_append(&string, advance());
		return (token_create(TOKEN_TERMINATOR, string.value));
	}
	if (c == ';') {
		string_append(&string, advance());
		return (token_create(TOKEN_SEMICOLON, string.value));
	}

	return (NULL);
}
