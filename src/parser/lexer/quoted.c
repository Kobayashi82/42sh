/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quoted.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 20:35:54 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/29 16:47:05 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/libft.h"
#include "parser/lexer.h"

char handle_quotes(t_string *string) {
	t_buff	*start_buffer = lexer.input;
	size_t	start_position = (lexer.input) ? lexer.input->position : 0;
	char	c;

	while ((c = peek(0))) {
		if (stack_top() == '\'') {
			if (c == '\'') {
				stack_pop();
				string_append(string, advance());
			} else if (peek(1) == '\0') {
				string_append(string, advance());
				string_append(string, '\n');
				lexer_append();
				continue;
			} else {
				string_append(string, advance());
				continue;
			}
		}
		if (c == '\'') {
			stack_push('\'');
			string_append(string, advance());
			continue;
		}

		if (stack_top() == '"') {
			if (c == '"') {
				stack_pop();
				string_append(string, advance());
			} else if (peek(1) == '\0') {
				if (c == '\\') {
					lexer.append_inline = 1;
					advance();
					lexer_append();
					continue;
				} else {
					string_append(string, advance());
					string_append(string, '\n');
					lexer_append();
					continue;
				}
			} else {
				if (c == '\\') string_append(string, advance());
				string_append(string, advance());
				continue;
			}
		}
		if (c == '"') {
			stack_push('"');
			string_append(string, advance());
			continue;
		}

		if (c == '\\') {
			if (peek(1) == '\0') {
				lexer.append_inline = 1;
				advance();
				lexer_append();
				continue;
			}
			string_append(string, advance());
			string_append(string, advance());
			continue;
		}

		break;
	}

	return (lexer.input != start_buffer || (lexer.input && lexer.input->position != start_position));
}
