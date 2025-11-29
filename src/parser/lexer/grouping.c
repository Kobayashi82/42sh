/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grouping.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:30:52 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/29 16:25:55 by vzurera-         ###   ########.fr       */
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
	t_string	string;
	char		c = peek(0);
	
	string_init(&string);

	if (c == '(') {
		if (peek(1) == '(') {
			string_append(&string, advance());
			string_append(&string, advance());
			stack_push('A');
			// return lexer de (())
		}
		string_append(&string, advance());
		stack_push('S');
		// return lexer de ()
	}

	if (c == '{') {
		string_append(&string, advance());
		stack_push('B');
		// return lexer de {}
	}

	return (NULL);
}
