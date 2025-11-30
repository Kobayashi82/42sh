/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:30:22 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/30 20:57:15 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "hashes/alias.h"
	#include "parser/lexer.h"
	#include "utils/libft.h"

#pragma endregion

// Tambien tratar como words $(), $(()), 

#pragma region "Is operator"

	int is_operator(t_lexer *lexer) {
		const char *operators[] = { " ", "\t", "\n", ">", "<", "&", "|", ";", "(", ")", "\0", NULL};

		for (int i = 0; operators[i]; i++) {
			if (peek(lexer, 0) == operators[i][0]) return (1);
		}

		return (peek(lexer, 0) == '{' && (peek(lexer, 1) == '\0' || isspace(peek(lexer, 1))));
	}

#pragma endregion

#pragma region "Word"

	t_token *word(t_lexer *lexer) {
		t_string	string;
		int			line = (lexer->input == lexer->user_buffer) ? lexer->line : -1;
		char		*full_line = (lexer->input) ? ft_strdup(lexer->input->value) : NULL;
		char		c;

		string_init(&string);

		while ((c = peek(lexer, 0))) {
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
						return (token_next(lexer));
					}
				} else if (peek(lexer, 1) == '\0') {
					lexer->append_inline = 1;
					advance(lexer);
					lexer_append(lexer);
					continue;
				}
				string_append(&string, advance(lexer));
				string_append(&string, advance(lexer));
			}

			if (is_operator(lexer) || peek(lexer, 1) == '\0') {
				if (!is_operator(lexer) && peek(lexer, 1) == '\0') string_append(&string, advance(lexer));

				if (should_expand_alias(lexer, string.value)) {
					char *alias_content = alias_find_value(string.value);
					if (alias_content) {
						int expand_next = 0;
						if (*alias_content) expand_next = isspace(alias_content[ft_strlen(alias_content) - 1]);
						buffer_push(lexer, alias_content, string.value);
						free(string.value);
						t_token *token = token_next(lexer);
						lexer->can_expand_alias = expand_next;
						free(full_line);
						return (token);
					}
				}

				lexer->can_expand_alias = 0;
				lexer->command_position = 0;
				return (token_create(lexer, TOKEN_WORD, string.value, line, full_line));
			}

			if (can_advance) string_append(&string, advance(lexer));
		}

		free(full_line);
		free(string.value);
		return (token_create(lexer, TOKEN_EOF, NULL, line, NULL));
	}

#pragma endregion
