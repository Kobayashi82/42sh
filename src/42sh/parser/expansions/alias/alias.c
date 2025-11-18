/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 20:58:15 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/18 23:01:22 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"
	#include "terminal/input.h"
	#include "parser/expansions/arithmetics.h"
	#include "parser/expansions/shell.h"
	#include "parser/syntax/syntax.h"
	#include "hashes/alias.h"
	#include "main/options.h"

	#define MAX_ALIAS_EXPANSIONS 50

#pragma endregion

#pragma region "Variables"

	static int	current_expansion;
	static char	*alias_list;

#pragma endregion

#pragma region "Expand"

		int expand_alias(char **input, t_context *context) {
			if (!options.expand_aliases || !input || !*input || !**input || !context) return (0);

			if (current_expansion++ == MAX_ALIAS_EXPANSIONS) { current_expansion = 0; return (1); }

			bool command_start = true;
			char last_token[3]; last_token[0] = '\0';
			size_t i = 0;

			while ((*input)[i]) {

					//	\	Handle Escape
				if (context->in_escape) {
					i += 1; context->in_escape = false; continue;
				} else if ((*input)[i] == '\\' && (!context->stack || context->stack->type != CTX_QUOTE)) {
					strcpy(last_token, "\\");
					i += 1; context->in_escape = true; continue;
				}

				//		Handle Spaces
				if ((*input)[i] != '\n' && ft_isspace((*input)[i])) { i += 1; continue; }

					//	'	Handle Single Quotes
				if (context->stack && context->stack->type == CTX_QUOTE) {
					if ((*input)[i] == '\'') stack_pop(&context->stack);
					i += 1; command_start = false; continue;
				} else if ((*input)[i] == '\'') {
					if (!context->stack || (context->stack->type != CTX_ARITHMETIC && context->stack->type != CTX_ARITHMETIC_GROUP)) stack_push(&context->stack, CTX_QUOTE);
					i += 1; command_start = false; continue;
				}
		
					//	"	Close Double Quotes
				if ((*input)[i] == '"' && context->stack && context->stack->type == CTX_DQUOTE) {
					i += 1; stack_pop(&context->stack);
					command_start = false; continue;
				}	//	))	Close Arithmetic Expansion or Arithmetic Expression
				else if (!strncmp(&(*input)[i], "))", 2) && context->stack && context->stack->type == CTX_ARITHMETIC) {
					i += 2; stack_pop(&context->stack);
					command_start = false; continue;
				}	//	)	Close Command Substitution or Subshell or Arithmetic Group
				else if ((*input)[i] == ')' && context->stack && (context->stack->type == CTX_SUBSHELL_COMMAND || context->stack->type == CTX_SUBSHELL || context->stack->type == CTX_ARITHMETIC_GROUP || context->stack->type == CTX_PROCESS_SUB_IN || context->stack->type == CTX_PROCESS_SUB_OUT)) {
					i += 1; stack_pop(&context->stack);
					command_start = false; continue;
				}	//	`	Close Backtick
				else if ((*input)[i] == '`' && is_context(context->stack, CTX_BACKTICK)) {
					while (context->stack && context->stack->type != CTX_BACKTICK) stack_pop(&context->stack);
					i += 1; stack_pop(&context->stack);
					command_start = false; continue;
				}	//	}	Close Parameter Expansion or Command Group
				else if ((*input)[i] == '}' && context->stack && (context->stack->type == CTX_BRACE_PARAM || context->stack->type == CTX_BRACE_COMMAND)) {
					i += 1; stack_pop(&context->stack);
					command_start = false; continue;
				}

				//	"	Open Double Quotes
				if ((*input)[i] == '"' && (!context->stack || (context->stack->type != CTX_ARITHMETIC && context->stack->type != CTX_ARITHMETIC_GROUP))) {
					i += 1; stack_push(&context->stack, CTX_DQUOTE);
					command_start = false; continue;
				}	//	$((	Open Arithmetic Expansion
				else if (!strncmp(&(*input)[i], "$((", 3) && is_arithmetic(&(*input)[i + 3])) {
					i += 3; stack_push(&context->stack, CTX_ARITHMETIC);
					command_start = false; continue;
				}	//	((	Open Arithmetic Expression
				else if (!strncmp(&(*input)[i], "((", 2) && (!context->stack || context->stack->type != CTX_DQUOTE) && is_arithmetic(&(*input)[i + 2])) {
					i += 2; stack_push(&context->stack, CTX_ARITHMETIC);
					command_start = false; continue;
				}	//	$(	Open Command Substitution
				else if (!strncmp(&(*input)[i], "$(", 2)) {
					i += 2; stack_push(&context->stack, CTX_SUBSHELL_COMMAND);
					command_start = true; continue;
				}	//	<(	Open Process Substitution In
				else if (!strncmp(&(*input)[i], "<(", 2)) {
					i += 2; stack_push(&context->stack, CTX_PROCESS_SUB_IN);
					command_start = true; continue;
				}	//	>(	Open Process Substitution Out
				else if (!strncmp(&(*input)[i], ">(", 2)) {
					i += 2; stack_push(&context->stack, CTX_PROCESS_SUB_OUT);
					command_start = true; continue;	
				}	//	(	Open Arithmetic Group
				else if ((*input)[i] == '(' && context->stack && (context->stack->type == CTX_ARITHMETIC || context->stack->type == CTX_ARITHMETIC_GROUP)) {
					i += 1; stack_push(&context->stack, CTX_ARITHMETIC_GROUP);
					command_start = false; continue;
				}	//	(	Open Subshell
				else if ((*input)[i] == '(' && (!context->stack || (context->stack->type != CTX_DQUOTE && context->stack->type != CTX_ARITHMETIC && context->stack->type != CTX_ARITHMETIC_GROUP))) {
					i += 1; stack_push(&context->stack, CTX_SUBSHELL);
					command_start = true; continue;	
				}	//	`	Open Backtick
				else if ((*input)[i] == '`' && !is_context(context->stack, CTX_BACKTICK)) {
					i += 1; stack_push(&context->stack, CTX_BACKTICK);
					command_start = true; continue;
				}	//	${	Open Parameter Expansion
				else if (!strncmp(&(*input)[i], "${", 2)) {
					i += 2; stack_push(&context->stack, CTX_BRACE_PARAM);
					command_start = false; continue;
				}	//	{ 	Open Command Group
				else if ((*input)[i] == '{' && ft_isspace((*input)[i + 1]) && (!context->stack || (context->stack->type != CTX_ARITHMETIC && context->stack->type != CTX_ARITHMETIC_GROUP))) {
					i += 1; stack_push(&context->stack, CTX_BRACE_COMMAND);
					command_start = true; continue;
				}	//	;	&	&&	|	||	\n	Command Separator
				else if ((!context->stack || (context->stack->type != CTX_DQUOTE && context->stack->type != CTX_ARITHMETIC && context->stack->type != CTX_ARITHMETIC_GROUP)) && is_separator(*input, &i, last_token)) {
					i += 1; command_start = true; continue;
				}
				else if (context->stack && (context->stack->type == CTX_ARITHMETIC || context->stack->type == CTX_ARITHMETIC_GROUP)) {
					i += 1; command_start = false; continue;
				}

				if (command_start && (!context->stack || (context->stack->type != CTX_QUOTE && context->stack->type != CTX_DQUOTE && context->stack->type != CTX_ARITHMETIC && context->stack->type != CTX_ARITHMETIC_GROUP))) {
					size_t start = i, end = i;

					while ((*input)[end] && !is_not_separator((*input)[end])) end++;
					char *alias_name  = strndup(*input + start, end - start);
					char *alias_value = NULL;
					if (alias_find_value(alias_name)) strdup(alias_find_value(alias_name));
					free(alias_name);
					char *tmp = ft_strjoin_sep("|", alias_value, "|", 0);

					if (alias_value && (!alias_list || !ft_strstr(alias_list, tmp))) {
						command_start = (*alias_value && ft_isspace(alias_value[ft_strlen(alias_value) - 1]));

						alias_list = ft_strjoin(alias_list, tmp, 3); tmp = NULL;
						expand_alias(&alias_value, context);
						size_t alias_len = ft_strlen(alias_value);

						char* new_input = replace_substring(*input, start, end - start, alias_value);
						free(alias_value); alias_value = NULL;
						free(alias_list); alias_list = NULL;

						is_separator(*input, &i, last_token);
						if (new_input) {
							free(*input);
							*input = new_input;
							i = start + alias_len;
							current_expansion = 0;
							continue;
						}
					}

					is_separator(*input, &i, last_token);
					current_expansion = 0;
					free(tmp); free(alias_value);
					free(alias_list); alias_list = NULL;
					if (!(*input)[end]) break;
					i = end;
					command_start = false;
					continue;
				}

				command_start = false;
				i += 1;
			}

			context->in_token = !is_context(context->stack, CTX_QUOTE) && !is_context(context->stack, CTX_DQUOTE) && (!strncmp(last_token, "&&", 2) || !strncmp(last_token, "||", 2) || *last_token == '|' || *last_token == '\\');
			context->in_escape = context->in_token && *last_token == '\\';
			return (0);
		}

#pragma endregion
