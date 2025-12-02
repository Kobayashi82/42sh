/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:30:41 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/02 15:52:56 by vzurera-         ###   ########.fr       */
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

	// t_token *expansion(t_lexer *lexer) {
	// 	t_string	string;
	// 	int			line = (lexer->input == lexer->user_buffer) ? lexer->line : -1;
	// 	char		*full_line = (lexer->input) ? ft_strdup(lexer->input->value) : NULL;
	// 	char		c = peek(lexer, 0);

	// 	string_init(&string);

	// 	if (c == '$') {
	// 		if (peek(lexer, 1) == '(' && peek(lexer, 2) == '(') {
	// 			string_append(&string, advance(lexer));
	// 			string_append(&string, advance(lexer));
	// 			string_append(&string, advance(lexer));
	// 			// stack_push(lexer, 'a');
	// 			return (token_create(lexer, TOKEN_BACKGROUND, string.value, line, full_line));
	// 		}
	// 		if (peek(lexer, 1) == '(') {
	// 			string_append(&string, advance(lexer));
	// 			string_append(&string, advance(lexer));
	// 			// stack_push(lexer, 's');
	// 			return (token_create(lexer, TOKEN_BACKGROUND, string.value, line, full_line));
	// 		}
	// 		if (peek(lexer, 1) == '{') {
	// 			string_append(&string, advance(lexer));
	// 			string_append(&string, advance(lexer));
	// 			// stack_push(lexer, 'p');
	// 			return (token_create(lexer, TOKEN_BACKGROUND, string.value, line, full_line));
	// 		}
	// 	}

	// 	free(full_line);
	// 	free(&string.value);
	// 	return (NULL);
	// }

#pragma endregion

#include <stdio.h>

t_token *expansion(t_lexer *lexer) {
	t_string	string;
	int			line = (lexer->input == lexer->user_buffer) ? lexer->line : -1;
	char		*full_line = (lexer->input) ? ft_strdup(lexer->input->value) : NULL;
	char		c;
	int			type;

	if (peek(lexer, 0) != '$' || peek(lexer, 1) != '(') return (NULL);

	string_init(&string);

	string_append(&string, advance(lexer));
	string_append(&string, advance(lexer));
	
	if (peek(lexer, 0) == '(') {
		string_append(&string, advance(lexer));
		stack_push(lexer, 'a');
		type = TOKEN_WORD;
	} else {
		stack_push(lexer, 's');
		type = TOKEN_WORD;
	}

	while ((c = peek(lexer, 0)) || string.len) {
		int can_advance = !handle_quotes(lexer, &string);
		c = peek(lexer, 0);

		if (c == '\\') {
			if (peek(lexer, 1) == '\n' && peek(lexer, 2) == '\0') {
				advance(lexer);
				advance(lexer);
				if (!*string.value) {
					free(full_line);
					free(string.value);
					lexer->append_inline = 1;
					lexer_append(lexer);
					return (token_get(lexer));
				}
			} else if (peek(lexer, 1) == '\0') {
				lexer->append_inline = 1;
				advance(lexer);
				lexer_append(lexer);
				continue;
			}
			string_append(&string, advance(lexer));
			string_append(&string, advance(lexer));
			continue;
		}

		// $((
		if (c == '$' && peek(lexer, 1) == '(' && peek(lexer, 2) == '(') {
			stack_push(lexer, 'a');
			string_append(&string, advance(lexer));
			string_append(&string, advance(lexer));
			string_append(&string, advance(lexer));
			continue;
		}
		// ((
		if (c == '(' && peek(lexer, 1) == '(') {
			stack_push(lexer, 'A');
			string_append(&string, advance(lexer));
			string_append(&string, advance(lexer));
			continue;
		}

		// (
		if (c == '(') {
			stack_push(lexer, 'S');
			string_append(&string, advance(lexer));
			string_append(&string, advance(lexer));
			continue;
		}
		// $(
		if (c == '$' && peek(lexer, 1) == '(') {
			stack_push(lexer, 's');
			string_append(&string, advance(lexer));
			string_append(&string, advance(lexer));
			continue;
		}

		// )) 
		if (c == ')' && peek(lexer, 1) == ')') {
			if (stack_top(lexer) == 'a' || stack_top(lexer) == 'A') {
				stack_pop(lexer);
				string_append(&string, advance(lexer));
				string_append(&string, advance(lexer));
				if (!lexer->stack_size) break;
				continue;
			} else if (stack_top(lexer) != 'a' && stack_top(lexer) != 'A') break;
		}

		// )
		if (c == ')') {
			if (stack_top(lexer) == 's' || stack_top(lexer) == 'S') {
				stack_pop(lexer);
				string_append(&string, advance(lexer));
				if (!lexer->stack_size) break;
				continue;
			} else if (stack_top(lexer) != 's' && stack_top(lexer) != 'S') break;
		}
		
		if (peek(lexer, 1) == '\0') {
			string_append(&string, advance(lexer));
			string_append(&string, '\n');
			lexer_append(lexer);
			continue;
		}

		if (can_advance) string_append(&string, advance(lexer));
	}

	if (!stack_top(lexer)) return (token_create(lexer, type, string.value, line, full_line));

	return (token_create(lexer, TOKEN_ERROR, string.value, line, full_line));
}
