/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 15:15:32 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/30 21:04:17 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "hashes/alias.h"
	#include "parser/lexer.h"
	#include "main/options.h"
	#include "utils/libft.h"

	#include <stdio.h>

#pragma endregion

#pragma region "Stack"

	// Stack contains the token context, with the following meanings:
	//
	// "	""					double quoted
	// '	''					single quoted
	// t	$""					translatable string
	// q	$''					ANSI-C quoting
	// `	``					command substitution
	// s	$(...)				command substitution
	// a	$((...))			arithmetic substitution
	// p	${...}				parameter expression
	// S	(...)				subshell
	// A	((...)	)			arithmetic
	// B	{ ...; } or {...}	braces (command group or brace expansion)

	#pragma region "Initialize"

		static void stack_init(t_lexer *lexer) {
			lexer->stack_capacity = 64;
			lexer->stack_size = 0;
			lexer->stack = calloc(1, lexer->stack_capacity);
		}

	#pragma endregion

	#pragma region "Push"

		void stack_push(t_lexer *lexer, char delim) {
			if (lexer->stack_size >= lexer->stack_capacity) {
				lexer->stack_capacity *= 2;
				lexer->stack = realloc(lexer->stack, lexer->stack_capacity);
			}
			lexer->stack[lexer->stack_size++] = delim;
		}

	#pragma endregion

	#pragma region "Pop"

		char stack_pop(t_lexer *lexer) {
			if (!lexer->stack_size) return ('\0');
			lexer->stack[lexer->stack_size] = '\0';
			lexer->stack_size--;
			return (lexer->stack[lexer->stack_size]);
		}

	#pragma endregion

	#pragma region "Top"

		char stack_top(t_lexer *lexer) {
			if (!lexer->stack_size) return ('\0');
			return (lexer->stack[lexer->stack_size - 1]);
		}

	#pragma endregion

#pragma endregion

#pragma region "Buffer"

	#pragma region "Push"

		void buffer_push(t_lexer *lexer, char *value, char *alias_name) {
			t_buff *new_buffer;

			new_buffer = malloc(sizeof(t_buff));
			new_buffer->value = ft_strdup(value);
			new_buffer->position = 0;
			new_buffer->alias_name = ft_strdup(alias_name);
			new_buffer->is_user_input = 0;
			new_buffer->next = lexer->input;

			lexer->input = new_buffer;
		}

	#pragma endregion

	#pragma region "Push User"

		void buffer_push_user(t_lexer *lexer, char *value) {
			t_buff *new_buffer;

			new_buffer = malloc(sizeof(t_buff));
			new_buffer->value = ft_strdup(value);
			new_buffer->position = 0;
			new_buffer->alias_name = NULL;
			new_buffer->is_user_input = 1;
			new_buffer->next = NULL;

			if (!lexer->input) {
				lexer->input = new_buffer;
				lexer->user_buffer = new_buffer;
			} else {
				lexer->user_buffer->next = new_buffer;
				lexer->user_buffer = new_buffer;
			}
		}

	#pragma endregion

	#pragma region "Pop"

		void buffer_pop(t_lexer *lexer) {
			if (!lexer->input) return;

			t_buff *old = lexer->input;
			lexer->input = lexer->input->next;

			free(old->value);
			free(old->alias_name);
			free(old);
		}

	#pragma endregion

	#pragma region "Should Expand Alias"

		int should_expand_alias(t_lexer *lexer, char *alias_name) {
			if (!alias_name || !lexer->can_expand_alias || !options.expand_aliases) return (0);

			t_buff *buffer = lexer->input;
			while (buffer) {
				if (buffer->alias_name && !strcmp(buffer->alias_name, alias_name)) return (0);
				buffer = buffer->next;
			}

			if (!alias_find_value(alias_name)) return (0);
			
			return (1);
		}

	#pragma endregion

#pragma endregion

#pragma region "String"

	#pragma region "Initialize"

		void string_init(t_string *string) {
			string->capacity = 32;
			string->value = malloc(string->capacity);
			string->value[0] = '\0';
			string->len = 0;
		}

	#pragma endregion

	#pragma region "Append"

		void string_append(t_string *string, char c) {
			if (string->len + 1 >= string->capacity) {
				string->capacity *= 2;
				string->value = realloc(string->value, string->capacity);
			}
			string->value[string->len++] = c;
			string->value[string->len] = '\0';
		}

	#pragma endregion

#pragma endregion

#pragma region "Input"

	#pragma region "Append"

		void lexer_append(t_lexer *lexer) {
			char *input = lexer->more_input();

			if (input) {

				if (lexer->interactive) {
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

				lexer->line++;
				buffer_push_user(lexer, input);
				free(input);
			}
		}

	#pragma endregion

	#pragma region "Initialize"

		void lexer_init(t_lexer *lexer, char *input, t_callback callback, int interactive, char *filename, int line) {
			stack_init(lexer);
			lexer->input = NULL;
			lexer->user_buffer = NULL;
			buffer_push_user(lexer, input);
			lexer->filename = ft_strdup(filename);
			// lexer->full_input = NULL;
			// (interactive) ?	lexer->full_input = input : free(input);
			lexer->full_input = input;	// borrar
			lexer->interactive = interactive;
			lexer->append_inline = 0;
			lexer->command_position = 1;
			lexer->can_expand_alias = 1;
			lexer->line = line;
			lexer->more_input = callback;
		}

	#pragma endregion

	#pragma region "Free"

		void lexer_free(t_lexer *lexer) {
			free(lexer->stack);
			free(lexer->filename);
			free(lexer->full_input);
			while (lexer->input) buffer_pop(lexer);
			lexer->filename = NULL;
			lexer->full_input = NULL;
			lexer->input = NULL;
			lexer->user_buffer = NULL;
			lexer->stack = NULL;
		}

	#pragma endregion

#pragma endregion

#pragma region "Navigation"

	#pragma region "Peek"

		char peek(t_lexer *lexer, size_t offset) {
			if (!lexer->input) return ('\0');

			t_buff	*buffer = lexer->input;
			size_t		remaining = offset;
			size_t		available;

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
			while (lexer->input && (!lexer->input->value || lexer->input->position >= ft_strlen(lexer->input->value))) buffer_pop(lexer);
			if (!lexer->input) return ('\0');

			char c = lexer->input->value[lexer->input->position];
			lexer->input->position++;
			if (lexer->input->position >= ft_strlen(lexer->input->value))	buffer_pop(lexer);

			return (c);
		}

	#pragma endregion

#pragma endregion

#pragma region "Token"

	#pragma region "Free"

		void token_free(t_token *token) {
			if (token) {
				free(token->value);
				free(token->filename);
				free(token->full_line);
				free(token);
			}
		}

	#pragma endregion

	#pragma region "Create"

		t_token *token_create(t_lexer *lexer, t_token_type type, char *value, int line, char *full_line) {
			t_token *token = malloc(sizeof(t_token));

			token->type = type;
			token->value = value;
			token->right_space = isspace(peek(lexer, 0)) || peek(lexer, 0) == '\0';
			token->filename = ft_strdup(lexer->filename);
			token->line = line;
			token->full_line = full_line;

			return (token);
		}
	#pragma endregion

	#pragma region "Next"

		t_token *token_next(t_lexer *lexer) {

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
				// if ((token = expansion(lexer)))		return (token);
				if ((token = grouping(lexer)))		return (token);
				if ((token = operator(lexer)))		return (token);
				if ((token = redirection(lexer)))	return (token);
				// if ((token = keyword(lexer)))		return (token);
				if ((token = word(lexer)))			return (token);
			}

			if (!lexer->interactive || lexer->append_inline) {
				lexer_append(lexer);
				if (lexer->input) return (token_next(lexer));
			}
			return (token_create(lexer, TOKEN_EOF, NULL, -1, NULL));
		}
	#pragma endregion

#pragma endregion
