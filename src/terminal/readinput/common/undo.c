/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   undo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 10:10:10 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/21 14:02:00 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "utils/libft.h"
	#include "terminal/readinput/termcaps.h"
	#include "terminal/readinput/readinput.h"

#pragma endregion

#pragma region "Variables"

	typedef struct s_undo {
		char			*value;
		size_t			size, position, length;
		struct s_undo	*next;
	} t_undo;

	static t_undo	*stack;
	static bool		pushed;

#pragma endregion

#pragma region "Push"

	void undo_push(bool push) {
		if (push && !pushed) return;
		if (stack && !strcmp(stack->value, buffer.value) && stack->length == buffer.length && stack->size == buffer.size) return;
		t_undo *new = malloc(sizeof(t_undo));
		new->size = buffer.size;
		new->length = buffer.length;
		new->position = buffer.position;
		new->value = malloc(buffer.size);
		memcpy(new->value, buffer.value, buffer.size);
		new->next = stack;
		stack = new;
		pushed = push;
	}

#pragma endregion

#pragma region "Undo"

	void undo_pop() {
		if (!stack) { beep(); return; }
		t_undo *top = stack;

		free(buffer.value);
		buffer.size = top->size;
		buffer.length = top->length;
		buffer.position = top->position;
		buffer.value = malloc(top->size);
		memcpy(buffer.value, top->value, top->size);
		stack = top->next;
		free(top->value);
		free(top);
		pushed = false;
	}

#pragma endregion

#pragma region "Undo All"

	void undo_all() {
		while (stack && stack->next) {
			t_undo *tmp = stack;
			stack = stack->next;
			free(tmp->value);
			free(tmp);
		} undo_pop();
	}

#pragma endregion

#pragma region "Clear"

	void undo_clear() {
		while (stack) {
			t_undo *tmp = stack;
			stack = stack->next;
			free(tmp->value);
			free(tmp);
		}
	}

#pragma endregion
