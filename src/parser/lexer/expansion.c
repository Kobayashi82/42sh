/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:30:41 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/25 12:38:12 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/lexer.h"
#include "utils/libft.h"

// EXPANSION (recursive parsing)
//
// ``		command substitution
// $()		command substitution
// $(())	arithmetic substitution
// ${}		parameter expression

t_token *expansion() {
	t_token *token = NULL;
	char	c = peek(0);

	if (c == '$') {
		if (peek(1) == '(' && peek(2) == '(') {
			advance(3);
			stack_push('a');
			// return lexer de $(())
		}
		if (peek(1) == '(') {
			advance(2);
			stack_push('s');
			// return lexer de $()
		}
		if (peek(1) == '{') {
			advance(2);
			stack_push('p');
			// return lexer de ${}
		}
	}

	if (c == '`') {
		advance(1);
		stack_push('`');
		// return lexer de ``
	}

	return (token);
}
