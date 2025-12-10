/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 18:01:47 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/10 18:19:00 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "parser/lexer.h"
	#include "utils/libft.h"

#pragma endregion

#pragma region "Free"

	void token_free(t_token *token) {
		if (token) {
			segment_free(token->segment);
			free(token->filename);
			free(token->full_line);
			free(token);
		}
	}

#pragma endregion

#pragma region "Create"

	t_token *token_create(t_lexer *lexer, int type, t_segment *segment, int line, char *full_line) {
		t_token *token = malloc(sizeof(t_token));
		token->type = type;
		token->segment = segment;
		token->right_space = lexer->right_space = isspace(peek(lexer, 0)) || peek(lexer, 0) == '\0';
		token->filename = ft_strdup(lexer->filename);
		token->line = line;
		token->full_line = full_line;

		return (token);
	}

#pragma endregion

#pragma region "Get"

	t_token *token_get(t_lexer *lexer) {
		if (!stack_top(lexer)) while (isspace(peek(lexer, 0)) && peek(lexer, 0) != '\n') advance(lexer);

		if (peek(lexer, 0) == '\n') {
			int		line = (lexer->input == lexer->user_buffer) ? lexer->line : -1;
			char	*full_line = (lexer->input) ? ft_strdup(lexer->input->value) : NULL;

			t_segment *segment = segment_new(NULL);
			segment_append(segment, advance(lexer));

			return (token_create(lexer, TOKEN_NEWLINE, segment, line, full_line));
		}

		if (peek(lexer, 0)) {
			t_token *token = NULL;
			// if ((token = variables(lexer)))		return (token, 0);
			// if ((token = expansion(lexer)))		return (token, 0);
			// if ((token = operator(lexer)))		return (token);
			// if ((token = redirection(lexer)))	return (token);
			// if ((token = keyword(lexer)))		return (token);
			if ((token = word(lexer)))			return (token);
		}

		if (!lexer->interactive || lexer->append_inline) {
			lexer_append(lexer);
			if (lexer->input) return (token_get(lexer));
		}

		return (token_create(lexer, TOKEN_EOF, NULL, lexer->line + 1, NULL));
	}

#pragma endregion
