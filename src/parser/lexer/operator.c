/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:30:57 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/25 18:30:48 by vzurera-         ###   ########.fr       */
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
	t_token *token = NULL;
	char	c = peek(0);

	if (c == '&' && peek(1) == '&') {
		advance(2);
		return (lexer_token_create(TOKEN_AND, NULL, is_space(-3), is_space(0)));
	}
	if (c == '&') {
		advance(1);
		return (lexer_token_create(TOKEN_BACKGROUND, NULL, is_space(-2), is_space(0)));
	}

	if (c == '|' && peek(1) == '|') {
		advance(2);
		return (lexer_token_create(TOKEN_OR, NULL, is_space(-3), is_space(0)));
	}

	if (c == ';' && peek(1) == ';' && peek(1) == '&') {
		advance(3);
		return (lexer_token_create(TOKEN_CONTINUE, NULL, is_space(-4), is_space(0)));
	}
	if (c == ';' && peek(1) == '&') {
		advance(2);
		return (lexer_token_create(TOKEN_FALLTHROUGH, NULL, is_space(-3), is_space(0)));
	}
	if (c == ';' && peek(1) == ';') {
		advance(2);
		return (lexer_token_create(TOKEN_TERMINATOR, NULL, is_space(-3), is_space(0)));
	}
	if (c == ';') {
		advance(1);
		return (lexer_token_create(TOKEN_SEMICOLON, NULL, is_space(-2), is_space(0)));
	}


	return (token);
}
