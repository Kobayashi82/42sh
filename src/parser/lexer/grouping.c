/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grouping.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:30:52 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/29 17:28:02 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/lexer.h"
#include "utils/libft.h"

// GROUPING (recursive parsing)
//
// ()		subshell
// (())		arithmetic
// { ; }	command group (si hay espacio después), close command group (si hay ; o newline antes)

t_token *grouping(t_lexer *lexer) {
	t_string	string;
	char		c = peek(lexer, 0);
	
	string_init(&string);

	if (c == '(') {
		if (peek(lexer, 1) == '(') {
			string_append(&string, advance(lexer));
			string_append(&string, advance(lexer));
			stack_push(lexer, 'A');
			// return lexer de (())
		}
		string_append(&string, advance(lexer));
		stack_push(lexer, 'S');
		// return lexer de ()
	}

	if (c == '{') {
		string_append(&string, advance(lexer));
		stack_push(lexer, 'B');
		// return lexer de {}
	}

	return (NULL);
}
