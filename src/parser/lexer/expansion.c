/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:30:41 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/30 21:04:10 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "parser/lexer.h"
	#include "utils/libft.h"

#pragma endregion

// EXPANSION (recursive parsing)
//
// ``		command substitution
// $()		command substitution
// $(())	arithmetic substitution
// ${}		parameter expression

#pragma region "Expansion"

	t_token *expansion(t_lexer *lexer) {
		t_string	string;
		int			line = (lexer->input == lexer->user_buffer) ? lexer->line : -1;
		char		*full_line = (lexer->input) ? ft_strdup(lexer->input->value) : NULL;
		char		c = peek(lexer, 0);

		string_init(&string);

		if (c == '$') {
			if (peek(lexer, 1) == '(' && peek(lexer, 2) == '(') {
				string_append(&string, advance(lexer));
				string_append(&string, advance(lexer));
				string_append(&string, advance(lexer));
				// stack_push(lexer, 'a');
				return (token_create(lexer, TOKEN_BACKGROUND, string.value, line, full_line));
			}
			if (peek(lexer, 1) == '(') {
				string_append(&string, advance(lexer));
				string_append(&string, advance(lexer));
				// stack_push(lexer, 's');
				return (token_create(lexer, TOKEN_BACKGROUND, string.value, line, full_line));
			}
			if (peek(lexer, 1) == '{') {
				string_append(&string, advance(lexer));
				string_append(&string, advance(lexer));
				// stack_push(lexer, 'p');
				return (token_create(lexer, TOKEN_BACKGROUND, string.value, line, full_line));
			}
		}

		free(full_line);
		free(string.value);
		return (NULL);
	}

#pragma endregion
