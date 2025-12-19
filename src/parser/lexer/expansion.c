/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:30:41 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/19 12:59:33 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "parser/lexer.h"
	#include "utils/libft.h"

#pragma endregion

#pragma region "Context"

	#pragma region "Backticks"

		int backticks(t_lexer *lexer, t_segment *segment) {
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

	#pragma region "Arithmetic Expansion"

		int arithmetic_expansion(t_lexer *lexer, t_segment *segment) {
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

	#pragma region "Command Substitution"

		int command_substitution(t_lexer *lexer, t_segment *segment) {
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

	#pragma region "Parameter Expansion"

		int parameter_expansion(t_lexer *lexer, t_segment *segment) {
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


	#pragma region "Is Expansion"

		static int is_expansion(t_lexer *lexer) {
			if (peek(lexer, 0) == '`')															return (1);
			if (peek(lexer, 0) == '$' && (peek(lexer, 1) == '(' || peek(lexer, 1) == '{'))		return (1);

			return (0);
		}

	#pragma endregion

	#pragma region "Start Context"

		static void start_context(t_lexer *lexer, t_segment *segment, int *type) {
			if (peek(lexer, 0) == '`') {
				segment_append(segment, advance(lexer));
				stack_push(lexer, '`');
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
				if (!lexer->input) return (2);
				return (1);
			} else if (peek(lexer, 0) == '\\' && peek(lexer, 1) == '\0') {
				advance(lexer);
				lexer->append_inline = 1;
				lexer_append(lexer);
				if (!lexer->input) return (2);
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
				if (!lexer->input) return (2);
				return (1);
			}

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Expansion"

	// `...`			backticks
	// $((...))			arithmetic substitution
	// $(...)			command substitution
	// ${...}			parameter expansion

	t_token *expansion(t_lexer *lexer) {
		int			line = (lexer->input == lexer->user_buffer) ? lexer->line : -1;
		char		*full_line = (lexer->input) ? ft_strdup(lexer->input->value) : NULL;
		int			type, group_can_end = 0;

		if (!is_expansion(lexer)) return (free(full_line), NULL);

		t_segment *segment = segment_new(NULL);
		start_context(lexer, segment, &type);

		while (peek(lexer, 0) || stack_top(lexer)) {
			int result = 0;
			if (peek(lexer, 0) == '\'' || peek(lexer, 0) == '"')					{ handle_quotes(lexer, segment, 1); continue; }

			if ((result = continuation(lexer, segment, &group_can_end)))			{ if (result == 1) continue; if (result == 2) break; }
			if ((result = backticks(lexer, segment)))								{ if (result == 1) continue; if (result == 2) break; }
			if ((result = arithmetic(lexer, segment)))								{ if (result == 1) continue; if (result == 2) break; }
			if ((result = arithmetic_expansion(lexer, segment)))					{ if (result == 1) continue; if (result == 2) break; }
			if ((result = subshell(lexer, segment, &type)))							{ if (result == 1) continue; if (result == 2) break; }
			if ((result = command_substitution(lexer, segment)))					{ if (result == 1) continue; if (result == 2) break; }
			if ((result = process_substitution(lexer, segment)))					{ if (result == 1) continue; if (result == 2) break; }
			if ((result = command_group(lexer, segment, &type, &group_can_end)))	{ if (result == 1) continue; if (result == 2) break; }
			if ((result = parameter_expansion(lexer, segment)))						{ if (result == 1) continue; if (result == 2) break; }
			if ((result = conditional_expression(lexer, segment)))					{ if (result == 1) continue; if (result == 2) break; }

			segment_append(segment, advance(lexer));
		}

		if (stack_top(lexer)) return (token_create(lexer, TOKEN_WORD, segment, line, full_line));

		return (token_create(lexer, type, segment, line, full_line));
	}

#pragma endregion

#pragma region "Variable"

	// $"..."	translatable string
	// $'...'	ANSI-C quoting
	// $VAR		variable

	t_token *variable(t_lexer *lexer) {
		int			line = (lexer->input == lexer->user_buffer) ? lexer->line : -1;
		char		*full_line = (lexer->input) ? ft_strdup(lexer->input->value) : NULL;

		t_segment *segment = segment_new(NULL);

		if (peek(lexer, 0) == '$' && (peek(lexer, 1) == '\'' || peek(lexer, 1) == '"')) {
			int type = (peek(lexer, 1) == '\'') ? TOKEN_ANSI_QUOTE : TOKEN_LOCATE_QUOTE;
			segment_append(segment, advance(lexer));
			handle_quotes(lexer, segment, 1);
			return (token_create(lexer, type, segment, line, full_line));
		}

		if (peek(lexer, 0) == '$' && (peek(lexer, 1) == '$' || (!is_operator(peek(lexer, 1)) && peek(lexer, 1) != '}'))) {
			segment_append(segment, advance(lexer));

			if (peek(lexer, 0) == '$') {
				segment_append(segment, advance(lexer));
				return (token_create(lexer, TOKEN_VAR, segment, line, full_line));
			}
			if (peek(lexer, 0) == '#') {
				segment_append(segment, advance(lexer));
				return (token_create(lexer, TOKEN_VAR, segment, line, full_line));
			}
			if (peek(lexer, 0) == '@') {
				segment_append(segment, advance(lexer));
				return (token_create(lexer, TOKEN_VAR, segment, line, full_line));
			}
			if (peek(lexer, 0) == '*') {
				segment_append(segment, advance(lexer));
				return (token_create(lexer, TOKEN_VAR, segment, line, full_line));
			}
			if (peek(lexer, 0) == '!') {
				segment_append(segment, advance(lexer));
				return (token_create(lexer, TOKEN_VAR, segment, line, full_line));
			}
			if (peek(lexer, 0) == '?') {
				segment_append(segment, advance(lexer));
				return (token_create(lexer, TOKEN_VAR, segment, line, full_line));
			}
			if (peek(lexer, 0) == '-') {
				segment_append(segment, advance(lexer));
				return (token_create(lexer, TOKEN_VAR, segment, line, full_line));
			}
			if (isdigit(peek(lexer, 0))) {
				while (isdigit(peek(lexer, 0))) segment_append(segment, advance(lexer));
				return (token_create(lexer, TOKEN_VAR, segment, line, full_line));
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

				segment_append(segment, advance(lexer));
			}

			return (token_create(lexer, TOKEN_VAR, segment, line, full_line));
		}

		free(full_line);
		segment_free(segment);
		return (NULL);
	}

#pragma endregion
