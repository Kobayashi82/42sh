/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 15:15:32 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/29 16:16:35 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Include"

	#include "parser/lexer.h"
	#include "utils/libft.h"

#pragma endregion

// Cambiar lexer como global
// Cambiar substring por otro sistema, ya que un word puede formar parte de dos buffers

t_lexer lexer;

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

	static void stack_init() {
		if (lexer.stack) free(lexer.stack);
		lexer.stack_capacity = 64;
		lexer.stack_size = 0;
		lexer.stack = calloc(1, lexer.stack_capacity);
	}

	void stack_push(char delim) {
		if (lexer.stack_size >= lexer.stack_capacity) {
			lexer.stack_capacity *= 2;
			lexer.stack = realloc(lexer.stack, lexer.stack_capacity);
		}
		lexer.stack[lexer.stack_size++] = delim;
	}

	char stack_pop() {
		if (!lexer.stack_size) return ('\0');
		lexer.stack[lexer.stack_size] = '\0';
		lexer.stack_size -= 1;
		return (lexer.stack[lexer.stack_size]);
	}

	char stack_top() {
		if (!lexer.stack_size) return ('\0');
		return (lexer.stack[lexer.stack_size - 1]);
	}

#pragma endregion

#pragma region "Buffer"

	// Push alias buffer
	void buffer_push(char *value, char *alias_name) {
		t_buff *new_buffer;

		new_buffer = malloc(sizeof(t_buff));
		new_buffer->value = ft_strdup(value);
		new_buffer->position = 0;
		new_buffer->alias_name = ft_strdup(alias_name);
		new_buffer->is_user_input = 0;
		new_buffer->next = lexer.input;

		lexer.input = new_buffer;
	}

	// Push user buffer
	void buffer_push_user(char *value) {
		t_buff *new_buffer;

		new_buffer = malloc(sizeof(t_buff));
		new_buffer->value = ft_strdup(value);
		new_buffer->position = 0;
		new_buffer->alias_name = NULL;
		new_buffer->is_user_input = 1;
		new_buffer->next = NULL;

		if (!lexer.input) {
			lexer.input = new_buffer;
			lexer.base_buffer = new_buffer;
		} else {
			lexer.base_buffer->next = new_buffer;
			lexer.base_buffer = new_buffer;
		}
	}

	void buffer_pop() {
		if (!lexer.input) return;

		t_buff *old = lexer.input;
		lexer.input = lexer.input->next;

		free(old->value);
		free(old->alias_name);
		free(old);
	}

	int is_alias_expanding(char *alias_name) {
		if (!alias_name) return (0);

		t_buff *buffer = lexer.input;
		while (buffer) {
			if (buffer->alias_name && !strcmp(buffer->alias_name, alias_name)) return (1);
			buffer = buffer->next;
		}

		return (0);
	}

#pragma endregion

#pragma region "String"

	void string_init(t_string *string) {
		string->capacity = 32;
		string->value = malloc(string->capacity);
		string->value[0] = '\0';
		string->len = 0;
	}

	void string_append(t_string *string, char c) {
		if (string->len + 1 >= string->capacity) {
			string->capacity *= 2;
			string->value = realloc(string->value, string->capacity);
		}
		string->value[string->len++] = c;
		string->value[string->len] = '\0';
	}

#pragma endregion

#pragma region "Input"

	void lexer_append() {
		char *input = lexer.more_input();
		char *new_full = NULL;

		if (input) {
			size_t full_len = ft_strlen(lexer.full_input);
			size_t new_len  = ft_strlen(input);

			if (lexer.append_inline) {
				if (full_len > 0 && lexer.full_input[full_len - 1] == '\\') {
					lexer.full_input[full_len - 1] = '\0';
					full_len--;
				}
				new_full = malloc(full_len + new_len + 1);
				strcpy(new_full, lexer.full_input);
				strcpy(new_full + full_len, input);
				lexer.append_inline = 0;
			} else {
				new_full = malloc(full_len + new_len + 2);
				strcpy(new_full, lexer.full_input);
				new_full[full_len] = '\n';
				strcpy(new_full + full_len + 1, input);
			}

			free(lexer.full_input);
			lexer.full_input = new_full;

			buffer_push_user(input);
			free(input);
		}
	}

	void lexer_init(char *input, t_callback callback) {
		lexer.full_input = input;
		lexer.input = NULL;
		lexer.base_buffer = NULL;
		buffer_push_user(input);
		lexer.append_inline = 0;
		lexer.more_input = callback;
		stack_init();
	}

	void lexer_free() {
		free(lexer.stack);
		free(lexer.full_input);
		while (lexer.input) buffer_pop();
		lexer.full_input = NULL;
		lexer.input = NULL;
		lexer.base_buffer = NULL;
		lexer.stack = NULL;
	}

#pragma endregion

#pragma region "Navigation"

	char peek(size_t offset) {
		if (!lexer.input) return ('\0');

		t_buff	*buffer = lexer.input;
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

	char advance() {
		while (lexer.input && (!lexer.input->value || lexer.input->position >= ft_strlen(lexer.input->value))) buffer_pop();
		if (!lexer.input) return ('\0');

		char c = lexer.input->value[lexer.input->position];
		lexer.input->position++;
		if (lexer.input->position >= ft_strlen(lexer.input->value))	buffer_pop();

		return (c);
	}

#pragma endregion

#pragma region "Token"

	void token_free(t_token *token) {
		if (token) {
			free(token->value);
			free(token);
		}
	}

	t_token *token_create(t_token_type type, char *value) {
		t_token *token = malloc(sizeof(t_token));

		token->type = type;
		token->value = value;
		token->right_space = isspace(peek(0)) || peek(0) == '\0';

		return (token);
	}

	t_token *token_next() {

		if (!stack_top()) while (isspace(peek(0))) advance();

		if (peek(0)) {
			t_token *token = NULL;
			// if ((token = expansion()))		return (token);
			// if ((token = grouping()))		return (token);
			// if ((token = operator()))		return (token);
			// if ((token = redirection()))	return (token);
			// if ((token = keyword()))		return (token);
			if ((token = word()))			return (token);
		}

		return (token_create(TOKEN_EOF, 0));
	}

#pragma endregion
