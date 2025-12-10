/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 18:01:47 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/10 16:07:13 by vzurera-         ###   ########.fr       */
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
