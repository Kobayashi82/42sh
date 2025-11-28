/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:26:16 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/28 21:56:54 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "utils/libft.h"
	#include "parser/syntax.h"

#pragma endregion

#pragma region "Stack"

	#pragma region "Push"

		t_stack *ctx_stack_push(t_stack **top, t_type type) {
			t_stack *new_node = malloc(sizeof(t_stack));
			
			new_node->type = type;
			new_node->value[0] = '\0';
			new_node->nvalue = 0;
			new_node->prev = *top;
			*top = new_node;

			return (new_node);
		}

	#pragma endregion

	#pragma region "Pop"

		void ctx_stack_pop(t_stack **top) {
			if (*top) {
				t_stack *temp = *top;
				*top = (*top)->prev;
				free(temp);
			}
		}

	#pragma endregion

	#pragma region "Clear"

		void ctx_stack_clear(t_stack **stack) {
			while (*stack) ctx_stack_pop(stack);
		}

	#pragma endregion


#pragma endregion

#pragma region "Context"

	#pragma region "Is Context"

		int ctx_is_context(t_stack *stack, t_type type) {
			while (stack) {
				if (stack->type == type) return (1);
				stack = stack->prev;
			}

			return (0);
		}

	#pragma endregion

#pragma endregion
