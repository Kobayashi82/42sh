/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 20:35:54 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/10 14:56:46 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "parser/lexer.h"

#pragma endregion

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
		char c;

		segment_new(segment);
		while ((c = peek(lexer, 0)) || stack_top(lexer)) {
			// Single Quoted
			if (stack_top(lexer) == '\'') {
				if (c == '\'') {
					stack_pop(lexer);
					segment_append(segment, advance(lexer));
					segment->quoted = '\'';
					segment->type = 1; // Literal
					break;
				} else if (c == '\0') {
					lexer_append(lexer);
					if (!lexer->input) break;
					segment_append(segment, '\n');
					continue;
				} else {
					segment_append(segment, advance(lexer));
					continue;
				}
			} else if (c == '\'') {
				stack_push(lexer,'\'');
				segment_append(segment, advance(lexer));
				continue;
			}

			// Double Quoted
			if (stack_top(lexer) == '"') {
				if (c == '"') {
					stack_pop(lexer);
					segment_append(segment, advance(lexer));
					break;
				} else if (c == '\\' && peek(lexer, 1) == '\0') {
					advance(lexer);
					lexer->append_inline = 1;
					lexer_append(lexer);
					continue;
				} else if (c == '\\') {
					segment_append(segment, advance(lexer));
					segment_append(segment, advance(lexer));
					continue;
				} else if (c == '\0') {
					segment_append(segment, '\n');
					lexer_append(lexer);
					continue;
				} else {
					segment_append(segment, advance(lexer));
					continue;
				}
			} else if (c == '"') {
				stack_push(lexer,'"');
				segment_append(segment, advance(lexer));
				continue;
			}

			break;
		}

		return (0);
	}

#pragma endregion
