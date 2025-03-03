/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 20:45:33 by vzurera-          #+#    #+#             */
/*   Updated: 2025/03/03 13:45:23 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"
	#include "terminal/print.h"
	#include "parser/input.h"
	#include "main/shell.h"
	#include "main/project.h"

#pragma endregion

#pragma region "Variables"

	typedef enum e_syntax_error {
		IN_TOKEN,
		IN_TOKEN_EOF,
	} t_syntax_error;

#pragma endregion

#pragma region "Is Separator"
	
	bool is_separator(const char *input, size_t *i, char *last_token) {
		if (!ft_strncmp(&input[*i], "&&", 2) || !ft_strncmp(&input[*i], "||", 2) || !ft_strncmp(&input[*i], "$(", 2) || !ft_strncmp(&input[*i], "${", 2) || !ft_strncmp(&input[*i], "{ ", 2)) {
			last_token[0] = input[*i]; last_token[1] = input[*i + 1]; last_token[2] = '\0';
			*i += 1; return (true);
		} else if (input[*i] == ';' || input[*i] == '|' || input[*i] == '&' || input[*i] == '\n') {
			last_token[0] = input[*i]; last_token[1] = '\0';
			return (true);
		}
		
		last_token[0] = '\0';
		return (false);
	}

	bool is_not_separator(char c) {
		return (c == '$' || c == '`' || c == '(' || c == ')' || c == '\'' || c == '"' || c == '{' || c == '}' || c == ';' || c == '&' || c == '|' || c == '\n' || ft_isspace(c));
	}

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

	return (false);
}

#pragma endregion

#pragma region "Syntax"

	#pragma region "Error"

		void syntax_error(t_syntax_error error, char *value, int line) {
			char *pname = ft_strjoin(PROYECTNAME, ": ", 0);
			if (shell.as_argument) pname = ft_strjoin(pname, "-c: ", 1);
			if (!shell.interactive && line >= 0) { 
				pname = ft_strjoin_sep(pname, "line ", ft_itoa(line), 6);
				pname = ft_strjoin(pname, ": ", 1);
			}

			if (value) value = ft_strjoin_sep("`", value, "'", 2);

			print(STDERR_FILENO, NULL, RESET);

			if (error == IN_TOKEN)
				print(STDERR_FILENO, ft_strjoin(pname, "syntax error: unexpected end of file", 0), FREE_JOIN);
			if (error == IN_TOKEN_EOF) {
				print(STDERR_FILENO, ft_strjoin_sep(pname, "unexpected EOF while looking for matching ", value, 0), FREE_JOIN);
			}

			print(STDERR_FILENO, "\n", PRINT);
			sfree(pname); sfree(value);
		}

	#pragma endregion

	#pragma region "Shell"
	
		static int syntax_shell(const char *input, size_t *i, t_context *context, char *last_token, int *line) {
			if (!input || !*input || !context) return (0);

			bool command_start = true;

			while (input[*i]) {
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
					stack_push(&context->stack, CTX_QUOTE);
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
				}	//	))	Close Arithmetic Expansion or Arithmetic Evaluation
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
				}	//	}	Close Parameter Expansion or Group Command or Brace Expansion
				else if (input[*i] == '}' && (context->stack->type == CTX_BRACE_PARAM || context->stack->type == CTX_BRACE_COMMAND || context->stack->type == CTX_BRACE)) {
					stack_pop(&context->stack);
					command_start = false; *i += 1; continue;
				}
		
				//	"	Open Double Quotes
				if (input[*i] == '"' && (!context->stack || context->stack->type != CTX_DQUOTE)) {
					stack_push(&context->stack, CTX_DQUOTE);
					command_start = false; *i += 1; continue;
				}	//	$((	Open Arithmetic Expansion
				else if (!ft_strncmp(&input[*i], "$((", 3) && is_arithmetic(&input[*i + 3])) {
					stack_push(&context->stack, CTX_ARITHMETIC);
					command_start = false; *i += 3; continue;
				}	//	((	Arithmetic Evaluation
				else if (!ft_strncmp(&input[*i], "((", 2) && (!context->stack || context->stack->type != CTX_DQUOTE) && is_arithmetic(&input[*i + 2])) {
					stack_push(&context->stack, CTX_ARITHMETIC);
					command_start = false; *i += 2; continue;
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
					command_start = false; *i += 1; continue;
				}	//	(	Open Subshell
				else if (input[*i] == '(' && (!context->stack || (context->stack->type != CTX_DQUOTE && context->stack->type != CTX_ARITHMETIC && context->stack->type != CTX_ARITHMETIC_GROUP))) {
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
				}	//	{ 	Open Group Command
				else if (!ft_strncmp(&input[*i], "{ ", 2) && (!context->stack || (context->stack->type != CTX_ARITHMETIC && context->stack->type != CTX_ARITHMETIC_GROUP))) {
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

					is_separator(input, i, last_token);
					if (!input[end]) break;
					*i = end;
					command_start = false;
					continue;
				}
		
				command_start = false;
				*i += 1;
			}

			return (0);
		}

	#pragma endregion

	#pragma region "Check"
	
		int	syntax_check(const char *input, t_context *context, int line) {
			if (!input || !*input || !context) return (1);

			char last_token[3]; last_token[0] = '\0';
			size_t i = 0;

			int result = syntax_shell(input, &i, context, last_token, &line);

			context->in_token = !is_context(context->stack, CTX_QUOTE) && !is_context(context->stack, CTX_DQUOTE) && (!ft_strncmp(last_token, "&&", 2) || !ft_strncmp(last_token, "||", 2) || *last_token == '|' || *last_token == '\\');
			context->in_escape = context->in_token && *last_token == '\\';

			// si hay cosas sin cerrar, error
			if (!shell.interactive && (context->stack || context->in_token)) {
				if (context->in_token) {
					syntax_error(IN_TOKEN, NULL, line);
				} else {
					char *value = NULL;
					if (context->stack->type == CTX_BRACE_COMMAND || context->stack->type == CTX_SUBSHELL || context->stack->type == CTX_PROCESS_SUB_IN || context->stack->type == CTX_PROCESS_SUB_OUT) line += 1;
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
	
#pragma endregion



// (echo lala
// bash: -c: line 2: syntax error: unexpected end of file
// $(echo lala
// bash: -c: line 2: unexpected EOF while looking for matching `)'
// ((echo lala
// bash: -c: line 1: unexpected EOF while looking for matching `)'
// $((echo lala
// bash: -c: line 1: unexpected EOF while looking for matching `)'
// <(echo lala
// bash: -c: line 2: unexpected EOF while looking for matching `)'
// $((echo (lala
// bash: -c: line 1: unexpected EOF while looking for matching `)'
// `echo lala
// bash: -c: line 1: unexpected EOF while looking for matching ``'
// ${
// bash: -c: line 1: unexpected EOF while looking for matching `}'
// {
// bash: -c: line 2: syntax error: unexpected end of file
// { echo; 
// bash: -c: line 2: syntax error: unexpected end of file
// echo |
// bash: -c: line 2: syntax error: unexpected end of file