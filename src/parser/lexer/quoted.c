/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quoted.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 20:35:54 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/29 14:24:47 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/libft.h"
#include "parser/lexer.h"

char handle_quotes() {
	char	c = peek(0);
	size_t	start = lexer.input->position;

	while (c) {
		if (stack_top() == '\'') {
			if (c == '\'') {
				stack_pop();
				c = advance(1);
			} else if (peek(1) == '\0') {
				c = advance(1);
				lexer_append_input();
				continue;
			} else {
				c = advance(1);
				continue;
			}
		}
		if (c == '\'') {
			stack_push('\'');
			c = advance(1);
			continue;
		}

		if (stack_top() == '"') {
			if (c == '"') {
				stack_pop();
				c = advance(1);
			} else if (peek(1) == '\0') {
				if (c == '\\') {
					lexer.append_inline = 1;
					lexer_append_input();
					continue;
				} else {
					c = advance(1);
					lexer_append_input();
					continue;
				}
			} else {
				if (c == '\\') c = advance(1);
				c = advance(1);
				continue;
			}
		}
		if (c == '"') {
			stack_push('"');
			c = advance(1);
			continue;
		}

		if (c == '\\') {
			if (peek(1) == '\0') {
				lexer.append_inline = 1;
				lexer_append_input();
				continue;
			}
			c = advance(2);
			continue;
		}

		break;
	}

	return (start != lexer.input->position);
}

int is_space(int n) {
	if (n < 0 && (int)lexer.input->position < n) return (0);
	if (lexer.input->position + n >= ft_strlen(lexer.input->value)) return (1);

	char c = lexer.input->value[lexer.input->position + n];
	return (isspace(c) || c == '\0');
}
