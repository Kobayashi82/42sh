/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:29:54 by vzurera-          #+#    #+#             */
/*   Updated: 2025/03/05 15:12:56 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"
	#include "parser/syntax.h"

#pragma endregion

#pragma region "Is Separator"
	
	bool is_separator(const char *input, size_t *i, char *last_token) {
			// Check for 2-character operators
		if (!ft_strncmp(&input[*i], "&&", 2) ||		// Logical AND
			!ft_strncmp(&input[*i], "||", 2) ||		// Logical OR
			!ft_strncmp(&input[*i], "$(", 2) ||		// Command Substitution
			!ft_strncmp(&input[*i], "${", 2) || 	// Parameter Expansion
			!ft_strncmp(&input[*i], "{ ", 2)) { 	// Command Group
			ft_strncpy(last_token, &input[*i], 2); last_token[2] = '\0';
			*i += 1;
			return (true);
		}	// Check for 1-character operators
		else if (ft_strchr(";|&\n", input[*i])) {
			ft_strncpy(last_token, &input[*i], 1); last_token[1] = '\0';
			return (true);
		}

		last_token[0] = '\0';
		return (false);
	}

	bool is_not_separator(char c) {
		return (c == '$' || c == '`' || c == '(' || c == ')' || c == '\'' || c == '"' || c == '{' || c == '}' || c == ';' || c == '&' || c == '|' || c == '\n' || ft_isspace(c));
	}

	bool is_end_arguments(const char *input, size_t i) {
		return (input[i] == '(' || input[i] == ')' || (input[i] == '{' && ft_isspace(input[i + 1])) || input[i] == '}' || input[i] == '\n' || ft_isspace(input[i]));
	}

#pragma endregion

#pragma region "Shell"
	
	int syntax_shell(const char *input, size_t *i, t_context *context, char *last_token, int *line) {
		if (!input || !*input || !context) return (0);

		bool command_start = true, is_argument = false;
		//int result = 0;
		(void) is_argument;

		while (input[*i]) {
			// if (context->stack && (context->stack->type == CTX_ARITHMETIC || context->stack->type == CTX_ARITHMETIC_GROUP)) {
			// 	if ((result = syntax_arithmetic(input, i, context, last_token, line))) return (result);
			// }

				//	\	Handle Escape
			if (context->in_escape) {
				context->in_escape = false; *i += 1; continue;
			} else if (input[*i] == '\\' && (!context->stack || context->stack->type != CTX_QUOTE)) {
				ft_strcpy(last_token, "\\");
				context->in_escape = true; *i += 1; continue;
			}
	
			if (input[*i] == '\n') *line += 1;
			//	'	Handle Single Quotes
			if (context->stack && context->stack->type == CTX_QUOTE) {
				if (input[*i] == '\'') stack_pop(&context->stack);
				command_start = false; *i += 1; continue;
			} else if (input[*i] == '\'') {
				if (!context->stack || (context->stack->type != CTX_ARITHMETIC && context->stack->type != CTX_ARITHMETIC_GROUP)) stack_push(&context->stack, CTX_QUOTE);
				command_start = false; *i += 1; continue;
			}
			
				//		Handle Spaces
			if (input[*i] != '\n' && ft_isspace(input[*i])) { *i += 1; continue; }
			
				//	}	Close Brace Expansion
			if (context->stack && context->stack->type == CTX_BRACE) {
				if (input[*i] == '}') stack_pop(&context->stack);
				command_start = false; *i += 1; continue;
			}	//	"	Close Double Quotes
			if (input[*i] == '"' && context->stack && context->stack->type == CTX_DQUOTE) {
				stack_pop(&context->stack);
				command_start = false; *i += 1; continue;
			}	//	))	Close Arithmetic Expansion or Arithmetic Expression
			else if (!ft_strncmp(&input[*i], "))", 2) && context->stack && context->stack->type == CTX_ARITHMETIC) {
				stack_pop(&context->stack);
				command_start = false; *i += 2; continue;
			}	//	)	Close Command Substitution or Subshell or Arithmetic Group
			else if (input[*i] == ')' && context->stack && (context->stack->type == CTX_SUBSHELL || context->stack->type == CTX_ARITHMETIC_GROUP || context->stack->type == CTX_PROCESS_SUB_IN || context->stack->type == CTX_PROCESS_SUB_OUT)) {
				stack_pop(&context->stack);
				command_start = false; *i += 1; continue;
			}	//	`	Close Backtick
			else if (input[*i] == '`' && is_context(context->stack, CTX_BACKTICK)) {
				while (context->stack && context->stack->type != CTX_BACKTICK) stack_pop(&context->stack);
				stack_pop(&context->stack);
				command_start = false; *i += 1; continue;
			}	//	}	Close Parameter Expansion or Command Group or Brace Expansion
			else if (input[*i] == '}' && (context->stack->type == CTX_BRACE_PARAM || context->stack->type == CTX_BRACE_COMMAND || context->stack->type == CTX_BRACE)) {
				stack_pop(&context->stack);
				command_start = false; *i += 1; continue;
			}
	
			//	"	Open Double Quotes
			if (input[*i] == '"' && (!context->stack || (context->stack->type != CTX_DQUOTE && context->stack->type != CTX_ARITHMETIC && context->stack->type != CTX_ARITHMETIC_GROUP))) {
				stack_push(&context->stack, CTX_DQUOTE);
				command_start = false; *i += 1; continue;
			}	//	$((	Open Arithmetic Expansion
			else if (!ft_strncmp(&input[*i], "$((", 3) && is_arithmetic(&input[*i + 3])) {
				stack_push(&context->stack, CTX_ARITHMETIC);
				command_start = false; *i += 3;
				continue;
			}	//	((	Open Arithmetic Expression
			else if (!ft_strncmp(&input[*i], "((", 2) && (!context->stack || context->stack->type != CTX_DQUOTE) && is_arithmetic(&input[*i + 2])) {
				if (!command_start) return (syntax_error(ARGS_ARITHMETIC, NULL, *line), 2);
				stack_push(&context->stack, CTX_ARITHMETIC);
				command_start = false; *i += 2;
				continue;
			}	//	$(	Open Command Substitution
			else if (!ft_strncmp(&input[*i], "$(", 2)) {
				stack_push(&context->stack, CTX_SUBSHELL);
				command_start = true; *i += 2; continue;
			}	//	<(	Open Process Substitution In
			else if (!ft_strncmp(&input[*i], "<(", 2)) {
				stack_push(&context->stack, CTX_PROCESS_SUB_IN);
				command_start = true; *i += 2; continue;
			}	//	>(	Open Process Substitution Out
			else if (!ft_strncmp(&input[*i], ">(", 2)) {
				stack_push(&context->stack, CTX_PROCESS_SUB_OUT);
				command_start = true; *i += 2; continue;	
			}	//	(	Open Arithmetic Group
			else if (input[*i] == '(' && context->stack && (context->stack->type == CTX_ARITHMETIC || context->stack->type == CTX_ARITHMETIC_GROUP)) {
				stack_push(&context->stack, CTX_ARITHMETIC_GROUP);
				command_start = false; *i += 1;
				continue;
			}	//	(	Open Subshell
			else if (input[*i] == '(' && (!context->stack || (context->stack->type != CTX_DQUOTE && context->stack->type != CTX_ARITHMETIC && context->stack->type != CTX_ARITHMETIC_GROUP))) {
				if (!command_start) return (syntax_error(ARGS_SUBSHELL, NULL, *line), 2);
				stack_push(&context->stack, CTX_SUBSHELL);
				command_start = true; *i += 1; continue;	
			}	//	`	Open Backtick
			else if (input[*i] == '`' && !is_context(context->stack, CTX_BACKTICK)) {
				stack_push(&context->stack, CTX_BACKTICK);
				command_start = true; *i += 1; continue;
			}	//	${	Open Parameter Expansion
			else if (!ft_strncmp(&input[*i], "${", 2)) {
				stack_push(&context->stack, CTX_BRACE_PARAM);
				command_start = false; *i += 2; continue;
			}	//	{ 	Open Command Group
			else if (input[*i] == '{' && ft_isspace(input[*i + 1]) && (!context->stack || (context->stack->type != CTX_ARITHMETIC && context->stack->type != CTX_ARITHMETIC_GROUP))) {
				if (!command_start) return (syntax_error(TOKEN_NEAR, ft_strdup("}"), *line), 2);
				stack_push(&context->stack, CTX_BRACE_COMMAND);
				command_start = true; *i += 1; continue;
			}	//	{	Open Brace Expansion
			else if (input[*i] == '{' && (!context->stack || (context->stack->type != CTX_DQUOTE && context->stack->type != CTX_ARITHMETIC && context->stack->type != CTX_ARITHMETIC_GROUP))) {
				stack_push(&context->stack, CTX_BRACE);
				command_start = false; *i += 1; continue;
			}	//	;	&	&&	|	||	\n	Command Separator
			else if (is_separator(input, i, last_token) && (!context->stack || (context->stack->type != CTX_DQUOTE && context->stack->type != CTX_ARITHMETIC && context->stack->type != CTX_ARITHMETIC_GROUP))) {
				command_start = true; *i += 1; continue;
			}

			if (command_start && (!context->stack || (context->stack->type != CTX_QUOTE && context->stack->type != CTX_DQUOTE && context->stack->type != CTX_ARITHMETIC && context->stack->type != CTX_ARITHMETIC_GROUP))) {
				size_t start = *i, end = *i;
				(void) start;
				while (input[end] && !is_not_separator(input[end])) end++;
				is_argument = !is_end_arguments(input, *i);
				is_separator(input, i, last_token);
				if (*last_token) is_argument = true;
				if (!input[end]) break;
				*i = end;
			} else *i += 1;		
			
			command_start = false;
		}

		return (0);
	}

#pragma endregion