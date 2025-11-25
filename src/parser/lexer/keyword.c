/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyword.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:30:37 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/25 15:04:03 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/lexer.h"
#include "utils/libft.h"

// KEYWORD (count as WORD)
//
// if, then, else, elif, fi
// case, esac, in
// while, until, do, done
// for
// function
// !		keyword de negación (solo al inicio de pipeline)
// [[, ]]	test compuesto

t_token *keyword() {
	t_token *token = NULL;
	char	c = peek(0);
	(void) c;

	return (token);
}
