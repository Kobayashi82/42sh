/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_alias.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 20:58:15 by vzurera-          #+#    #+#             */
/*   Updated: 2025/03/01 14:34:11 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"
	#include "parser/input.h"
	#include "hashes/alias.h"
	#include "main/options.h"

	#define MAX_ALIAS_EXPANSIONS 50

#pragma endregion

#pragma region "Variables"

	static int	current_expansion;
	static char	*alias_list;

#pragma endregion

#pragma region "Expand"

	#pragma region "Replace"

		static char *replace_substring(char *original, size_t start, size_t len, const char *replacement) {
			size_t orig_len = ft_strlen(original);
			size_t repl_len = ft_strlen(replacement);
			
			if (start > orig_len)		return (NULL);
			if (start + len > orig_len)	len = orig_len - start;
			
			char *new_str = smalloc(orig_len - len + repl_len + 1);
			
			ft_strncpy(new_str, original, start);
			new_str[start] = '\0';
			ft_strcat(new_str, replacement);
			if (start + len < orig_len) ft_strcat(new_str, original + start + len);
			
			return (new_str);
		}

	#pragma endregion

	#pragma region "Is Separator"
	
		static bool is_separator(char *input, size_t *i, char *last_token) {
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
	
		static bool is_not_separator(char c) {
			return (c == '$' || c == '`' || c == '(' || c == ')' || c == '\'' || c == '"' || c == '{' || c == '}' || c == ';' || c == '&' || c == '|' || c == '\n' || ft_isspace(c));
		}

	#pragma endregion

	#pragma region "Is Arithmetic"

		static bool is_arithmetic(const char *input) {
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

	#pragma region "Is Close Backtick"
		
		int is_close_backtick(t_stack *stack) {
			while (stack) {
				if (stack->type == CTX_BACKTICK) return (1);
				stack = stack->prev;
			}

			return (0);
		}

	#pragma endregion

	#pragma region "Expand"

		int expand_alias(char **input, t_context *context) {
			if (!options.expand_aliases || !input || !*input || !**input) return (0);

			if (current_expansion++ == MAX_ALIAS_EXPANSIONS) {
				current_expansion = 0; return (1); }

			bool command_start = true;
			char last_token[3]; last_token[0] = '\0';
			size_t i = 0;

			while ((*input)[i]) {
				// Handle Escape
				if (context->in_escape)								{ context->in_escape = false;									i++; continue; }
				if ((*input)[i] == '\\' && !context->in_quotes)		{ context->in_escape = true;	ft_strcpy(last_token, "\\");	i++; continue; }
				// Handle Quotes
				if ((*input)[i] == '\'' && !context->in_dquotes)	{ context->in_quotes  = !context->in_quotes;					i++; continue; }
				if ((*input)[i] == '"'  && !context->in_quotes)		{ context->in_dquotes = !context->in_dquotes;					i++; continue; }
				// Handle Spaces
				if ((*input)[i] != '\n' && ft_isspace((*input)[i])) {																i++; continue; }

				if (!context->in_quotes && !context->in_dquotes) {

					if (context->stack && context->stack->type == CTX_BRACE) {
						if ((*input)[i] == '}') stack_pop(&context->stack);
						command_start = false; i += 1; continue;
					}
					
						//	$((	Open Arithmetic Expansion
					if (!ft_strncmp(&(*input)[i], "$((", 3) && is_arithmetic(&(*input)[i + 3])) {
						stack_push(&context->stack, CTX_ARITHMETIC);
						command_start = false; i += 3; continue;
					}	//	((	Arithmetic Evaluation
					else if (!ft_strncmp(&(*input)[i], "((", 2)  && is_arithmetic(&(*input)[i + 2])) {
						stack_push(&context->stack, CTX_ARITHMETIC);
						command_start = false; i += 2; continue;
					}	//	$(	Open Command Substitution
					else if (!ft_strncmp(&(*input)[i], "$(", 2)) {
						stack_push(&context->stack, CTX_SUBSHELL);
						command_start = true; i += 2; continue;
					}	//	(	Open Arithmetic Group
					else if ((*input)[i] == '(' && context->stack && (context->stack->type == CTX_ARITHMETIC || context->stack->type == CTX_ARITHMETIC_GROUP)) {
						stack_push(&context->stack, CTX_ARITHMETIC_GROUP);
						command_start = false; i += 1; continue;
					}	//	(	Open Subshell
					else if ((*input)[i] == '(' && (!context->stack || (context->stack->type != CTX_ARITHMETIC && context->stack->type != CTX_ARITHMETIC_GROUP))) {
						stack_push(&context->stack, CTX_SUBSHELL);
						command_start = true; i += 1; continue;	
					}	//	`	Open Backtick
					else if ((*input)[i] == '`' && !is_close_backtick(context->stack)) {
						stack_push(&context->stack, CTX_BACKTICK);
						command_start = true; i += 1; continue;
					}	//	${	Open Parameter Expansion
					else if (!ft_strncmp(&(*input)[i], "${", 2)) {
						stack_push(&context->stack, CTX_BRACE_PARAM);
						command_start = false; i += 1; continue;
					}	//	{ 	Open Group Command
					else if (!ft_strncmp(&(*input)[i], "{ ", 2) && (!context->stack || (context->stack->type != CTX_ARITHMETIC && context->stack->type != CTX_ARITHMETIC_GROUP))) {
						stack_push(&context->stack, CTX_BRACE_COMMAND);
						command_start = true; i += 1; continue;
					}	//	{	Open Brace Expansion
					else if ((*input)[i] == '{' && (!context->stack || (context->stack->type != CTX_ARITHMETIC && context->stack->type != CTX_ARITHMETIC_GROUP))) {
						stack_push(&context->stack, CTX_BRACE);
						command_start = false; i += 1; continue;
					}	//	;	&	&&	|	||	\n	Command Separator
					else if (is_separator(*input, &i, last_token) && (!context->stack || (context->stack->type != CTX_ARITHMETIC && context->stack->type != CTX_ARITHMETIC_GROUP))) {
						command_start = true; i++; continue;
					}

						//	))	Close Arithmetic Expansion or Arithmetic Evaluation
					if (!ft_strncmp(&(*input)[i], "))", 2) && context->stack && context->stack->type == CTX_ARITHMETIC) {
						stack_pop(&context->stack);
						command_start = false; i += 2; continue;
					}	//	)	Close Command Substitution or Subshell or Arithmetic Group
					else if (context->stack && (context->stack->type == CTX_SUBSHELL || context->stack->type == CTX_ARITHMETIC_GROUP) && (*input)[i] == ')') {
						stack_pop(&context->stack);						
						command_start = false; i += 1; continue;
					}	//	`	Close Backtick
					else if ((*input)[i] == '`' && is_close_backtick(context->stack)) {
						while (context->stack && context->stack->type != CTX_BACKTICK) stack_pop(&context->stack);
						stack_pop(&context->stack);
						command_start = false; i += 1; continue;
					}	//	}	Close Parameter Expansion or Group Command or Brace Expansion
					else if ((*input)[i] == '}' && (context->stack->type == CTX_BRACE_PARAM || context->stack->type == CTX_BRACE_COMMAND || context->stack->type == CTX_BRACE)) {
						stack_pop(&context->stack);
						command_start = false; i += 1; continue;
					}
				}

				// Expandir aliases cuando corresponde
				if (command_start && !context->in_quotes && !context->in_dquotes && (!context->stack || (context->stack->type != CTX_ARITHMETIC && context->stack->type != CTX_ARITHMETIC_GROUP))) {
					size_t start = i, end = i;
					
					// Encontrar fin del posible alias
					while ((*input)[end] && !is_not_separator((*input)[end])) end++;
					char *alias_name  = ft_strndup(*input + start, end - start);
					char *alias_value = ft_strdup(alias_find_value(alias_name)); sfree(alias_name);
					char *tmp = ft_strjoin_sep("|", alias_value, "|", 0);
					
					if (alias_value && (!alias_list || !ft_strstr(alias_list, tmp))) {
						command_start = (*alias_value && ft_isspace(alias_value[ft_strlen(alias_value) - 1]));
						
						alias_list = ft_strjoin(alias_list, tmp, 3); tmp = NULL;
						expand_alias(&alias_value, context);
						size_t alias_len = ft_strlen(alias_value);
						
						char* new_input = replace_substring(*input, start, end - start, alias_value);
						sfree(alias_value); alias_value = NULL;
						sfree(alias_list); alias_list = NULL;
						
						if (new_input) {
							sfree(*input);
							*input = new_input;
							i = start + alias_len;
							current_expansion = 0;
							continue;
						}
					}

					current_expansion = 0;
					sfree(tmp); sfree(alias_value);
					sfree(alias_list); alias_list = NULL;
					if (!(*input)[end]) break;
					i = end;
				}

				command_start = false;
				i++;
			}
			
			context->in_token = !context->in_quotes && !context->in_dquotes && (!ft_strncmp(last_token, "&&", 2) || !ft_strncmp(last_token, "||", 2) || *last_token == '|' || *last_token == '\\');
			context->in_escape = context->in_token && *last_token == '\\';
			return (0);
		}

	#pragma endregion

#pragma endregion
