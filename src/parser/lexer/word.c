/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:30:22 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/29 20:13:02 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "hashes/alias.h"
	#include "parser/lexer.h"
	#include "utils/libft.h"

#pragma endregion

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

		    if (should_expand_alias(lexer, string.value)) {
				char *alias_content = alias_find_value(string.value);
				if (alias_content) {
					buffer_push(lexer, alias_content, string.value);
					free(string.value);
					return token_next(lexer);
				}
			}

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
