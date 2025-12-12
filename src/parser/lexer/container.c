/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   container.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 17:57:18 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/11 13:50:26 by vzurera-         ###   ########.fr       */
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

#pragma region "Segment"

	#pragma region "Empty"

		int segment_empty(t_segment *segment) {
			while (segment) {
				if (segment->string.len || segment->quoted) return (0);
				segment = segment->next;
			}

			return (1);
		}

	#pragma endregion

	#pragma region "Set Type"

		void segment_set_type(t_segment *segment, int type) {
			if (!segment) return;

			t_segment *curr = segment;
			while (curr->next)
				curr = curr->next;

			curr->type = type;
		}

	#pragma endregion

	#pragma region "Set Quoted"

		void segment_set_quoted(t_segment *segment, char quoted) {
			if (!segment) return;

			t_segment *curr = segment;
			while (curr->next)
				curr = curr->next;

			curr->quoted = quoted;
		}

	#pragma endregion

	void segment_append_token(t_segment *segment, t_token *token, char quoted) {
		if (!segment || !token || !token->segment) return;

		t_segment *curr = segment;
		while (curr->next)
			curr = curr->next;

		if (!curr->prev && !curr->string.len && (!curr->quoted || curr->quoted == quoted)) {
			free(curr->string.value);
			curr->string.value = token->segment->string.value;
			curr->string.len = token->segment->string.len;
			curr->string.capacity = token->segment->string.capacity;
			curr->next = token->segment->next;
			curr->quoted = quoted;
			curr->type = token->type;
			free(token->segment);
		} else if (!curr->string.len && (!curr->quoted || curr->quoted == quoted)) {
			t_segment *tmp = curr;
			token->segment->prev = curr->prev;
			curr->prev->next = token->segment;
			curr = token->segment;
			curr->quoted = quoted;
			curr->type = token->type;
			segment_free(tmp);
		} else {
			curr->next = token->segment;
			curr->next->prev = curr;
			curr->next->quoted = quoted;
			curr->next->type = token->type;
		}

		token->segment = NULL;
		token_free(token);
	}

	#pragma region "Last Value"

		char *segment_last_value(t_segment *segment) {
			if (!segment) return (NULL);

			t_segment *curr = segment;
			while (curr->next)
				curr = curr->next;

			return (curr->string.value);
		}

	#pragma endregion

	#pragma region "Free"

		void segment_free(t_segment *segment) {
			while (segment) {
				t_segment *curr = segment;
				segment = segment->next;
				free(curr->string.value);
				free(curr);
			}
		}

	#pragma endregion

	#pragma region "Append"

		void segment_append(t_segment *segment, char c) {
			if (!segment) return;

			t_segment *curr = segment;
			while (curr->next)
				curr = curr->next;

			if (curr->string.len + 1 >= curr->string.capacity) {
				curr->string.capacity *= 2;
				curr->string.value = realloc(curr->string.value, curr->string.capacity);
			}

			curr->string.value[curr->string.len++] = c;
			curr->string.value[curr->string.len] = '\0';
		}

	#pragma endregion

	#pragma region "New"

		t_segment *segment_new(t_segment *segment) {
			t_segment *curr = segment;
			while (curr && curr->next)
				curr = curr->next;

			t_segment *new_segment = malloc(sizeof(t_segment));
			new_segment->prev = curr;
			new_segment->next = NULL;
			new_segment->quoted = 0;
			new_segment->type = 0;
			new_segment->string.len = 0;
			new_segment->string.capacity = 32;
			new_segment->string.value = malloc(new_segment->string.capacity);
			new_segment->string.value[0] = '\0';

			if (curr)	curr->next = new_segment;
			else		curr = new_segment;

			return (curr);
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
