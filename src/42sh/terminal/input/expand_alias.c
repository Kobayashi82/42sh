/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_alias.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 20:58:15 by vzurera-          #+#    #+#             */
/*   Updated: 2025/02/27 19:23:10 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"
	#include "terminal/input.h"
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

	#pragma region "Stack"

		#pragma region "Push"

			static t_context *stack_push(t_context **top, t_type type) {
				t_context *new_node = smalloc(sizeof(t_context));
				
				new_node->type = type;
				new_node->nest_level = (*top) ? (*top)->nest_level + 1 : 1;
				new_node->prev = *top;
				*top = new_node;

				return (new_node);
			}

		#pragma endregion

		#pragma region "Pop"

			static void stack_pop(t_context **top) {
				if (*top) {
					t_context *temp = *top;
					*top = (*top)->prev;
					sfree(temp);
				}
			}

		#pragma endregion

		#pragma region "Clear"

			void stack_clear(t_context **stack) {
				while (*stack) stack_pop(stack);
			}

		#pragma endregion

	#pragma endregion

	#pragma region "Is Separator"

		static bool is_separator(char c, int type) {
			if (!type)	return (c == ';' || c == '|' || c == '&' || c == '\n');
			else		return (c == '$' || c == '`' || c == ')' || ft_isspace(c));
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
		
		int is_close_backtick(t_context *stack) {
			while (stack) {
				if (stack->type == CTX_BACKTICK) return (1);
				stack = stack->prev;
			}

			return (0);
		}

	#pragma endregion

	#pragma region "Expand"

		int expand_alias(char **input, t_context **stack, bool *in_quotes, bool *in_dquotes, bool *escape) {
			if (current_expansion++ == MAX_ALIAS_EXPANSIONS) {
				current_expansion = 0; return (1); }

			bool	command_start = true;
			size_t	i = 0;

			while ((*input)[i]) {

				// Handle Escape
				if (*escape)								{ *escape = false;				i++; continue; }
				if ((*input)[i] == '\\' && !*in_quotes)		{ *escape = true;				i++; continue; }
				// Handle Quotes
				if ((*input)[i] == '\'' && !*in_dquotes)	{ *in_quotes  = !*in_quotes;	i++; continue; }
				if ((*input)[i] == '"'  && !*in_quotes)		{ *in_dquotes = !*in_dquotes;	i++; continue; }
				// Handle Spaces
				if ((*input)[i] != '\n' && ft_isspace((*input)[i])) {						i++; continue; }

				if (!*in_quotes && !*in_dquotes) {
						// Open Arithmetic Expansion				$((
					if (!ft_strncmp(&(*input)[i], "$((", 3) && is_arithmetic(&(*input)[i + 3])) {
						stack_push(stack, CTX_ARITHMETIC);
						command_start = false; i += 3; continue;
					}	// Arithmetic Evaluation					((
					else if (!ft_strncmp(&(*input)[i], "((", 2)  && is_arithmetic(&(*input)[i + 2])) {
						stack_push(stack, CTX_ARITHMETIC);
						command_start = false; i += 2; continue;
					}	// Open Command Substitution				$(
					else if (!ft_strncmp(&(*input)[i], "$(", 2)) {
						stack_push(stack, CTX_SUBSHELL);
						command_start = true; i += 2; continue;
					}	// Open Subshell 							(
					else if ((*input)[i] == '(' && (!*stack || (*stack)->type != CTX_ARITHMETIC)) {
						stack_push(stack, CTX_SUBSHELL);
						command_start = true; i += 1; continue;
					}	// Open Backtick 							`
					else if ((*input)[i] == '`' && !is_close_backtick(*stack)) {
						stack_push(stack, CTX_BACKTICK);
						command_start = true; i += 1; continue;
					}	// Separator								;	&	&&	|	||	\n
					else if (is_separator((*input)[i], 0) && (!*stack || (*stack)->type != CTX_ARITHMETIC)) {
						command_start = true; i++; continue;
					}

						// Close Arithmetic							))
					if (!ft_strncmp(&(*input)[i], "))", 2) && *stack && (*stack)->type == CTX_ARITHMETIC) {
						stack_pop(stack);
						command_start = false; i += 2; continue;
					}	// Close Subshell or Command Substitution	)
					else if (*stack && (*stack)->type == CTX_SUBSHELL && (*input)[i] == ')') {
						stack_pop(stack);						
						command_start = false; i += 1; continue;
					}	// Close Backtick							`
					else if ((*input)[i] == '`' && is_close_backtick(*stack)) {
						while (*stack && (*stack)->type != CTX_BACKTICK) stack_pop(stack);
						stack_pop(stack);
						command_start = false; i += 1; continue;
					}
				}

				// Expandir aliases cuando corresponde
				if (command_start && (!*stack || (*stack)->type != CTX_ARITHMETIC)) {
					size_t start = i, end = i;
					
					// Encontrar fin del posible alias
					while ((*input)[end] && !is_separator((*input)[end], 1)) end++;
					char *alias_name  = ft_strndup(*input + start, end - start);
					char *alias_value = ft_strdup(alias_find_value(alias_name)); sfree(alias_name);
					char *tmp = ft_strjoin_sep("|", alias_value, "|", 0);
					
					if (alias_value && (!alias_list || !ft_strstr(alias_list, tmp))) {
						command_start = (*alias_value && ft_isspace(alias_value[ft_strlen(alias_value) - 1]));
						
						alias_list = ft_strjoin(alias_list, tmp, 3); tmp = NULL;
						expand_alias(&alias_value, stack, in_quotes, in_dquotes, escape);
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

			return (0);
		}

	#pragma endregion

#pragma endregion
