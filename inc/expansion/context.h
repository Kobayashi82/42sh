/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:04:02 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/28 23:25:07 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Variables"

	#pragma region "Enumerators"

		typedef enum e_type {
			CTX_QUOTE,
			CTX_DQUOTE,
			CTX_ARITHMETIC,
			CTX_ARITHMETIC_EXPAND,
			CTX_ARITHMETIC_GROUP,
			CTX_SUBSHELL_COMMAND,
			CTX_SUBSHELL,
			CTX_BACKTICK,
			CTX_BRACE,
			CTX_BRACE_PARAM,
			CTX_BRACE_COMMAND,
			CTX_PROCESS_SUB_IN,
			CTX_PROCESS_SUB_OUT,
		} t_type;

	#pragma endregion

	#pragma region "Structures"

		typedef struct s_stack {
			t_type			type;
			char			value[4];
			int				nvalue;
			struct s_stack	*prev;
		} t_stack;

		typedef struct s_context {
			t_stack	*stack;
			int		in_escape;
			int		in_token;
			int		error;
		} t_context;

	#pragma endregion

#pragma endregion

#pragma region Methods

	//	--------- CONTEXT ----------
	t_stack	*ctx_stack_push(t_stack **top, t_type type);
	void	ctx_stack_pop(t_stack **top);
	void	ctx_stack_clear(t_stack **stack);
	int		ctx_is_context(t_stack *stack, t_type type);

#pragma endregion
