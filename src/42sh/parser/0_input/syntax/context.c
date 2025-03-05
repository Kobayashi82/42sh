/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:26:16 by vzurera-          #+#    #+#             */
/*   Updated: 2025/03/05 14:00:19 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"
	#include "parser/syntax.h"

#pragma endregion

#pragma region "Stack"

	#pragma region "Push"

		t_stack *stack_push(t_stack **top, t_type type) {
			t_stack *new_node = smalloc(sizeof(t_stack));
			
			new_node->type = type;
			new_node->value[0] = '\0';
			new_node->nvalue = 0;
			new_node->prev = *top;
			*top = new_node;

			return (new_node);
		}

	#pragma endregion

	#pragma region "Pop"

		void stack_pop(t_stack **top) {
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

		t_stack *stack_copy(const t_stack *stack) {
			if (!stack) return (NULL);
			
			t_stack *current = (t_stack *) stack;
			t_stack *reversed = NULL;
			
			while (current) {
				stack_push(&reversed, current->type);
				ft_strcpy(reversed->value, current->value);
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
		
	#pragma endregion

#pragma endregion

#pragma region "Context"

	#pragma region "Is Context"

		int is_context(t_stack *stack, t_type type) {
			while (stack) {
				if (stack->type == type) return (1);
				stack = stack->prev;
			}

			return (0);
		}

	#pragma endregion

	#pragma region "Copy"

		void context_copy(t_context *dst, const t_context *src) {
			if (!dst || !src) return;
			
			stack_clear(&dst->stack);
			dst->stack = stack_copy(src->stack);
			
			dst->in_escape = src->in_escape;
			dst->in_token = src->in_token;
			dst->error = src->error;
		}

	#pragma endregion

#pragma endregion
