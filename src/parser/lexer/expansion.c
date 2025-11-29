/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:30:41 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/29 17:28:19 by vzurera-         ###   ########.fr       */
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

t_token *expansion(t_lexer *lexer) {
	t_string	string;
	char		c = peek(lexer, 0);

	string_init(&string);

	if (c == '$') {
		if (peek(lexer, 1) == '(' && peek(lexer, 2) == '(') {
			string_append(&string, advance(lexer));
			string_append(&string, advance(lexer));
			string_append(&string, advance(lexer));
			stack_push(lexer, 'a');
			// return lexer de $(())
		}
		if (peek(lexer, 1) == '(') {
			string_append(&string, advance(lexer));
			string_append(&string, advance(lexer));
			stack_push(lexer, 's');
			// return lexer de $()
		}
		if (peek(lexer, 1) == '{') {
			string_append(&string, advance(lexer));
			string_append(&string, advance(lexer));
			stack_push(lexer, 'p');
			// return lexer de ${}
		}
	}

	if (c == '`') {
		string_append(&string, advance(lexer));
		stack_push(lexer, '`');
		// return lexer de ``
	}

	return (NULL);
}
