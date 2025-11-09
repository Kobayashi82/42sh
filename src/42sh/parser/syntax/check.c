/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 20:45:33 by vzurera-          #+#    #+#             */
/*   Updated: 2025/03/06 13:43:11 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"
	#include "parser/expansions/shell.h"
	#include "parser/syntax/syntax.h"
	#include "main/shell.h"

#pragma endregion

#pragma region "Check"
	
	int	syntax_check(const char *input, t_context *context, int line) {
		if (!input || !*input || !context) return (1);

		char last_token[4]; last_token[0] = '\0';
		size_t i = 0;
	
		int result = syntax_shell(input, &i, context, last_token, &line);

		context->in_token = !is_context(context->stack, CTX_QUOTE) && !is_context(context->stack, CTX_DQUOTE) && (!ft_strncmp(last_token, "&&", 2) || !ft_strncmp(last_token, "||", 2) || *last_token == '|' || *last_token == '\\');
		context->in_escape = context->in_token && *last_token == '\\';

		if (!shell.interactive && (context->stack || context->in_token)) {
			if (context->in_token) {
				syntax_error(IN_TOKEN, NULL, line);
			} else {
				char *value = NULL;
				if (context->stack->type == CTX_QUOTE) value = ft_strdup("'");
				else if (context->stack->type == CTX_DQUOTE) value = ft_strdup("\"");
				else if (context->stack->type == CTX_BACKTICK) value = ft_strdup("`");
				else if (context->stack->type == CTX_BRACE || context->stack->type == CTX_BRACE_PARAM || context->stack->type == CTX_BRACE_COMMAND) value = ft_strdup("}");
				else value = ft_strdup(")");
				syntax_error(IN_TOKEN_EOF, value, line);
			}
			result = 2;
		}

		if (result) shell.exit_code = result;

		return (result);
	}

#pragma endregion
