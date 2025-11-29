/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:30:57 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/29 14:25:21 by vzurera-         ###   ########.fr       */
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
	size_t	start = lexer.input->position;
	char	c = peek(0);

	if (c == '&' && peek(1) == '&') {
		advance(2);
		return (token_create(TOKEN_AND, start));
	}
	if (c == '&') {
		advance(1);
		return (token_create(TOKEN_BACKGROUND, start));
	}

	if (c == '|' && peek(1) == '|') {
		advance(2);
		return (token_create(TOKEN_OR, start));
	}

	if (c == ';' && peek(1) == ';' && peek(1) == '&') {
		advance(3);
		return (token_create(TOKEN_CONTINUE, start));
	}
	if (c == ';' && peek(1) == '&') {
		advance(2);
		return (token_create(TOKEN_FALLTHROUGH, start));
	}
	if (c == ';' && peek(1) == ';') {
		advance(2);
		return (token_create(TOKEN_TERMINATOR, start));
	}
	if (c == ';') {
		advance(1);
		return (token_create(TOKEN_SEMICOLON, start));
	}


	return (NULL);
}
