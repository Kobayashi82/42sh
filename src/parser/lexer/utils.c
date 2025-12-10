/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 20:35:54 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/10 23:21:29 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "parser/lexer.h"

#pragma endregion
#include <stdio.h>
#pragma region "Is operator"

	int is_operator(char c) {
		const char *operators[] = { " ", "\t", "\n", ">", "<", "&", "|", ";", "(", ")", "{", "$", "\0", NULL};

		for (int i = 0; operators[i]; i++) {
			if (c == operators[i][0]) return (1);
		}

		return (0);
	}

#pragma endregion

#pragma region "Handle Quotes"

	char handle_quotes(t_lexer *lexer, t_segment *segment) {
		char c = peek(lexer, 0);

		segment_set_quoted(segment, c);

		if (c == '\'') {
			while ((c = peek(lexer, 0)) || stack_top(lexer)) {
				// Single Quoted
				if (stack_top(lexer) == '\'') {
					if (c == '\'') {
						stack_pop(lexer);
						advance(lexer);
						segment->type = 1; // Literal
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
						advance(lexer);
						segment->type = 2; // Quotes
						return (0);
					} else if (c == '\\' && peek(lexer, 1) == '\0') {
						advance(lexer);
						lexer->append_inline = 1;
						lexer_append(lexer);
						if (!lexer->input) return (0);
						continue;
					} else if (c == '\\') {
						segment_append(segment, advance(lexer));
						segment_append(segment, advance(lexer));
						continue;
					} else if (c == '\0') {
						segment_append(segment, '\n');
						lexer_append(lexer);
						if (!lexer->input) return (0);
						continue;
					} else {
						t_token *token = NULL;
						stack_pop(lexer);
						if (!token) token = variables(lexer, 1);
						if (!token) token = expansion(lexer, 1);
						stack_push(lexer, '"');
						if (token) {
							segment_append_token(segment, token, '"');
							segment_new(segment);
							segment_set_quoted(segment, '"');
						} else {
							segment_append(segment, advance(lexer));
						}

						continue;
					}
				} else if (c == '"') {
					stack_push(lexer,'"');
					advance(lexer);
					continue;
				}

				return (0);
			}
		}

		return (0);
	}

#pragma endregion
