/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:30:22 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/29 18:07:11 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/lexer.h"
#include "utils/libft.h"

// WORD
//
// ""		double quote (recursive parsing)
// ''		single quote
// $""		translatable string
// $''		ANSI-C quoting
// {...}	brace expansion

t_token *word(t_lexer *lexer) {
	t_string	string;
	char		c;
	
	string_init(&string);

	while ((c = peek(lexer, 0))) {
		int can_advance = !handle_quotes(lexer, &string);

		if (c == ' ' || c == '\t' || c == '\n' || c == '\0' || peek(lexer, 1) == '\0') { // or operator
			if (peek(lexer, 1) == '\0') string_append(&string, advance(lexer));
			return (token_create(lexer, TOKEN_WORD, string.value));
		}

		if (can_advance) string_append(&string, advance(lexer));
	}

	if (lexer->stack_size) {
		// error
		return (NULL);
	}

	return (token_create(lexer, TOKEN_EOF, NULL));
}
