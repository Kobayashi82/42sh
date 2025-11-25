/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grouping.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:30:52 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/25 12:37:28 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/lexer.h"
#include "utils/libft.h"

// GROUPING (recursive parsing)
//
// ()		subshell
// (())		arithmetic
// { ; }	command group (si hay espacio después), close command group (si hay ; o newline antes)

t_token *grouping() {
	t_token *token = NULL;
	char	c = peek(0);

	if (c == '(') {
		if (peek(1) == '(') {
			advance(2);
			stack_push('A');
			// return lexer de (())
		}
		advance(1);
		stack_push('S');
		// return lexer de ()
	}

	if (c == '{') {
		advance(1);
		stack_push('B');
		// return lexer de {}
	}

	return (token);
}
