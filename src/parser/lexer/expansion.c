/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:30:41 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/02 20:29:34 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "parser/lexer.h"
	#include "utils/libft.h"

#pragma endregion

#pragma region "Variables"

	t_token *variables(t_lexer *lexer) {
		t_string	string;
		int			line = (lexer->input == lexer->user_buffer) ? lexer->line : -1;
		char		*full_line = (lexer->input) ? ft_strdup(lexer->input->value) : NULL;

		string_init(&string);

 		if (peek(lexer, 0) == '$' && (peek(lexer, 1) == '\'' || peek(lexer, 1) == '"')) {
			string_append(&string, advance(lexer));
			handle_quotes(lexer, &string);
			return (token_create(lexer, TOKEN_WORD, string.value, line, full_line));
		}

		if (peek(lexer, 0) == '$' && (peek(lexer, 1) == '$' || !is_operator(peek(lexer, 1)))) {
			string_append(&string, advance(lexer));

			if (peek(lexer, 0) == '$') {
				string_append(&string, advance(lexer));
				return (token_create(lexer, TOKEN_WORD, string.value, line, full_line));
			}
			if (peek(lexer, 0) == '#') {
				string_append(&string, advance(lexer));
				return (token_create(lexer, TOKEN_WORD, string.value, line, full_line));
			}
			if (peek(lexer, 0) == '@') {
				string_append(&string, advance(lexer));
				return (token_create(lexer, TOKEN_WORD, string.value, line, full_line));
			}
			if (peek(lexer, 0) == '*') {
				string_append(&string, advance(lexer));
				return (token_create(lexer, TOKEN_WORD, string.value, line, full_line));
			}
			if (peek(lexer, 0) == '!') {
				string_append(&string, advance(lexer));
				return (token_create(lexer, TOKEN_WORD, string.value, line, full_line));
			}
			if (peek(lexer, 0) == '?') {
				string_append(&string, advance(lexer));
				return (token_create(lexer, TOKEN_WORD, string.value, line, full_line));
			}
			if (peek(lexer, 0) == '-') {
				string_append(&string, advance(lexer));
				return (token_create(lexer, TOKEN_WORD, string.value, line, full_line));
			}
			if (isdigit(peek(lexer, 0))) {
				while (isdigit(peek(lexer, 0))) string_append(&string, advance(lexer));
				return (token_create(lexer, TOKEN_WORD, string.value, line, full_line));
			}

			while (!is_operator(peek(lexer, 0)) && peek(lexer, 0) != '\'' && peek(lexer, 0) != '"') {
				if (peek(lexer, 0) == '\\' && peek(lexer, 1) == '\n' && peek(lexer, 2) == '\0') {
					advance(lexer);
					advance(lexer);
					if (string.len) {
						free(full_line);
						free(string.value);
						lexer->append_inline = 1;
						lexer_append(lexer);
						return (token_get(lexer));
					}
				} else if (peek(lexer, 0) == '\\' && peek(lexer, 1) == '\0') {
					advance(lexer);
					lexer->append_inline = 1;
					lexer_append(lexer);
					continue;
				} else if (peek(lexer, 0) == '\\') {
					string_append(&string, advance(lexer));
					string_append(&string, advance(lexer));
					continue;
				}

				string_append(&string, advance(lexer));
			}

			return (token_create(lexer, TOKEN_WORD, string.value, line, full_line));
		}

		free(full_line);
		free(string.value);
		return (NULL);
	}

#pragma endregion

#pragma region "Expansion"

	t_token *expansion(t_lexer *lexer) {
		t_string	string;
		int			line = (lexer->input == lexer->user_buffer) ? lexer->line : -1;
		char		*full_line = (lexer->input) ? ft_strdup(lexer->input->value) : NULL;
		char		c;
		int			type;
		int			group_can_end = 0;

		if (peek(lexer,0) != '`' && !(peek(lexer,0) == '$' && (peek(lexer,1) == '(' || peek(lexer,1) == '{'))) {
			free(full_line);
			return (NULL);
		}

		string_init(&string);

		if (peek(lexer, 0) == '`') {
			string_append(&string, advance(lexer));
			stack_push(lexer, '`');
			type = TOKEN_WORD;
		} else {
			string_append(&string, advance(lexer));

			if (peek(lexer, 0) == '(' && peek(lexer, 1) == '(') {
				string_append(&string, advance(lexer));
				string_append(&string, advance(lexer));
				stack_push(lexer, 'a');
				type = TOKEN_WORD;
			} else if (peek(lexer, 0) == '(') {
				string_append(&string, advance(lexer));
				stack_push(lexer, 's');
				type = TOKEN_WORD;
			} else if (peek(lexer, 0) == '{') {
				string_append(&string, advance(lexer));
				stack_push(lexer, 'p');
				type = TOKEN_WORD;
			}
		}

		while ((c = peek(lexer, 0)) || stack_top(lexer)) {

			if (c == '\'' || c == '"') {
				handle_quotes(lexer, &string);
				continue;
			}

			if (c == '\\' && peek(lexer, 1) == '\n' && peek(lexer, 2) == '\0') {
				advance(lexer);
				advance(lexer);
				if (string.len) {
					free(full_line);
					free(string.value);
					lexer->append_inline = 1;
					lexer_append(lexer);
					return (token_get(lexer));
				}
			} else if (c == '\\' && peek(lexer, 1) == '\0') {
				advance(lexer);
				lexer->append_inline = 1;
				lexer_append(lexer);
				continue;
			} else if (c == '\\') {
				string_append(&string, advance(lexer));
				string_append(&string, advance(lexer));
				continue;
			}

			if (c == '\0') {
				if (stack_top(lexer) == 'G') group_can_end = 1;
				string_append(&string, '\n');
				lexer_append(lexer);
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
				continue;
			}
			// $(
			if (c == '$' && peek(lexer, 1) == '(') {
				stack_push(lexer, 's');
				string_append(&string, advance(lexer));
				string_append(&string, advance(lexer));
				continue;
			}

			// ${
			if (c == '$' && peek(lexer, 1) == '{') {
				stack_push(lexer, 'p');
				string_append(&string, advance(lexer));
				string_append(&string, advance(lexer));
				continue;
			}
			// {
			if (c == '{' && (isspace(peek(lexer, 1)) || peek(lexer, 1) == '\0')) {
				stack_push(lexer, 'G');
				string_append(&string, advance(lexer));
				if (peek(lexer, 0)) string_append(&string, advance(lexer));
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

			// }
			if (c == '}') {
				if (stack_top(lexer) == 'p') {
					stack_pop(lexer);
					string_append(&string, advance(lexer));
					if (!lexer->stack_size) break;
					continue;
				} else if (stack_top(lexer) == 'G' && group_can_end) {
					stack_pop(lexer);
					string_append(&string, advance(lexer));
					if (!lexer->stack_size) break;
					continue;
				}
			}
			if (stack_top(lexer) == 'G' && (c == ';' || c == '\n')) {
				group_can_end = 1;
				string_append(&string, advance(lexer));
				continue;
			} else if (stack_top(lexer) == 'G' && isspace(c) && group_can_end) {
				string_append(&string, advance(lexer));
				continue;
			}

			// `
			if (c == '`') {
				if (stack_top(lexer) == '`') {
					stack_pop(lexer);
					string_append(&string, advance(lexer));
					if (!lexer->stack_size) break;
					continue;
				} else {
					stack_push(lexer, '`');
					string_append(&string, advance(lexer));
					continue;
				}
			}

			string_append(&string, advance(lexer));
		}

		if (!stack_top(lexer)) return (token_create(lexer, type, string.value, line, full_line));

		return (token_create(lexer, TOKEN_ERROR, string.value, line, full_line));
	}

#pragma endregion

// EXPANSION (recursive parsing)
//
// ``		command substitution
// $()		command substitution
// $(())	arithmetic substitution
// ${}		parameter expression
// $""		translatable string
// $''		ANSI-C quoting
// $VAR		variable
