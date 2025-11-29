/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:30:41 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/29 16:26:34 by vzurera-         ###   ########.fr       */
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
	t_string	string;
	char		c = peek(0);

	string_init(&string);

	if (c == '$') {
		if (peek(1) == '(' && peek(2) == '(') {
			string_append(&string, advance());
			string_append(&string, advance());
			string_append(&string, advance());
			stack_push('a');
			// return lexer de $(())
		}
		if (peek(1) == '(') {
			string_append(&string, advance());
			string_append(&string, advance());
			stack_push('s');
			// return lexer de $()
		}
		if (peek(1) == '{') {
			string_append(&string, advance());
			string_append(&string, advance());
			stack_push('p');
			// return lexer de ${}
		}
	}

	if (c == '`') {
		string_append(&string, advance());
		stack_push('`');
		// return lexer de ``
	}

	return (NULL);
}
