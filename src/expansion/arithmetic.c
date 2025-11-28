/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arithmetic.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:10:29 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/28 22:03:28 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #pragma region "Is Separator"

	// static bool is_separator_arithmetic(const char *input, size_t *i, int *nvalue) {
	// 	// Check for 3-character operators
	// 	if (!strncmp(&input[*i], "**=", 2) ||		// Power assignment
	// 		!strncmp(&input[*i], "<<=", 3) ||		// Left shift with assignment
	// 		!strncmp(&input[*i], ">>=", 3)) {		// Right shift with assignment
	// 		*i += 3; *nvalue = 2; return (true);
	// 	}	// Check for 2-character operators
	// 	else if (!strncmp(&input[*i], "&&", 2) ||	// Logical AND
	// 		!strncmp(&input[*i], "||", 2) ||		// Logical OR
	// 		!strncmp(&input[*i], "**", 2) ||		// Power
	// 		!strncmp(&input[*i], "<<", 2) || 		// Left Shift
	// 		!strncmp(&input[*i], ">>", 2) || 		// Right Shift
	// 		!strncmp(&input[*i], "==", 2) || 		// Equality
	// 		!strncmp(&input[*i], "!=", 2) || 		// Inequality
	// 		!strncmp(&input[*i], "<=", 2) || 		// Less than or equal
	// 		!strncmp(&input[*i], ">=", 2) || 		// Greater then or equal
	// 		!strncmp(&input[*i], "+=", 2) || 		// Addition assignment
	// 		!strncmp(&input[*i], "-=", 2) || 		// Subtraction assignment
	// 		!strncmp(&input[*i], "*=", 2) || 		// Multiplication assignment
	// 		!strncmp(&input[*i], "/=", 2) || 		// Division assignment
	// 		!strncmp(&input[*i], "%=", 2) || 		// Modulo assignment
	// 		!strncmp(&input[*i], "&=", 2) || 		// Bitwise AND assignment
	// 		!strncmp(&input[*i], "|=", 2) || 		// Bitwise OR assignment
	// 		!strncmp(&input[*i], "^=", 2)) { 		// Bitwise XOR assignment
	// 		*i += 2; *nvalue = 2; return (true);
	// 	}	// Check for Process Substitution In/Out
	// 	else if (!strncmp(&input[*i], "<(", 2) ||	// Process Substitution In
	// 		!strncmp(&input[*i], ">(", 2)) {		// Process Substitution Out
	// 		return (false);
	// 	}	// Check for 1-character operators
	// 	else if (strchr("|&+-*/%=<>!^", input[*i])) {			
	// 		*i += 1; *nvalue = 2; return (true);
	// 	}

	// 	return (false);
	// }

	// static bool is_not_separator_arithmetic(const char *input, size_t *i) {
	// 	if (strchr("|&+-*/%=<>!^", input[*i]) || isspace(input[*i])) {
	// 		return (true);
	// 	}

	// 	return (false);
	// }

// #pragma endregion

// #pragma region "Arithmetic"

// 	int syntax_arithmetic(const char *input, size_t *i, t_context *context, char *last_token, int *line) {
// 		if (!input || !*input || !context) return (0);
// 		if (!context->stack || (context->stack->type != CTX_ARITHMETIC && context->stack->type != CTX_ARITHMETIC_EXPAND && context->stack->type != CTX_ARITHMETIC_GROUP)) return (0);
		
// 		int result = 0;
// 		if (!context->stack->nvalue) context->stack->nvalue = 2;

// 		while (input[*i]) {
// 				//	\	Handle Escape
// 			if (context->in_escape) {
// 				if (input[*i + 1]) context->stack->nvalue = 1;
// 				context->in_escape = false; *i += 1; continue;
// 			} else if (input[*i] == '\\' && (!context->stack || context->stack->type != CTX_QUOTE)) {
// 				strcpy(last_token, "\\");
// 				context->in_escape = true; *i += 1; continue;
// 			}
			
// 			//		Handle Spaces
// 			if (input[*i] == '\n') *line += 1;
// 			if (input[*i] != '\n' && isspace(input[*i])) { *i += 1; continue; }
// 			if (input[*i] == '"') { *i += 1; continue; }

// 			if (context->stack->nvalue == 2 && is_separator_arithmetic(input, i, &context->stack->nvalue)) {
// 				return (syntax_error(TOKEN_NEAR, ft_strdup("two operators"), *line), 2);
// 			}
// 			else if (is_separator_arithmetic(input, i, &context->stack->nvalue)) {
// 				continue;
// 			}
			
// 				//	))	Close Arithmetic Expansion or Arithmetic Expression
// 			if (!strncmp(&input[*i], "))", 2) && context->stack && (context->stack->type == CTX_ARITHMETIC || context->stack->type == CTX_ARITHMETIC_EXPAND)) {
// 				if (context->stack->nvalue == 2) return (syntax_error(TOKEN_NEAR, ft_strdup("end with operator"), *line), 2);
// 				*i += 2; ctx_stack_pop(&context->stack);
// 				if (context->stack) context->stack->nvalue = 1;
// 				return(0);
// 			}	//	)	Close Command Substitution or Subshell or Arithmetic Group
// 			else if (input[*i] == ')' && context->stack && context->stack->type == CTX_ARITHMETIC_GROUP) {
// 				if (context->stack->nvalue == 2) return (syntax_error(TOKEN_NEAR, ft_strdup("end with operator"), *line), 2);
// 				*i += 1; ctx_stack_pop(&context->stack);
// 				if (context->stack) context->stack->nvalue = 1;
// 				return(0);
// 			}

// 				//	$((	Open Arithmetic Expansion
// 			else if (!strncmp(&input[*i], "$((", 3) && is_arithmetic(&input[*i + 3])) {
// 				if (context->stack->nvalue == 1) return (syntax_error(TOKEN_NEAR, ft_strdup("expected operator"), *line), 2);
// 				*i += 3; ctx_stack_push(&context->stack, CTX_ARITHMETIC_EXPAND);
// 				if ((result = syntax_arithmetic(input, i, context, last_token, line))) return (result);
// 				continue;
// 			}	//	((	Open Arithmetic Expression
// 			else if (!strncmp(&input[*i], "((", 2) && (!context->stack || context->stack->type != CTX_DQUOTE) && is_arithmetic(&input[*i + 2])) {
// 				if (context->stack->nvalue == 1) return (syntax_error(TOKEN_NEAR, ft_strdup("expected operator"), *line), 2);
// 				*i += 2; ctx_stack_push(&context->stack, CTX_ARITHMETIC);
// 				if ((result = syntax_arithmetic(input, i, context, last_token, line))) return (result);
// 				continue;
// 			}	//	(	Open Arithmetic Group
// 			else if (input[*i] == '(') {
// 				if (context->stack->nvalue == 1) return (syntax_error(TOKEN_NEAR, ft_strdup("expected operator"), *line), 2);
// 				*i += 1; ctx_stack_push(&context->stack, CTX_ARITHMETIC_GROUP);
// 				if ((result = syntax_arithmetic(input, i, context, last_token, line))) return (result);
// 				continue;
// 			}

// 				//	$(	Open Command Substitution
// 			else if (!strncmp(&input[*i], "$(", 2)) {
// 				if (context->stack->nvalue == 1) return (syntax_error(TOKEN_NEAR, ft_strdup("expected operator"), *line), 2);
// 				*i += 2; ctx_stack_push(&context->stack, CTX_SUBSHELL_COMMAND);
// 				if ((result = syntax_shell(input, i, context, last_token, line))) return (result);
// 				continue;
// 			}	//	`	Open Backtick
// 			else if (input[*i] == '`' && !ctx_is_context(context->stack, CTX_BACKTICK)) {
// 				if (context->stack->nvalue == 1) return (syntax_error(TOKEN_NEAR, ft_strdup("expected operator"), *line), 2);
// 				*i += 1; ctx_stack_push(&context->stack, CTX_BACKTICK);
// 				if ((result = syntax_shell(input, i, context, last_token, line))) return (result);
// 				continue;
// 			}

// 				//	${	Open Parameter Expansion
// 			else if (!strncmp(&input[*i], "${", 2)) {
// 				if (context->stack->nvalue == 1) return (syntax_error(TOKEN_NEAR, ft_strdup("expected operator"), *line), 2);
// 				*i += 2; ctx_stack_push(&context->stack, CTX_BRACE_PARAM);
// 				if ((result = syntax_parameter(input, i, context, last_token, line))) return (result);
// 				continue;
// 			}
			
// 			//	Forbidden Tokens

// 				//	<(	Open Process Substitution In/Out
// 			else if (!strncmp(&input[*i], "<(", 2) || !strncmp(&input[*i], ">(", 2)) {
// 				return (syntax_error(TOKEN_NEAR, ft_strdup("Process Subtitution"), *line), 2);
// 			}	//	{ 	Open Command Group
// 			else if (input[*i] == '{' && input[*i + 1] && isspace(input[*i + 1])) {
// 				return (syntax_error(TOKEN_NEAR, ft_strdup("{ cmd; }"), *line), 2);
// 			}	//	{	Open Brace Expansion
// 			else if (input[*i] == '{') {
// 				return (syntax_error(TOKEN_NEAR, ft_strdup("{a,b,c}"), *line), 2);
// 			}

// 				//	Two operands togethers
// 			if (context->stack->nvalue == 1) {
// 				return (syntax_error(TOKEN_NEAR, ft_strdup("two numbers"), *line), 2);
// 			}

// 			while (input[*i] && (input[*i] == '"' || !is_not_separator_arithmetic(input, i))) *i += 1;
// 			context->stack->nvalue = 1;
// 		}

// 		return (0);
// 	}

// #pragma endregion
