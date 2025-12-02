/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:30:22 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/02 20:18:35 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "hashes/alias.h"
	#include "parser/lexer.h"
	#include "utils/libft.h"

#pragma endregion

#pragma region "Word"

	t_token *word(t_lexer *lexer) {
		t_string	string;
		int			line = (lexer->input == lexer->user_buffer) ? lexer->line : -1;
		char		*full_line = (lexer->input) ? ft_strdup(lexer->input->value) : NULL;
		char		c;

		string_init(&string);

		while ((c = peek(lexer, 0)) || string.len) {

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

			if (is_operator(c)) {

				if (should_expand_alias(lexer, string.value)) {
					char *alias_content = alias_find_value(string.value);
					if (alias_content) {
						int expand_next = 0;
						if (*alias_content) expand_next = isspace(alias_content[ft_strlen(alias_content) - 1]);
						buffer_push(lexer, alias_content, string.value);
						free(string.value);
						t_token *token = token_get(lexer);
						lexer->can_expand_alias = expand_next;
						free(full_line);
						return (token);
					}
				}

				lexer->can_expand_alias = 0;
				lexer->command_position = 0;
				return (token_create(lexer, TOKEN_WORD, string.value, line, full_line));
			}

			string_append(&string, advance(lexer));
		}

		free(full_line);
		free(string.value);
		return (token_create(lexer, TOKEN_EOF, NULL, line, NULL));
	}

#pragma endregion
