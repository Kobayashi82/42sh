/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_alias.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 20:58:15 by vzurera-          #+#    #+#             */
/*   Updated: 2025/02/27 23:31:32 by vzurera-         ###   ########.fr       */
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

			static t_stack *stack_push(t_stack **top, t_type type) {
				t_stack *new_node = smalloc(sizeof(t_stack));
				
				new_node->type = type;
				new_node->prev = *top;
				*top = new_node;

				return (new_node);
			}

		#pragma endregion

		#pragma region "Pop"

			static void stack_pop(t_stack **top) {
				if (*top) {
					t_stack *temp = *top;
					*top = (*top)->prev;
					sfree(temp);
				}
			}

		#pragma endregion

		#pragma region "Clear"

			void stack_clear(t_stack **stack) {
				while (*stack) stack_pop(stack);
			}

		#pragma endregion

		#pragma region "Copy"

			static t_stack *stack_copy(const t_stack *stack) {
				if (!stack) return (NULL);
				
				t_stack *current = (t_stack *) stack;
				t_stack *reversed = NULL;
				
				while (current) {
					stack_push(&reversed, current->type);
					current = current->prev;
				}
				
				t_stack *new_stack = NULL;
				while (reversed) {
					t_stack *next = reversed->prev;
					reversed->prev = new_stack;
					new_stack = reversed;
					reversed = next;
				}
				
				return (new_stack);
			}
			
			void context_copy(t_context *dst, const t_context *src) {
				if (!dst || !src) return;
			
				stack_clear(&dst->stack);
				dst->stack = stack_copy(src->stack);

				dst->in_quotes = src->in_quotes;
				dst->in_dquotes = src->in_dquotes;
				dst->in_escape = src->in_escape;
				dst->in_token = src->in_token;
				dst->error = src->error;
			}

		#pragma endregion

	#pragma endregion

	#pragma region "Is Separator"

		static bool is_separator(char c, int type) {
			if (!type)	return (c == ';' || c == '|' || c == '&' || c == '\n');
			else		return (c == '$' || c == '`' || c == '(' || c == ')' || c == '\'' || c == '"' || c == '{' || c == '}' || c == ';' || c == '&' || c == '|' || c == '\n' || ft_isspace(c));
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

		int expand_alias(char **input, t_context *alias) {
			if (!options.expand_aliases || !input || !*input || !**input) return (0);

			if (current_expansion++ == MAX_ALIAS_EXPANSIONS) {
				current_expansion = 0; return (1); }

			bool command_start = true, end_escaped = false;
			size_t i = 0;

			while ((*input)[i]) {
				alias->in_token = (!alias->in_quotes && !alias->in_dquotes && !alias->in_escape && ((ft_isspace((*input)[i]) && alias->in_token) || (*input)[i] == '&' || (*input)[i] == '|'));
				// Handle Escape
				if (alias->in_escape)							{ alias->in_escape = false;		alias->in_token = false;	i++; continue; }
				if ((*input)[i] == '\\' && !alias->in_quotes)	{ alias->in_escape = true; end_escaped = true;		alias->in_token = true;		i++; continue; }
				// Handle Quotes
				if ((*input)[i] == '\'' && !alias->in_dquotes)	{ alias->in_quotes  = !alias->in_quotes; end_escaped = false;	i++; continue; }
				if ((*input)[i] == '"'  && !alias->in_quotes)	{ alias->in_dquotes = !alias->in_dquotes; end_escaped = false;	i++; continue; }
				// Handle Spaces
				if ((*input)[i] != '\n' && ft_isspace((*input)[i])) {										i++; continue; }
				end_escaped = false;

				if (!alias->in_quotes && !alias->in_dquotes) {
						// Open Arithmetic Expansion				$((
					if (!ft_strncmp(&(*input)[i], "$((", 3) && is_arithmetic(&(*input)[i + 3])) {
						stack_push(&alias->stack, CTX_ARITHMETIC);
						command_start = false; i += 3; continue;
					}	// Arithmetic Evaluation					((
					else if (!ft_strncmp(&(*input)[i], "((", 2)  && is_arithmetic(&(*input)[i + 2])) {
						stack_push(&alias->stack, CTX_ARITHMETIC);
						command_start = false; i += 2; continue;
					}	// Open Command Substitution				$(
					else if (!ft_strncmp(&(*input)[i], "$(", 2)) {
						stack_push(&alias->stack, CTX_SUBSHELL);
						command_start = true; i += 2; continue;
					}	// Open Subshell 							(
					else if ((*input)[i] == '(' && (!alias->stack || alias->stack->type != CTX_ARITHMETIC)) {
						stack_push(&alias->stack, CTX_SUBSHELL);
						command_start = true; i += 1; continue;
					}	// Open Backtick 							`
					else if ((*input)[i] == '`' && !is_close_backtick(alias->stack)) {
						stack_push(&alias->stack, CTX_BACKTICK);
						command_start = true; i += 1; continue;
					}	// Separator								;	&	&&	|	||	\n
					else if (is_separator((*input)[i], 0) && (!alias->stack || alias->stack->type != CTX_ARITHMETIC)) {
						command_start = true; i++; continue;
					}

						// Close Arithmetic							))
					if (!ft_strncmp(&(*input)[i], "))", 2) && alias->stack && alias->stack->type == CTX_ARITHMETIC) {
						stack_pop(&alias->stack);
						command_start = false; i += 2; continue;
					}	// Close Subshell or Command Substitution	)
					else if (alias->stack && alias->stack->type == CTX_SUBSHELL && (*input)[i] == ')') {
						stack_pop(&alias->stack);						
						command_start = false; i += 1; continue;
					}	// Close Backtick							`
					else if ((*input)[i] == '`' && is_close_backtick(alias->stack)) {
						while (alias->stack && alias->stack->type != CTX_BACKTICK) stack_pop(&alias->stack);
						stack_pop(&alias->stack);
						command_start = false; i += 1; continue;
					}
				}

				// Expandir aliases cuando corresponde
				if (command_start && !alias->in_quotes && !alias->in_dquotes && (!alias->stack || alias->stack->type != CTX_ARITHMETIC)) {
					size_t start = i, end = i;
					
					// Encontrar fin del posible alias
					while ((*input)[end] && !is_separator((*input)[end], 1)) end++;
					char *alias_name  = ft_strndup(*input + start, end - start);
					char *alias_value = ft_strdup(alias_find_value(alias_name)); sfree(alias_name);
					char *tmp = ft_strjoin_sep("|", alias_value, "|", 0);
					
					if (alias_value && (!alias_list || !ft_strstr(alias_list, tmp))) {
						command_start = (*alias_value && ft_isspace(alias_value[ft_strlen(alias_value) - 1]));
						
						alias_list = ft_strjoin(alias_list, tmp, 3); tmp = NULL;
						expand_alias(&alias_value, alias);
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

			if (end_escaped) { alias->in_token = true; alias->in_escape = true; }
			return (0);
		}

	#pragma endregion

#pragma endregion
