/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arithmetic.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:10:29 by vzurera-          #+#    #+#             */
/*   Updated: 2025/03/05 15:03:38 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//	Comprobar si {a,b,c} es válido, si no, no lo cuenta como stack
//	Comprobar si { cmd; } es válido, si no, no lo cuenta como stack
//	Básicamente {   } se considera literal
//	Saber cuando es comando o argumento


//	Ignorar comillas en aritmeticas dentro de expand_alias

//	Sintaxis arithmetics, ignorar comillas
//	Sintaxis arithmetics, numeros negativos.

//	En is_separator_arithmetic() si el char es + o - y está pegado a otro carácter que no es separador arithmetico

//	$(()), (()), ()	- syntax_arithmetics()
//	$(), `			- syntax_subshell()
//	${}				- syntax_param()

#pragma region "Includes"

	#include "libft.h"
	#include "parser/syntax.h"

#pragma endregion

#pragma region "Is Arithmetic"

	bool is_arithmetic(const char *input) {
		size_t	i = 0;
		int		parenthesis = 0;
		bool	in_quotes = false, in_dquotes = false, escape = false;

		while (input[i] && parenthesis >= 0) {
			// Manejar escapes
			if (escape)								{ escape = false;			i++; continue; }
			if (input[i] == '\\' && !in_quotes)		{ escape = true;			i++; continue; }

			// Manejar comillas
			if (input[i] == '\'' && !in_dquotes)	{ in_quotes  = !in_quotes;	i++; continue; }
			if (input[i] == '"'  && !in_quotes)		{ in_dquotes = !in_dquotes;	i++; continue; }

			if (input[i] == ')' && input[i + 1] == ')' && !parenthesis) return (true);
			if (input[i] == '(') parenthesis++;
			if (input[i] == ')') parenthesis--;
			i++;
	}

	
	if (!input[i] || input[i] == '\n') return (true);

	return (false);
}

#pragma endregion

#pragma region "Is Separator"

	static bool is_separator_arithmetic(const char *input, size_t *i, int *nvalue) {
		// Check for 3-character operators
		if (!ft_strncmp(&input[*i], "**=", 2) ||		// Power assignment
			!ft_strncmp(&input[*i], "<<=", 3) ||		// Left shift with assignment
			!ft_strncmp(&input[*i], ">>=", 3)) {		// Right shift with assignment
			*i += 3; *nvalue = 2; return (true);
		}	// Check for 2-character operators
		else if (!ft_strncmp(&input[*i], "&&", 2) ||	// Logical AND
			!ft_strncmp(&input[*i], "||", 2) ||			// Logical OR
			!ft_strncmp(&input[*i], "**", 2) ||			// Power
			!ft_strncmp(&input[*i], "<<", 2) || 		// Left Shift
			!ft_strncmp(&input[*i], ">>", 2) || 		// Right Shift
			!ft_strncmp(&input[*i], "==", 2) || 		// Equality
			!ft_strncmp(&input[*i], "!=", 2) || 		// Inequality
			!ft_strncmp(&input[*i], "<=", 2) || 		// Less than or equal
			!ft_strncmp(&input[*i], ">=", 2) || 		// Greater then or equal
			!ft_strncmp(&input[*i], "+=", 2) || 		// Addition assignment
			!ft_strncmp(&input[*i], "-=", 2) || 		// Subtraction assignment
			!ft_strncmp(&input[*i], "*=", 2) || 		// Multiplication assignment
			!ft_strncmp(&input[*i], "/=", 2) || 		// Division assignment
			!ft_strncmp(&input[*i], "%=", 2) || 		// Modulo assignment
			!ft_strncmp(&input[*i], "&=", 2) || 		// Bitwise AND assignment
			!ft_strncmp(&input[*i], "|=", 2) || 		// Bitwise OR assignment
			!ft_strncmp(&input[*i], "^=", 2)) { 		// Bitwise XOR assignment
			*i += 2; *nvalue = 2; return (true);
		}	// Check for Process Substitution In/Out
		else if (!ft_strncmp(&input[*i], "<(", 2) ||	// Process Substitution In
			!ft_strncmp(&input[*i], ">(", 2)) {			// Process Substitution Out
			return (false);
		}	// Check for 1-character operators
		else if (ft_strchr("|&+-*/%=<>!^", input[*i])) {			
			*i += 1; *nvalue = 2; return (true);
		}

		return (false);
	}

	static bool is_not_separator_arithmetic(const char *input, size_t *i) {
		if (ft_strchr("|&+-*/%=<>!^", input[*i]) || ft_isspace(input[*i])) {
			return (true);
		}

		return (false);
	}

	#pragma endregion

#pragma region "Arithmetic"

	int syntax_arithmetic(const char *input, size_t *i, t_context *context, char *last_token, int *line) {
		if (!input || !*input || !context) return (0);
		if (!context->stack || (context->stack->type != CTX_ARITHMETIC && context->stack->type != CTX_ARITHMETIC_GROUP)) return (0);
		
		int result = 0;
		if (!context->stack->nvalue) context->stack->nvalue = 2;

		while (input[*i]) {
				//	\	Handle Escape
			if (context->in_escape) {
				if (input[*i + 1]) context->stack->nvalue = 1;
				context->in_escape = false; *i += 1; continue;
			} else if (input[*i] == '\\' && (!context->stack || context->stack->type != CTX_QUOTE)) {
				ft_strcpy(last_token, "\\");
				context->in_escape = true; *i += 1; continue;
			}
			
			//		Handle Spaces
			if (input[*i] == '\n') *line += 1;
			if (input[*i] != '\n' && ft_isspace(input[*i])) { *i += 1; continue; }
			if (input[*i] == '"') { *i += 1; continue; }

			if (context->stack->nvalue == 2 && is_separator_arithmetic(input, i, &context->stack->nvalue)) {
				return (syntax_error(TOKEN_NEAR, ft_strdup("two operators"), *line), 2);
			}
			else if (is_separator_arithmetic(input, i, &context->stack->nvalue)) {
				continue;
			}
			
				//	))	Close Arithmetic Expansion or Arithmetic Expression
			if (!ft_strncmp(&input[*i], "))", 2) && context->stack && context->stack->type == CTX_ARITHMETIC) {
				if (context->stack->nvalue == 2) return (syntax_error(TOKEN_NEAR, ft_strdup("end with operator"), *line), 2);
				*i += 2; stack_pop(&context->stack);
				if (context->stack) context->stack->nvalue = 1;
				return(0);
			}	//	)	Close Command Substitution or Subshell or Arithmetic Group
			else if (input[*i] == ')' && context->stack && context->stack->type == CTX_ARITHMETIC_GROUP) {
				if (context->stack->nvalue == 2) return (syntax_error(TOKEN_NEAR, ft_strdup("end with operator"), *line), 2);
				*i += 1; stack_pop(&context->stack);
				if (context->stack) context->stack->nvalue = 1;
				return(0);
			}

				//	$((	Open Arithmetic Expansion
			else if (!ft_strncmp(&input[*i], "$((", 3) && is_arithmetic(&input[*i + 3])) {
				if (context->stack->nvalue == 1) return (syntax_error(TOKEN_NEAR, ft_strdup("expected operator"), *line), 2);
				*i += 3; stack_push(&context->stack, CTX_ARITHMETIC);
				if ((result = syntax_arithmetic(input, i, context, last_token, line))) return (result);
				continue;
			}	//	((	Open Arithmetic Expression
			else if (!ft_strncmp(&input[*i], "((", 2) && (!context->stack || context->stack->type != CTX_DQUOTE) && is_arithmetic(&input[*i + 2])) {
				if (context->stack->nvalue == 1) return (syntax_error(TOKEN_NEAR, ft_strdup("expected operator"), *line), 2);
				*i += 2; stack_push(&context->stack, CTX_ARITHMETIC);
				if ((result = syntax_arithmetic(input, i, context, last_token, line))) return (result);
				continue;
			}	//	(	Open Arithmetic Group
			else if (input[*i] == '(') {
				if (context->stack->nvalue == 1) return (syntax_error(TOKEN_NEAR, ft_strdup("expected operator"), *line), 2);
				*i += 1; stack_push(&context->stack, CTX_ARITHMETIC_GROUP);
				if ((result = syntax_arithmetic(input, i, context, last_token, line))) return (result);
				continue;
			}

				//	$(	Open Command Substitution
			else if (!ft_strncmp(&input[*i], "$(", 2)) {
				if (context->stack->nvalue == 1) return (syntax_error(TOKEN_NEAR, ft_strdup("expected operator"), *line), 2);
				*i += 2; stack_push(&context->stack, CTX_SUBSHELL);
				if ((result = syntax_shell(input, i, context, last_token, line))) return (result);
				continue;
			}	//	`	Open Backtick
			else if (input[*i] == '`' && !is_context(context->stack, CTX_BACKTICK)) {
				if (context->stack->nvalue == 1) return (syntax_error(TOKEN_NEAR, ft_strdup("expected operator"), *line), 2);
				*i += 1; stack_push(&context->stack, CTX_BACKTICK);
				if ((result = syntax_shell(input, i, context, last_token, line))) return (result);
				continue;
			}

				//	${	Open Parameter Expansion
			else if (!ft_strncmp(&input[*i], "${", 2)) {
				if (context->stack->nvalue == 1) return (syntax_error(TOKEN_NEAR, ft_strdup("expected operator"), *line), 2);
				*i += 2; stack_push(&context->stack, CTX_BRACE_PARAM);
				if ((result = syntax_parameter(input, i, context, last_token, line))) return (result);
				continue;
			}
			
			//	Forbidden Tokens

				//	<(	Open Process Substitution In/Out
			else if (!ft_strncmp(&input[*i], "<(", 2) || !ft_strncmp(&input[*i], ">(", 2)) {
				return (syntax_error(TOKEN_NEAR, ft_strdup("Process Subtitution"), *line), 2);
			}	//	{ 	Open Command Group
			else if (input[*i] == '{' && input[*i + 1] && ft_isspace(input[*i + 1])) {
				return (syntax_error(TOKEN_NEAR, ft_strdup("{ cmd; }"), *line), 2);
			}	//	{	Open Brace Expansion
			else if (input[*i] == '{') {
				return (syntax_error(TOKEN_NEAR, ft_strdup("{a,b,c}"), *line), 2);
			}

				//	Two operands togethers
			if (context->stack->nvalue == 1) {
				return (syntax_error(TOKEN_NEAR, ft_strdup("two numbers"), *line), 2);
			}

			while (input[*i] && (input[*i] == '"' || !is_not_separator_arithmetic(input, i))) *i += 1;
			context->stack->nvalue = 1;
		}

		return (0);
	}

#pragma endregion
