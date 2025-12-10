/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:30:57 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/10 14:47:54 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "parser/lexer.h"
	#include "utils/libft.h"

#pragma endregion

#pragma region "Operator"

	t_token *operator(t_lexer *lexer) {
		t_string	string;
		int			line = (lexer->input == lexer->user_buffer) ? lexer->line : -1;
		char		*full_line = (lexer->input) ? ft_strdup(lexer->input->value) : NULL;
		char		c = peek(lexer, 0);
		
		string_init(&string);

		if (c == '&' && peek(lexer, 1) == '&') {
			segment_append(segment,advance(lexer));
			segment_append(segment,advance(lexer));
			if (!lexer->input || ft_isspace_s(lexer->input->value + lexer->input->position)) lexer->append_inline = 2;
			return (token_create(lexer, TOKEN_AND, string.value, line, full_line));
		}
		if (c == '&') {
			segment_append(segment,advance(lexer));
			if (!lexer->input || ft_isspace_s(lexer->input->value + lexer->input->position)) lexer->append_inline = 2;
			return (token_create(lexer, TOKEN_BACKGROUND, string.value, line, full_line));
		}

		if (c == '|' && peek(lexer, 1) == '|') {
			segment_append(segment,advance(lexer));
			segment_append(segment,advance(lexer));
			if (!lexer->input || ft_isspace_s(lexer->input->value + lexer->input->position)) lexer->append_inline = 2;
			return (token_create(lexer, TOKEN_OR, string.value, line, full_line));
		}
		if (c == '|' && peek(lexer, 1) == '&') {
			segment_append(segment,advance(lexer));
			segment_append(segment,advance(lexer));
			if (!lexer->input || ft_isspace_s(lexer->input->value + lexer->input->position)) lexer->append_inline = 2;
			return (token_create(lexer, TOKEN_PIPE_ALL, string.value, line, full_line));
		}
		if (c == '|') {
			segment_append(segment,advance(lexer));
			if (!lexer->input || ft_isspace_s(lexer->input->value + lexer->input->position)) lexer->append_inline = 2;
			return (token_create(lexer, TOKEN_PIPE, string.value, line, full_line));
		}

		if (c == ';' && peek(lexer, 1) == ';' && peek(lexer, 1) == '&') {
			segment_append(segment,advance(lexer));
			segment_append(segment,advance(lexer));
			segment_append(segment,advance(lexer));
			return (token_create(lexer, TOKEN_CONTINUE, string.value, line, full_line));
		}
		if (c == ';' && peek(lexer, 1) == '&') {
			segment_append(segment,advance(lexer));
			segment_append(segment,advance(lexer));
			return (token_create(lexer, TOKEN_FALLTHROUGH, string.value, line, full_line));
		}
		if (c == ';' && peek(lexer, 1) == ';') {
			segment_append(segment,advance(lexer));
			segment_append(segment,advance(lexer));
			return (token_create(lexer, TOKEN_TERMINATOR, string.value, line, full_line));
		}
		if (c == ';') {
			segment_append(segment,advance(lexer));
			return (token_create(lexer, TOKEN_SEMICOLON, string.value, line, full_line));
		}

		free(full_line);
		free(string.value);
		return (NULL);
	}

#pragma endregion

// OPERATOR
//
// &		background
// &&		and
// ||		or
// |		pipe
// ;		semicolon
// #		comentario
// ;;		case terminator
// ;&		case fallthrough
// ;;&		case continue
