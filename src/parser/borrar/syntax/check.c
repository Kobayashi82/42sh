/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 20:45:33 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/27 23:42:43 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "utils/libft.h"
	#include "expansion/command.h"
	#include "parser/syntax.h"
	#include "main/shell.h"

#pragma endregion

#pragma region "Check"
	
	int	syntax_check(const char *input, t_context *context, int line) {
		if (!input || !*input || !context) return (1);

		char last_token[4]; last_token[0] = '\0';
		size_t i = 0;
	
		int result = syntax_shell(input, &i, context, last_token, &line);

		context->in_token = !is_context(context->stack, CTX_QUOTE) && !is_context(context->stack, CTX_DQUOTE) && (!strncmp(last_token, "&&", 2) || !strncmp(last_token, "||", 2) || *last_token == '|' || *last_token == '\\');
		context->in_escape = context->in_token && *last_token == '\\';

		if (shell.source == SRC_NO_INTERACTIVE && (context->stack || context->in_token)) {
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
