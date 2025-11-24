/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brace.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:52:02 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/24 19:38:23 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "utils/libft.h"
	#include "expansion/brace.h"
	#include "parser/syntax.h"

#pragma endregion

#pragma region "Brace"

	int syntax_brace(const char *input, size_t *i, t_context *context, char *last_token, int *line) {
		if (!input || !*input || !context) return (0);

		(void) i;
		(void) last_token;
		(void) line;

		return (0);
	}

#pragma endregion
