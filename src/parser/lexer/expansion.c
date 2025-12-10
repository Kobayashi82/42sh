/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:30:41 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/10 16:04:58 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "parser/lexer.h"
	#include "utils/libft.h"

#pragma endregion

#pragma region "Context"

	#pragma region "Backticks"

		static int backticks(t_lexer *lexer, t_segment *segment) {
			// `
			if (peek(lexer, 0) == '`') {
				if (stack_top(lexer) == '`') {
					stack_pop(lexer);
					segment_append(segment, advance(lexer));
					if (!lexer->stack.len) return (2);
					return (1);
				} else {
					stack_push(lexer, '`');
					segment_append(segment, advance(lexer));
					return (1);
				}
			}

			return (0);
		}

	#pragma endregion

	#pragma region "Arithmetic"

		static int arithmetic(t_lexer *lexer, t_segment *segment) {
			// ((
			if (peek(lexer, 0) == '(' && peek(lexer, 1) == '(') {
				stack_push(lexer, 'A');
				segment_append(segment, advance(lexer));
				segment_append(segment, advance(lexer));
				return (1);
			}
			// ))
			if (peek(lexer, 0) == ')' && peek(lexer, 1) == ')') {
				if (stack_top(lexer) == 'a' || stack_top(lexer) == 'A') {
					stack_pop(lexer);
					segment_append(segment, advance(lexer));
					segment_append(segment, advance(lexer));
					if (!lexer->stack.len) return (2);
					return (1);
				} else if (stack_top(lexer) != 'a' && stack_top(lexer) != 'A') return (2);
			}

			return (0);
		}

	#pragma endregion

	#pragma region "Arithmetic Expansion"

		static int arithmetic_expansion(t_lexer *lexer, t_segment *segment) {
			// $((
			if (peek(lexer, 0) == '$' && peek(lexer, 1) == '(' && peek(lexer, 2) == '(') {
				stack_push(lexer, 'a');
				segment_append(segment, advance(lexer));
				segment_append(segment, advance(lexer));
				segment_append(segment, advance(lexer));
				return (1);
			}

			return (0);
		}

	#pragma endregion

	#pragma region "Subshell"

		static int subshell(t_lexer *lexer, t_segment *segment, int *type) {
			// (
			if (peek(lexer, 0) == '(') {
				stack_push(lexer, 'S');
				segment_append(segment, advance(lexer));
				return (1);
			}
			// )
			if (peek(lexer, 0) == ')') {
				if (stack_top(lexer) == 's' || stack_top(lexer) == 'S' || stack_top(lexer) == 'i' || stack_top(lexer) == 'o') {
					stack_pop(lexer);
					segment_append(segment, advance(lexer));
					if (!lexer->stack.len) return (2);
					return (1);
				} else if (stack_top(lexer) == 'a' || stack_top(lexer) == 'A') {
					if (stack_top(lexer) == 'A') lexer->stack.value[lexer->stack.len - 1] = 'S';
					if (stack_top(lexer) == 'a') lexer->stack.value[lexer->stack.len - 1] = 's';
					if (lexer->stack.len == 1) {
						if (stack_top(lexer) == 'S') *type = TOKEN_SUBSHELL;
						if (stack_top(lexer) == 's') *type = TOKEN_WORD;
					}
					segment_append(segment, advance(lexer));
					return (1);
				} else if (stack_top(lexer) != 's' && stack_top(lexer) != 'S' && stack_top(lexer) != 'i' && stack_top(lexer) != 'o') return (2);
			}

			return (0);
		}

	#pragma endregion

	#pragma region "Command Substitution"

		static int command_substitution(t_lexer *lexer, t_segment *segment) {
			// (
			if (peek(lexer, 0) == '$' && peek(lexer, 1) == '(') {
				stack_push(lexer, 's');
				segment_append(segment, advance(lexer));
				segment_append(segment, advance(lexer));
				return (1);
			}

			return (0);
		}

	#pragma endregion

	#pragma region "Process Substitution"

		static int process_substitution(t_lexer *lexer, t_segment *segment) {
			// <(
			if (peek(lexer, 0) == '<' && peek(lexer, 1) == '(') {
				stack_push(lexer, 'i');
				segment_append(segment, advance(lexer));
				segment_append(segment, advance(lexer));
				return (1);
			}
			// >(
			if (peek(lexer, 0) == '>' && peek(lexer, 1) == '(') {
				stack_push(lexer, 'o');
				segment_append(segment, advance(lexer));
				segment_append(segment, advance(lexer));
				return (1);
			}

			return (0);
		}

	#pragma endregion

	#pragma region "Command Group"

		static int command_group(t_lexer *lexer, t_segment *segment, int *type, int *group_can_end) {
			// {
			if (peek(lexer, 0) == '{' && (isspace(peek(lexer, 1)) || peek(lexer, 1) == '\0')) {
				stack_push(lexer, 'G');
				segment_append(segment, advance(lexer));
				if (peek(lexer, 0)) segment_append(segment, advance(lexer));
				return (1);
			}
			// }
			if (peek(lexer, 0) == '}') {
				if (stack_top(lexer) == 'p') {
					stack_pop(lexer);
					segment_append(segment, advance(lexer));
					if (!lexer->stack.len) return (2);
					return (1);
				} else if (stack_top(lexer) == 'G' && *group_can_end) {
					stack_pop(lexer);
					segment_append(segment, advance(lexer));
					if (!lexer->stack.len) return (2);
					return (1);
				} else if (stack_top(lexer) == 'G') {
					stack_pop(lexer);
					segment_append(segment, advance(lexer));
					if (!lexer->stack.len) {
						*type = TOKEN_WORD;
						return (2);
					}
				}
			}
			if (stack_top(lexer) == 'G' && (peek(lexer, 0) == ';' || peek(lexer, 0) == '\n')) {
				*group_can_end = 1;
			} else if (stack_top(lexer) == 'G' && !(*group_can_end)) {
				*group_can_end = 0;
			}

			return (0);
		}

	#pragma endregion

	#pragma region "Parameter Expansion"

		static int parameter_expansion(t_lexer *lexer, t_segment *segment) {
			// ${
			if (peek(lexer, 0) == '$' && peek(lexer, 1) == '{') {
				stack_push(lexer, 'p');
				segment_append(segment, advance(lexer));
				segment_append(segment, advance(lexer));
				return (1);
			}

			return (0);
		}

	#pragma endregion

	#pragma region "Conditional Expression"

		static int conditional_expression(t_lexer *lexer, t_segment *segment) {
			// [[
			if (peek(lexer, 0) == '[' && peek(lexer, 1) == '[') {
				stack_push(lexer, 'C');
				segment_append(segment, advance(lexer));
				segment_append(segment, advance(lexer));
				return (1);
			}
			// ]]
			if (peek(lexer, 0) == ']' && peek(lexer, 1) == ']') {
				if (stack_top(lexer) == 'C') {
					stack_pop(lexer);
					segment_append(segment, advance(lexer));
					segment_append(segment, advance(lexer));
					if (!lexer->stack.len) return (2);
					return (1);
				} else if (stack_top(lexer) != ']') return (2);
			}

			return (0);
		}

	#pragma endregion


	#pragma region "Is Expansion"

		static int is_expansion(t_lexer *lexer) {

			if (peek(lexer, 0) == '`')															return (1);
			if (peek(lexer, 0) == '(')															return (1);
			if (peek(lexer, 0) == '{' && (isspace(peek(lexer, 1)) || peek(lexer, 1) == '\0'))	return (1);
			if (peek(lexer, 0) == '$' && (peek(lexer, 1) == '(' || peek(lexer, 1) == '{'))		return (1);
			if ((peek(lexer, 0) == '<' || peek(lexer, 0) == '>') && peek(lexer, 1) == '(')		return (1);
			if (peek(lexer, 0) == '[' && peek(lexer, 1) == '[')									return (1);

			return (0);
		}

	#pragma endregion

	#pragma region "Start Context"

		static void start_context(t_lexer *lexer, t_segment *segment, int *type) {
			if (peek(lexer, 0) == '`') {
				segment_append(segment, advance(lexer));
				stack_push(lexer, '`');
				*type = TOKEN_WORD;
			} else if (peek(lexer, 0) == '<' && peek(lexer, 1) == '(') {
				segment_append(segment, advance(lexer));
				segment_append(segment, advance(lexer));
				stack_push(lexer, 'i');
				*type = TOKEN_WORD;
			} else if (peek(lexer, 0) == '>' && peek(lexer, 1) == '(') {
				segment_append(segment, advance(lexer));
				segment_append(segment, advance(lexer));
				stack_push(lexer, 'o');
				*type = TOKEN_WORD;
			} else if (peek(lexer, 0) == '(' && peek(lexer, 1) == '(') {
				segment_append(segment, advance(lexer));
				segment_append(segment, advance(lexer));
				stack_push(lexer, 'A');
				*type = TOKEN_ARITH;
			} else if (peek(lexer, 0) == '(') {
				segment_append(segment, advance(lexer));
				stack_push(lexer, 'S');
				*type = TOKEN_SUBSHELL;
			} else if (peek(lexer, 0) == '[' && peek(lexer, 1) == '[') {
				segment_append(segment, advance(lexer));
				segment_append(segment, advance(lexer));
				stack_push(lexer, 'C');
				*type = TOKEN_WORD;
			} else if (peek(lexer, 0) == '{' && (isspace(peek(lexer, 1)) || peek(lexer, 1) == '\0')) {
				segment_append(segment, advance(lexer));
				stack_push(lexer, 'G');
				*type = TOKEN_WORD;
			} else if (peek(lexer, 0) == '$') {
				segment_append(segment, advance(lexer));

				if (peek(lexer, 0) == '(' && peek(lexer, 1) == '(') {
					segment_append(segment, advance(lexer));
					segment_append(segment, advance(lexer));
					stack_push(lexer, 'a');
					*type = TOKEN_WORD;
				} else if (peek(lexer, 0) == '(') {
					segment_append(segment, advance(lexer));
					stack_push(lexer, 's');
					*type = TOKEN_WORD;
				} else if (peek(lexer, 0) == '{') {
					segment_append(segment, advance(lexer));
					stack_push(lexer, 'p');
					*type = TOKEN_WORD;
				}
			}
		}

	#pragma endregion

	#pragma region "Continuation"

		static int continuation(t_lexer *lexer, t_segment *segment, int *group_can_end) {
			if (peek(lexer, 0) == '\\' && peek(lexer, 1) == '\n' && peek(lexer, 2) == '\0') {
				advance(lexer);
				advance(lexer);
				lexer->append_inline = 1;
				lexer_append(lexer);
				return (1);
			} else if (peek(lexer, 0) == '\\' && peek(lexer, 1) == '\0') {
				advance(lexer);
				lexer->append_inline = 1;
				lexer_append(lexer);
				return (1);
			} else if (peek(lexer, 0) == '\\') {
				segment_append(segment, advance(lexer));
				segment_append(segment, advance(lexer));
				return (1);
			}

			if (peek(lexer, 0) == '\0') {
				if (stack_top(lexer) == 'G') *group_can_end = 1;
				segment_append(segment, '\n');
				lexer_append(lexer);
				return (1);
			}

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Expansion"

	// `...`			backticks
	// ((...))			arithmetic
	// $((...))			arithmetic substitution
	// (...)			subshell
	// $(...)			command substitution
	// <(...), >(...)	process substitution
	// { ... ; }		command group
	// ${...}			parameter expansion
	// [[...]]			conditional expression

	t_token *expansion(t_lexer *lexer) {
		int			line = (lexer->input == lexer->user_buffer) ? lexer->line : -1;
		char		*full_line = (lexer->input) ? ft_strdup(lexer->input->value) : NULL;
		int			type, group_can_end = 0;

		if (!is_expansion(lexer)) return (free(full_line), NULL);

		t_segment *segment = segment_new(NULL);
		start_context(lexer, segment, &type);

		while (peek(lexer, 0) || (lexer->input && stack_top(lexer))) {

			if (peek(lexer, 0) == '\'' || peek(lexer, 0) == '"')					{ handle_quotes(lexer, segment); continue; }
			if (continuation(lexer, segment, &group_can_end))						  continue;

			int result = 0;
			if ((result = backticks(lexer, segment)))								{ if (result == 1) continue; if (result == 2) break; }
			if ((result = arithmetic(lexer, segment)))								{ if (result == 1) continue; if (result == 2) break; }
			if ((result = arithmetic_expansion(lexer, segment)))					{ if (result == 1) continue; if (result == 2) break; }
			if ((result = subshell(lexer, segment, &type)))							{ if (result == 1) continue; if (result == 2) break; }
			if ((result = command_substitution(lexer, segment)))					{ if (result == 1) continue; if (result == 2) break; }
			if ((result = process_substitution(lexer, segment)))					{ if (result == 1) continue; if (result == 2) break; }
			if ((result = command_group(lexer, segment, &type, &group_can_end)))	{ if (result == 1) continue; if (result == 2) break; }
			if ((result = parameter_expansion(lexer, segment)))						{ if (result == 1) continue; if (result == 2) break; }
			if ((result = conditional_expression(lexer, segment)))					{ if (result == 1) continue; if (result == 2) break; }

			segment_append(segment,advance(lexer));
		}

		if (stack_top(lexer)) return (token_create(lexer, TOKEN_WORD, segment, line, full_line));

		return (token_create(lexer, type, segment, line, full_line));
	}

#pragma endregion

#pragma region "Variables"

	// $"..."	translatable string
	// $'...'	ANSI-C quoting
	// $VAR		variable

	t_token *variables(t_lexer *lexer) {
		int			line = (lexer->input == lexer->user_buffer) ? lexer->line : -1;
		char		*full_line = (lexer->input) ? ft_strdup(lexer->input->value) : NULL;

		t_segment *segment = segment_new(NULL);

 		if (peek(lexer, 0) == '$' && (peek(lexer, 1) == '\'' || peek(lexer, 1) == '"')) {
			segment_append(segment,advance(lexer));
			handle_quotes(lexer, segment);
			return (token_create(lexer, TOKEN_WORD, segment, line, full_line));
		}

		if (peek(lexer, 0) == '$' && (peek(lexer, 1) == '$' || (!is_operator(peek(lexer, 1)) && peek(lexer, 1) != '}'))) {
			segment_append(segment,advance(lexer));

			if (peek(lexer, 0) == '$') {
				segment_append(segment,advance(lexer));
				return (token_create(lexer, TOKEN_WORD, segment, line, full_line));
			}
			if (peek(lexer, 0) == '#') {
				segment_append(segment,advance(lexer));
				return (token_create(lexer, TOKEN_WORD, segment, line, full_line));
			}
			if (peek(lexer, 0) == '@') {
				segment_append(segment,advance(lexer));
				return (token_create(lexer, TOKEN_WORD, segment, line, full_line));
			}
			if (peek(lexer, 0) == '*') {
				segment_append(segment,advance(lexer));
				return (token_create(lexer, TOKEN_WORD, segment, line, full_line));
			}
			if (peek(lexer, 0) == '!') {
				segment_append(segment,advance(lexer));
				return (token_create(lexer, TOKEN_WORD, segment, line, full_line));
			}
			if (peek(lexer, 0) == '?') {
				segment_append(segment,advance(lexer));
				return (token_create(lexer, TOKEN_WORD, segment, line, full_line));
			}
			if (peek(lexer, 0) == '-') {
				segment_append(segment,advance(lexer));
				return (token_create(lexer, TOKEN_WORD, segment, line, full_line));
			}
			if (isdigit(peek(lexer, 0))) {
				while (isdigit(peek(lexer, 0))) segment_append(segment,advance(lexer));
				return (token_create(lexer, TOKEN_WORD, segment, line, full_line));
			}

			while (!is_operator(peek(lexer, 0)) && peek(lexer, 0) != '\'' && peek(lexer, 0) != '"' && peek(lexer, 0) != '}') {
				if (peek(lexer, 0) == '\\' && peek(lexer, 1) == '\n' && peek(lexer, 2) == '\0') {
					advance(lexer);
					advance(lexer);
					lexer->append_inline = 1;
					lexer_append(lexer);
					continue;
				} else if (peek(lexer, 0) == '\\' && peek(lexer, 1) == '\0') {
					advance(lexer);
					lexer->append_inline = 1;
					lexer_append(lexer);
					continue;
				} else if (peek(lexer, 0) == '\\') break;

				segment_append(segment,advance(lexer));
			}

			return (token_create(lexer, TOKEN_WORD, segment, line, full_line));
		}

		free(full_line);
		segment_free(segment);
		return (NULL);
	}

#pragma endregion
