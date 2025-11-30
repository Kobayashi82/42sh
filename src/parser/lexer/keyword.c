/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyword.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:30:37 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/30 21:06:28 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Word"

	#include "parser/lexer.h"
	#include "utils/libft.h"

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

#pragma region "Keyword"

	t_token *keyword(t_lexer *lexer) {
		t_token *token = NULL;
		char	c = peek(lexer, 0);
		(void) c;

		return (token);
	}

#pragma endregion
