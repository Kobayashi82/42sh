/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 20:35:54 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/19 14:38:48 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "parser/lexer.h"
	#include "utils/libft.h"

#pragma endregion

#pragma region "Is operator"

	int is_operator(char c) {
		const char *operators[] = { ">", "<", "&", "|", ";", "(", ")", "{", "\0", NULL};

		for (int i = 0; operators[i]; i++) {
			if (c == operators[i][0]) return (1);
		}

		return (isspace(c));
	}

#pragma endregion

#pragma region "Handle Quotes"

	char handle_quotes(t_lexer *lexer, t_segment *segment, int add_quote) {
		char c = peek(lexer, 0);

		segment_set_quoted(segment, c);

		if (c == '\'') {
			while ((c = peek(lexer, 0)) || stack_top(lexer)) {
				// Single Quoted
				if (stack_top(lexer) == '\'') {
					if (c == '\'') {
						stack_pop(lexer);
						advance(lexer);
						return (0);
					} else if (c == '\0') {
						lexer_append(lexer);
						if (!lexer->input) return (0);
						segment_append(segment, '\n');
						continue;
					} else {
						segment_append(segment, advance(lexer));
						continue;
					}
				} else if (c == '\'') {
					stack_push(lexer,'\'');
					advance(lexer);
					continue;
				}
				return (0);
			}
		}

		if (c == '"') {
			while ((c = peek(lexer, 0)) || stack_top(lexer)) {

				// Double Quoted
				if (stack_top(lexer) == '"') {
					if (c == '"') {
						stack_pop(lexer);
						if (add_quote)	segment_append(segment, advance(lexer));
						else			advance(lexer);
						return (0);
					} else if (c == '\\' && peek(lexer, 1) == '\0') {
						advance(lexer);
						lexer->append_inline = 1;
						lexer_append(lexer);
						if (!lexer->input) return (0);
						continue;
					} else if (c == '\\') {
						if (*segment_last_value(segment)) segment_new(segment);
						if (!strchr("$\"\'\\\n", peek(lexer, 1)))	segment_append(segment, advance(lexer));
						else										advance(lexer);
						segment_append(segment, advance(lexer));
						segment_set_quoted(segment, '\'');
						if (peek(lexer, 0) && peek(lexer, 0) != '"') {
							segment_new(segment);
							segment_set_quoted(segment, '"');
						}
						continue;
					} else if (c == '\0') {
						segment_append(segment, '\n');
						lexer_append(lexer);
						if (!lexer->input) return (0);
						continue;
					} else {
						t_token *token = NULL;
						stack_pop(lexer);
						if (!token) token = variable(lexer);
						if (!token) token = expansion(lexer);
						stack_push(lexer, '"');
						if (token) {
							if (*segment_last_value(segment)) segment_new(segment);
							char *value = segment_flatten(token->segment);

							t_segment *curr = segment_last(segment);
							free(curr->string.value);
							curr->type = token->type;
							curr->string.value = value;
							curr->string.len = ft_strlen(curr->string.value);
							curr->string.capacity = (value) ? curr->string.len + 1 : 0;
							segment_set_quoted(curr, '"');
							token_free(token);

							if (peek(lexer, 0) != '"') {
								segment_new(segment);
								segment_set_quoted(segment, '"');
							}
						} else {
							segment_append(segment, advance(lexer));
						}

						continue;
					}
				} else if (c == '"') {
					stack_push(lexer,'"');
					if (add_quote)	segment_append(segment, advance(lexer));
					else			advance(lexer);
					continue;
				}

				return (0);
			}
		}

		return (0);
	}

#pragma endregion
// echo "hola $(como estas $(
// "
// popo
// "
// cierro)
// adios
// $USER"
// vale"
// cierro 2)
// $USER
// y falta comillas"