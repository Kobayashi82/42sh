/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:30:22 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/29 16:21:43 by vzurera-         ###   ########.fr       */
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

t_token *word() {
	t_string	string;
	char		c;
	
	string_init(&string);

	while ((c = peek(0))) {
		int can_advance = !handle_quotes(&string);

		if (c == ' ' || c == '\t' || c == '\n' || c == '\0' || peek(1) == '\0') { // or operator
			if (peek(1) == '\0') string_append(&string, advance());
			return (token_create(TOKEN_WORD, string.value));
		}

		if (can_advance) string_append(&string, advance());
	}

	if (stack_top()) {
		// error
		return (NULL);
	}

	return (token_create(TOKEN_EOF, NULL));
}
