/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   group.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:52:00 by vzurera-          #+#    #+#             */
/*   Updated: 2025/03/05 22:12:37 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"
	#include "parser/syntax.h"

#pragma endregion

#pragma region "Group"

	int syntax_group(const char *input, size_t *i, t_context *context, char *last_token, int *line) {
		if (!input || !*input || !context) return (0);

		bool command_start = true, is_argument = false;
		int result = 0;
		size_t starting = *i;

		while (input[*i]) {
				//	\	Handle Escape
			if (context->in_escape) {
				*i += 1; context->in_escape = false; is_argument = true; continue;
			} else if (input[*i] == '\\' && (!context->stack || context->stack->type != CTX_QUOTE)) {
				ft_strcpy(last_token, "\\");
				*i += 1; context->in_escape = true; is_argument = true; continue;
			}
	
				//		Handle Spaces
			if (input[*i] == '\n') *line += 1;
			if (input[*i] != '\n' && ft_isspace(input[*i])) { *i += 1; continue; }

				//	'	Handle Single Quotes
			if (context->stack && context->stack->type == CTX_QUOTE) {
				if (input[*i] == '\'') stack_pop(&context->stack);
				*i += 1; command_start = false; is_argument = true; continue;
			} else if (input[*i] == '\'') {
				if (!context->stack || (context->stack->type != CTX_ARITHMETIC && context->stack->type != CTX_ARITHMETIC_EXPAND && context->stack->type != CTX_ARITHMETIC_GROUP)) stack_push(&context->stack, CTX_QUOTE);
				*i += 1; command_start = false; is_argument = true; continue;
			}
			
			if (input[*i] == ')' && (!context->stack || (context->stack->type != CTX_ARITHMETIC && context->stack->type != CTX_ARITHMETIC_EXPAND && context->stack->type != CTX_SUBSHELL_COMMAND && context->stack->type != CTX_SUBSHELL && context->stack->type != CTX_PROCESS_SUB_IN && context->stack->type != CTX_PROCESS_SUB_OUT))) return (syntax_error(TOKEN_NEAR, ft_strdup("invalid close"), *line), 2);
			if (input[*i] == '}' && (!context->stack || (context->stack->type != CTX_BRACE_PARAM && context->stack->type != CTX_BRACE_COMMAND))) return (syntax_error(TOKEN_NEAR, ft_strdup("invalid close"), *line), 2);

				//	"	Close Double Quotes
			if (input[*i] == '"' && context->stack && context->stack->type == CTX_DQUOTE) {
				*i += 1; stack_pop(&context->stack);
				command_start = false; is_argument = true; continue;
			}	//	))	Close Arithmetic Expansion or Arithmetic Expression
			else if (!ft_strncmp(&input[*i], "))", 2) && context->stack && (context->stack->type == CTX_ARITHMETIC || context->stack->type == CTX_ARITHMETIC_EXPAND)) {
				is_argument = (context->stack->type == CTX_ARITHMETIC_EXPAND);
				*i += 2; stack_pop(&context->stack);
				command_start = false; continue;
			}	//	)	Close Command Substitution or Subshell or Arithmetic Group
			else if (input[*i] == ')' && context->stack && (context->stack->type == CTX_SUBSHELL_COMMAND || context->stack->type == CTX_SUBSHELL || context->stack->type == CTX_ARITHMETIC_GROUP || context->stack->type == CTX_PROCESS_SUB_IN || context->stack->type == CTX_PROCESS_SUB_OUT)) {
				if (context->stack->type == CTX_SUBSHELL && starting == *i) return (syntax_error(TOKEN_NEAR, ft_strdup("empty subshell"), *line), 2);
				*i += 1; stack_pop(&context->stack);
				command_start = false; is_argument = true; continue;
			}	//	`	Close Backtick
			else if (input[*i] == '`' && is_context(context->stack, CTX_BACKTICK)) {
				while (context->stack && context->stack->type != CTX_BACKTICK) stack_pop(&context->stack);
				*i += 1; stack_pop(&context->stack);
				command_start = false; is_argument = true; continue;
			}	//	}	Close Parameter Expansion or Command Group
			else if (input[*i] == '}' && context->stack && (context->stack->type == CTX_BRACE_PARAM || context->stack->type == CTX_BRACE_COMMAND)) {
				if (context->stack->type == CTX_BRACE_COMMAND && *last_token != ';' && *last_token != '\n') return (syntax_error(TOKEN_NEAR, ft_strdup("invalid close }"), *line), 2);
				is_argument = (context->stack->type == CTX_BRACE_PARAM);
				*i += 1; stack_pop(&context->stack);
				command_start = false; continue;
			}
	
			//	"	Open Double Quotes
			if (input[*i] == '"' && (!context->stack || (context->stack->type != CTX_ARITHMETIC && context->stack->type != CTX_ARITHMETIC_EXPAND && context->stack->type != CTX_ARITHMETIC_GROUP))) {
				*i += 1; stack_push(&context->stack, CTX_DQUOTE);
				command_start = false; is_argument = true; continue;
			}	//	$((	Open Arithmetic Expansion
			else if (!ft_strncmp(&input[*i], "$((", 3) && is_arithmetic(&input[*i + 3])) {
				*i += 3; stack_push(&context->stack, CTX_ARITHMETIC_EXPAND);
				command_start = false; is_argument = true; continue;
			}	//	((	Open Arithmetic Expression
			else if (!ft_strncmp(&input[*i], "((", 2) && (!context->stack || context->stack->type != CTX_DQUOTE) && is_arithmetic(&input[*i + 2])) {
				if (!command_start) return (syntax_error(ARGS_ARITHMETIC, NULL, *line), 2);
				*i += 2; stack_push(&context->stack, CTX_ARITHMETIC);
				command_start = false; is_argument = false; continue;
			}	//	$(	Open Command Substitution
			else if (!ft_strncmp(&input[*i], "$(", 2)) {
				*i += 2; stack_push(&context->stack, CTX_SUBSHELL_COMMAND);
				if ((result = syntax_shell(input, i, context, last_token, line))) return (2);
				command_start = false; is_argument = true; continue;
			}	//	<(	Open Process Substitution In
			else if (!ft_strncmp(&input[*i], "<(", 2)) {
				*i += 2; stack_push(&context->stack, CTX_PROCESS_SUB_IN);
				if ((result = syntax_shell(input, i, context, last_token, line))) return (2);
				command_start = false; is_argument = true; continue;
			}	//	>(	Open Process Substitution Out
			else if (!ft_strncmp(&input[*i], ">(", 2)) {
				*i += 2; stack_push(&context->stack, CTX_PROCESS_SUB_OUT);
				if ((result = syntax_shell(input, i, context, last_token, line))) return (2);
				command_start = false; is_argument = true; continue;	
			}	//	(	Open Arithmetic Group
			else if (input[*i] == '(' && context->stack && (context->stack->type == CTX_ARITHMETIC || context->stack->type == CTX_ARITHMETIC_GROUP)) {
				*i += 1; stack_push(&context->stack, CTX_ARITHMETIC_GROUP);
				command_start = false; is_argument = false; continue;
			}	//	(	Open Subshell
			else if (input[*i] == '(' && (!context->stack || (context->stack->type != CTX_DQUOTE && context->stack->type != CTX_ARITHMETIC && context->stack->type != CTX_ARITHMETIC_GROUP))) {
				if (!command_start) return (syntax_error(ARGS_SUBSHELL, NULL, *line), 2);
				*i += 1; stack_push(&context->stack, CTX_SUBSHELL);
				if ((result = syntax_shell(input, i, context, last_token, line))) return (2);
				command_start = false; is_argument = false; continue;	
			}	//	`	Open Backtick
			else if (input[*i] == '`' && !is_context(context->stack, CTX_BACKTICK)) {
				*i += 1; stack_push(&context->stack, CTX_BACKTICK);
				if ((result = syntax_shell(input, i, context, last_token, line))) return (2);
				command_start = false; is_argument = true; continue;
			}	//	${	Open Parameter Expansion
			else if (!ft_strncmp(&input[*i], "${", 2)) {
				*i += 2; stack_push(&context->stack, CTX_BRACE_PARAM);
				command_start = false; is_argument = true; continue;
			}	//	{ 	Open Command Group
			else if (input[*i] == '{' && ft_isspace(input[*i + 1]) && (!context->stack || (context->stack->type != CTX_ARITHMETIC && context->stack->type != CTX_ARITHMETIC_EXPAND && context->stack->type != CTX_ARITHMETIC_GROUP))) {
				if (!command_start) return (syntax_error(TOKEN_NEAR, ft_strdup("invalid subshell"), *line), 2);
				*i += 1; stack_push(&context->stack, CTX_BRACE_COMMAND);
				if ((result = syntax_group(input, i, context, last_token, line))) return (2);
				command_start = true; is_argument = false; continue;
			}	//	;	&	&&	|	||	\n	Command Separator
			else if (is_separator(input, i, last_token) && (!context->stack || (context->stack->type != CTX_DQUOTE && context->stack->type != CTX_ARITHMETIC && context->stack->type != CTX_ARITHMETIC_EXPAND && context->stack->type != CTX_ARITHMETIC_GROUP))) {
				if (command_start && *last_token) return (syntax_error(TOKEN_NEAR, ft_strdup("two separators"), *line), 2);
				*i += 1; command_start = true; continue;
			}

			if (!command_start && !is_argument) return (syntax_error(TOKEN_NEAR, ft_strdup("invalid comand"), *line), 2);

			if (command_start && (!context->stack || (context->stack->type != CTX_QUOTE && context->stack->type != CTX_DQUOTE && context->stack->type != CTX_ARITHMETIC && context->stack->type != CTX_ARITHMETIC_EXPAND && context->stack->type != CTX_ARITHMETIC_GROUP))) {
				while (input[*i] && !is_not_separator(input[*i])) *i += 1;
				is_argument = !is_separator(input, i, last_token);

				if (!input[*i]) break;
			} else *i += 1;
			
			command_start = false;
		}

		return (0);
	}

#pragma endregion
