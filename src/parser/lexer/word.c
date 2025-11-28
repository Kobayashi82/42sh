/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:30:22 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/28 21:17:05 by vzurera-         ###   ########.fr       */
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
	size_t	start = lexer.pos;
	char	c = peek(0);

	while (c) {
		int can_advance = !handle_quotes();

		if (c == ' ' || c == '\t' || c == '\n' || c == '\0' || peek(1) == '\0') { // or operator
			if (peek(1) == '\0') advance(1);
			return (token_create(TOKEN_WORD, start));
		}

		if (can_advance) c = advance(1);
	}

	if (stack_top()) {
		// error
		return (NULL);
	}

	return (token_create(TOKEN_EOF, 0));
}
