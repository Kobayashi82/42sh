/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grouping.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:30:52 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/30 21:05:20 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "parser/lexer.h"
	#include "utils/libft.h"

#pragma endregion

// GROUPING (recursive parsing)
//
// ()		subshell
// (())		arithmetic
// { ; }	command group (si hay espacio después), close command group (si hay ; o newline antes)

#pragma region "Grouping"

	t_token *grouping(t_lexer *lexer) {
		t_string	string;
		int			line = (lexer->input == lexer->user_buffer) ? lexer->line : -1;
		char		*full_line = (lexer->input) ? ft_strdup(lexer->input->value) : NULL;
		char		c = peek(lexer, 0);
		
		string_init(&string);

		if (c == '(') {
			if (peek(lexer, 1) == '(') {
				string_append(&string, advance(lexer));
				string_append(&string, advance(lexer));
				// stack_push(lexer, 'A');
				return (token_create(lexer, TOKEN_BACKGROUND, string.value, line, full_line));
			}
			string_append(&string, advance(lexer));
			// stack_push(lexer, 'S');
			return (token_create(lexer, TOKEN_BACKGROUND, string.value, line, full_line));
		}

		if (c == '{' && (peek(lexer, 1) == '\0' || isspace(peek(lexer, 1)))) {
			string_append(&string, advance(lexer));
			// stack_push(lexer, 'B');
			return (token_create(lexer, TOKEN_BACKGROUND, string.value, line, full_line));
		}

		free(full_line);
		free(string.value);
		return (NULL);
	}

#pragma endregion
