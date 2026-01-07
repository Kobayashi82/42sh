/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:30:57 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/07 23:49:36 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "parser/lexer.h"
	#include "utils/utils.h"

#pragma endregion

#pragma region "Operator"

	t_token *operator(t_lexer *lexer) {
		int			line = (lexer->input == lexer->user_buffer) ? lexer->line : -1;
		char		*full_line = (lexer->input) ? ft_strdup(lexer->input->value) : NULL;
		char		c = peek(lexer, 0);
		
		t_segment *segment = segment_new(NULL);

		if (c == '&' && peek(lexer, 1) == '&') {
			segment_append(segment, advance(lexer));
			segment_append(segment, advance(lexer));
			if (!lexer->input || ft_isspace_s(lexer->input->value + lexer->input->position)) lexer->append_inline = 2;
			return (token_create(lexer, TOKEN_AND, segment, line, full_line));
		}
		if (c == '&') {
			segment_append(segment, advance(lexer));
			if (!lexer->input || ft_isspace_s(lexer->input->value + lexer->input->position)) lexer->append_inline = 2;
			return (token_create(lexer, TOKEN_BACKGROUND, segment, line, full_line));
		}

		if (c == '|' && peek(lexer, 1) == '|') {
			segment_append(segment, advance(lexer));
			segment_append(segment, advance(lexer));
			if (!lexer->input || ft_isspace_s(lexer->input->value + lexer->input->position)) lexer->append_inline = 2;
			return (token_create(lexer, TOKEN_OR, segment, line, full_line));
		}
		if (c == '|' && peek(lexer, 1) == '&') {
			segment_append(segment, advance(lexer));
			segment_append(segment, advance(lexer));
			if (!lexer->input || ft_isspace_s(lexer->input->value + lexer->input->position)) lexer->append_inline = 2;
			return (token_create(lexer, TOKEN_PIPE_ALL, segment, line, full_line));
		}
		if (c == '|') {
			segment_append(segment, advance(lexer));
			if (!lexer->input || ft_isspace_s(lexer->input->value + lexer->input->position)) lexer->append_inline = 2;
			return (token_create(lexer, TOKEN_PIPE, segment, line, full_line));
		}

		if (c == ';' && peek(lexer, 1) == ';' && peek(lexer, 1) == '&') {
			segment_append(segment, advance(lexer));
			segment_append(segment, advance(lexer));
			segment_append(segment, advance(lexer));
			return (token_create(lexer, TOKEN_CONTINUE, segment, line, full_line));
		}
		if (c == ';' && peek(lexer, 1) == '&') {
			segment_append(segment, advance(lexer));
			segment_append(segment, advance(lexer));
			return (token_create(lexer, TOKEN_FALLTHROUGH, segment, line, full_line));
		}
		if (c == ';' && peek(lexer, 1) == ';') {
			segment_append(segment, advance(lexer));
			segment_append(segment, advance(lexer));
			return (token_create(lexer, TOKEN_TERMINATOR, segment, line, full_line));
		}
		if (c == ';') {
			segment_append(segment, advance(lexer));
			return (token_create(lexer, TOKEN_SEMICOLON, segment, line, full_line));
		}

		free(full_line);
		segment_free(segment);
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
