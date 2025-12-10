/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyword.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:30:37 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/10 16:03:49 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Word"

	#include "parser/lexer.h"
	#include "utils/libft.h"

#pragma endregion

#pragma region "Keyword"

	t_token *keyword(t_lexer *lexer) {
		int			line = (lexer->input == lexer->user_buffer) ? lexer->line : -1;
		char		*full_line = (lexer->input) ? ft_strdup(lexer->input->value) : NULL;
		char		c = peek(lexer, 0);
		
		t_segment *segment = segment_new(NULL);

		(void)c;
		(void)line;

		free(full_line);
		segment_free(segment);
		return (NULL);
	}

#pragma endregion

// KEYWORD (count as WORD)
//
// if, then, else, elif, fi
// case, esac, in
// while, until, do, done
// for
// function
// !		keyword de negación (solo al inicio de pipeline)
// [[, ]]	test compuesto
