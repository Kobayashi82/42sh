/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:30:22 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/19 12:57:53 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "hashes/alias.h"
	#include "parser/lexer.h"
	#include "main/options.h"
	#include "utils/libft.h"

#pragma endregion

#pragma region "Expand Alias"

	static int expand_alias(t_lexer *lexer, char *alias_name) {
		if (!alias_name || !lexer->can_expand_alias || !options.expand_aliases) return (0);

		t_buff *buffer = lexer->input;
		while (buffer) {
			if (buffer->alias_name && !strcmp(buffer->alias_name, alias_name)) return (0);
			buffer = buffer->next;
		}

		if (!alias_find_value(alias_name)) return (0);
		
		return (1);
	}

#pragma endregion

#pragma region "Return Word"

	t_token *return_word(t_lexer *lexer, t_segment *segment, char *full_line, int line) {
		if (!segment) return (NULL);

		if (!segment->string.len && !segment->quoted) {
			free(full_line);
			segment_free(segment);
			if (is_operator(peek(lexer, 0))) return (token_create(lexer, TOKEN_EOF, NULL, lexer->line + 1, NULL));;
			return (token_get(lexer));
		}

		char *value = segment_last_value(segment);

		if (expand_alias(lexer, value)) {
			char *alias_content = alias_find_value(value);
			if (alias_content) {
				int expand_next = 0;
				if (*alias_content) expand_next = isspace(alias_content[ft_strlen(alias_content) - 1]);
				buffer_push(lexer, alias_content, value);
				segment_free(segment);
				free(full_line);
				t_token *token = token_get(lexer);
				lexer->can_expand_alias = expand_next;
				return (token);
			}
		}

		lexer->can_expand_alias = 0;
		lexer->command_position = 0;
		return (token_create(lexer, TOKEN_WORD, segment, line, full_line));
	}

#pragma endregion

#pragma region "Word"

	t_token *word(t_lexer *lexer) {
		int		line = (lexer->input == lexer->user_buffer) ? lexer->line : -1;
		char	*full_line = (lexer->input) ? ft_strdup(lexer->input->value) : NULL;
		char	c;

		t_segment *segment = segment_new(NULL);

		while ((c = peek(lexer, 0))) {

			if (c == '\'' || c == '"') {
				if (*segment_last_value(segment)) segment_new(segment);
				handle_quotes(lexer, segment, 0);
				if (!is_operator(peek(lexer, 0)) || (peek(lexer, 0) == '{' && !isspace(peek(lexer, 1)) && peek(lexer, 1) != '\0')) segment_new(segment);
				continue;
			}

			if (c == '\\' && peek(lexer, 1) == '\n' && peek(lexer, 2) == '\0') {
				advance(lexer);
				advance(lexer);
				lexer->append_inline = 1;
				lexer_append(lexer);
				continue;
			} else if (c == '\\' && peek(lexer, 1) == '\0') {
				advance(lexer);
				lexer->append_inline = 1;
				lexer_append(lexer);
				continue;
			} else if (c == '\\') {
				if (*segment_last_value(segment)) segment_new(segment);
				advance(lexer);
				segment_append(segment, advance(lexer));
				segment_set_quoted(segment, '\'');
				if (!is_operator(peek(lexer, 0)) || (peek(lexer, 0) == '{' && !isspace(peek(lexer, 1)) && peek(lexer, 1) != '\0')) segment_new(segment);
				continue;
			}

			if (is_operator(c) && !(c == '{' && !isspace(peek(lexer, 1)) && peek(lexer, 1) != '\0')) {
				return (return_word(lexer, segment, full_line, line));
			}

			segment_append(segment, advance(lexer));
		}

		if (!segment_empty(segment)) {
			return (return_word(lexer, segment, full_line, line));
		}

		segment_free(segment);
		return (token_create(lexer, TOKEN_EOF, NULL, line, full_line));
	}

#pragma endregion

