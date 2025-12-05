/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 20:35:54 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/05 20:29:35 by vzurera-         ###   ########.fr       */
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

	char handle_quotes(t_lexer *lexer, t_string *string) {
		char c;

		while ((c = peek(lexer, 0)) || stack_top(lexer)) {
			// Single Quoted
			if (stack_top(lexer) == '\'') {
				if (c == '\'') {
					stack_pop(lexer);
					string_append(string, advance(lexer));
					break;
				} else if (c == '\0') {
					lexer_append(lexer);
					if (!lexer->input) break;
					string_append(string, '\n');
					continue;
				} else {
					string_append(string, advance(lexer));
					continue;
				}
			} else if (c == '\'') {
				stack_push(lexer,'\'');
				string_append(string, advance(lexer));
				continue;
			}

			// Double Quoted
			if (stack_top(lexer) == '"') {
				if (c == '"') {
					stack_pop(lexer);
					string_append(string, advance(lexer));
					break;
				} else if (c == '\\' && peek(lexer, 1) == '\0') {
					advance(lexer);
					lexer->append_inline = 1;
					lexer_append(lexer);
					continue;
				} else if (c == '\\') {
					string_append(string, advance(lexer));
					string_append(string, advance(lexer));
					continue;
				} else if (c == '\0') {
					string_append(string, '\n');
					lexer_append(lexer);
					continue;
				} else {
					string_append(string, advance(lexer));
					continue;
				}
			} else if (c == '"') {
				stack_push(lexer,'"');
				string_append(string, advance(lexer));
				continue;
			}		

			break;
		}

		return (0);
	}

#pragma endregion
