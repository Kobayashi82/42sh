/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   container.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 17:57:18 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/10 00:07:08 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "parser/lexer.h"
	#include "utils/libft.h"

#pragma endregion

#pragma region "Stack"

	// Stack contains the token context, with the following meanings:
	//
	// "	""			double quoted
	// '	''			single quoted
	// `	``			command substitution
	// s	$(...)		command substitution
	// S	(...)		subshell
	// a	$((...))	arithmetic substitution
	// A	((...)	)	arithmetic
	// p	${...}		parameter expression
	// G	{ ...; }	braces (command group or brace expansion)
	// i	<(...)		process substitution in
	// o	>(...)		process substitution out
	// C	[[...]]		conditional expression

	#pragma region "Get"

		char *stack_get(t_lexer *lexer) {
			if (!lexer->stack.len) return ("");

			switch (lexer->stack.value[lexer->stack.len - 1]) {
				case 'A':	return ("))");
			}

			return ("");
		}

	#pragma endregion

	#pragma region "Top"

		char stack_top(t_lexer *lexer) {
			if (!lexer->stack.len) return ('\0');

			return (lexer->stack.value[lexer->stack.len - 1]);
		}

	#pragma endregion

	#pragma region "Pop"

		char stack_pop(t_lexer *lexer) {
			if (!lexer->stack.len) return ('\0');

			lexer->stack.value[lexer->stack.len--] = '\0';
			return (lexer->stack.value[lexer->stack.len]);
		}

	#pragma endregion

	#pragma region "Push"

		void stack_push(t_lexer *lexer, char c) {
			if (lexer->stack.len >= lexer->stack.capacity) {
				lexer->stack.capacity *= 2;
				lexer->stack.value = realloc(lexer->stack.value, lexer->stack.capacity);
			}

			lexer->stack.value[lexer->stack.len++] = c;
			lexer->stack.value[lexer->stack.len] = '\0';
		}

	#pragma endregion

#pragma endregion

#pragma region "String"

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

	#pragma region "Initialize"

		void string_init(t_string *string) {
			string->len = 0;
			string->capacity = 32;
			string->value = malloc(string->capacity);
			string->value[0] = '\0';
		}

	#pragma endregion

#pragma endregion

#pragma region "Buffer"

	#pragma region "Pop"

		void buffer_pop(t_lexer *lexer) {
			if (!lexer->input) return;

			t_buff *old = lexer->input;
			lexer->input = lexer->input->next;

			if (!lexer->input)
				lexer->user_buffer = NULL;
			else if (lexer->input->is_user_input)
				lexer->user_buffer = lexer->input;

			free(old->value);
			free(old->alias_name);
			free(old);
		}

	#pragma endregion

	#pragma region "Push"

		void buffer_push(t_lexer *lexer, char *value, char *alias_name) {
			t_buff *new_buffer = malloc(sizeof(t_buff));
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
			t_buff *new_buffer = malloc(sizeof(t_buff));
			new_buffer->value = value;
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

#pragma endregion
