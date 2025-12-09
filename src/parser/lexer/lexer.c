/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 15:15:32 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/10 00:13:45 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "parser/lexer.h"
	#include "utils/libft.h"

#pragma endregion

#pragma region "Input"

	#pragma region "Comments"

		static void remove_comments(char *input) {
			if (!input) return;

			int quoted = 0;
			int escaped = 0;
			int right_space = 1;
			int c, i = 0;

			while ((c = input[i])) {
				if (escaped)							{ escaped = 0;	i++;	continue; }
				if (!quoted && c == '\\')				{ escaped = 1;	i++;	continue; }
				if (!quoted && (c == '\'' || c == '"'))	{ quoted = c;	i++;	continue; }
				if (!quoted && c == '#' && right_space)	{
					int start = i;
					while (input[i] != '\n' && input[i] != '\0') ++i;
					memmove(input + start, input + i, i - start);
					break;
				}
				right_space = (i == 0 || (!quoted && is_operator(c)));
				i++;
			}
		}

	#pragma endregion

	#pragma region "Free"

		void lexer_free(t_lexer *lexer) {
			free(lexer->stack.value);
			free(lexer->filename);
			free(lexer->full_input);

			while (lexer->input)
				buffer_pop(lexer);

			lexer->filename = NULL;
			lexer->full_input = NULL;
			lexer->input = NULL;
			lexer->user_buffer = NULL;
			lexer->stack.value = NULL;
		}

	#pragma endregion

	#pragma region "Full Line"

		static void full_line_append(t_lexer *lexer, const char *input) {
			if (!input) return ;

			char	*new_full = NULL;
			size_t	full_len = ft_strlen(lexer->full_input);
			size_t	new_len  = ft_strlen(input);

			if (lexer->append_inline == 1) {
				if (full_len > 1 && lexer->full_input[full_len - 2] == '\\' && lexer->full_input[full_len - 1] == '\n') {
					lexer->full_input[full_len - 2] = ' ';
					lexer->full_input[full_len - 1] = '\0';
					full_len -= 2;
				}
				if (full_len > 0 && lexer->full_input[full_len - 1] == '\\') {
					lexer->full_input[full_len - 1] = '\0';
					full_len--;
				}
				new_full = malloc(full_len + new_len + 1);
				strcpy(new_full, lexer->full_input);
				strcpy(new_full + full_len, input);
				lexer->line--;
				lexer->append_inline = 0;
			} else if (lexer->append_inline == 2) {
				new_full = malloc(full_len + new_len + 2);
				strcpy(new_full, lexer->full_input);
				new_full[full_len] = ' ';
				strcpy(new_full + full_len + 1, input);
				lexer->append_inline = 0;
			} else {
				new_full = malloc(full_len + new_len + 2);
				strcpy(new_full, lexer->full_input);
				new_full[full_len] = '\n';
				strcpy(new_full + full_len + 1, input);
			}

			free(lexer->full_input);
			lexer->full_input = new_full;

		}

	#pragma endregion

	#pragma region "Append"

		void lexer_append(t_lexer *lexer) {
			char *input = (lexer->more_input) ? lexer->more_input() : NULL;

			if (input) {
				remove_comments(input);

				if (lexer->interactive)
					full_line_append(lexer, input);

				if (lexer->line != -1) lexer->line++;
				buffer_push_user(lexer, input);
			} else {
				buffer_pop(lexer);
			}
		}

	#pragma endregion

	#pragma region "Initialize"

		void lexer_init(t_lexer *lexer, char *input, t_callback callback, int interactive, char *filename, int line) {
			lexer->stack.len = 0;
			lexer->stack.capacity = 64;
			lexer->stack.value = calloc(1, lexer->stack.capacity);

			lexer->input = NULL;
			lexer->user_buffer = NULL;
			remove_comments(input);
			buffer_push_user(lexer, input);
			lexer->filename = ft_strdup(filename);
			lexer->full_input = (interactive) ? ft_strdup(input) : NULL;
			lexer->interactive = interactive;
			lexer->append_inline = 0;
			lexer->command_position = 1;
			lexer->can_expand_alias = 1;
			lexer->right_space = 1;
			lexer->line = line;
			lexer->more_input = callback;
		}

	#pragma endregion

#pragma endregion

#pragma region "Navigation"

	#pragma region "Peek"

		char peek(t_lexer *lexer, size_t offset) {
			if (!lexer->input) return ('\0');

			t_buff	*buffer = lexer->input;
			size_t	remaining = offset;
			size_t	available;

			while (buffer) {
				available = ft_strlen(buffer->value) - buffer->position;
				if (remaining < available) return (buffer->value[buffer->position + remaining]);

				remaining -= available;
				buffer = buffer->next;
			}

			return ('\0');
		}

	#pragma endregion

	#pragma region "Advance"

		char advance(t_lexer *lexer) {
			while (lexer->input && (!lexer->input->value || lexer->input->position >= ft_strlen(lexer->input->value)))
				buffer_pop(lexer);

			if (!lexer->input) return ('\0');

			char c = lexer->input->value[lexer->input->position++];
			if (lexer->input->position >= ft_strlen(lexer->input->value))
				buffer_pop(lexer);

			return (c);
		}

	#pragma endregion

#pragma endregion

#pragma region "Get"

	t_token *token_get(t_lexer *lexer) {
		if (!stack_top(lexer)) while (isspace(peek(lexer, 0)) && peek(lexer, 0) != '\n') advance(lexer);

		if (peek(lexer, 0) == '\n') {
			t_string	string;
			int			line = (lexer->input == lexer->user_buffer) ? lexer->line : -1;
			char		*full_line = (lexer->input) ? ft_strdup(lexer->input->value) : NULL;
			string_init(&string);
			string_append(&string, advance(lexer));
			return (token_create(lexer, TOKEN_NEWLINE, string.value, line, full_line));
		}

		if (peek(lexer, 0)) {
			t_token *token = NULL;
			if ((token = variables(lexer)))		return (token);
			if ((token = expansion(lexer)))		return (token);
			// if ((token = grouping(lexer)))		return (token);
			if ((token = operator(lexer)))		return (token);
			if ((token = redirection(lexer)))	return (token);
			// if ((token = keyword(lexer)))		return (token);
			if ((token = word(lexer)))			return (token);
		}

		if (!lexer->interactive || lexer->append_inline) {
			lexer_append(lexer);
			if (lexer->input) return (token_get(lexer));
		}

		return (token_create(lexer, TOKEN_EOF, NULL, lexer->line + 1, NULL));
	}

#pragma endregion
