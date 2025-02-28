/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:04:02 by vzurera-          #+#    #+#             */
/*   Updated: 2025/02/28 14:12:13 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Variables"

	#pragma region "Enumerators"

		typedef enum e_syntax_type { FULL, PARTIAL } t_syntax_type;
		typedef enum e_type { CTX_ARITHMETIC, CTX_ARITHMETIC_GROUP, CTX_SUBSHELL, CTX_BACKTICK, CTX_BRACE, CTX_BRACE_PARAM, CTX_BRACE_COMMAND } t_type;

	#pragma endregion

	#pragma region "Structures"

		typedef struct e_stack {
			t_type type;
			struct e_stack* prev;
		} t_stack;

		typedef struct e_context {
			t_stack	*stack;
			bool	in_quotes;
			bool	in_dquotes;
			bool	in_escape;
			bool	in_token;
			bool	error;
		} t_context;

	#pragma endregion

#pragma endregion

#pragma region Methods

	//	---------- INPUT -----------
	char	*get_input();

	//	--------- CONTEXT ----------
	t_stack	*stack_push(t_stack **top, t_type type);
	void	stack_pop(t_stack **top);
	t_stack	*stack_copy(const t_stack *stack);
	void	stack_clear(t_stack **stack);
	void	context_copy(t_context *dst, const t_context *src);

	//	------ EXPAND HISTORY ------
	void	expand_history(char **input, t_context *context);

	//	------- EXPAND ALIAS -------
	int		expand_alias(char **input, t_context *context);
	
	//	------- CHECK SYNTAX -------
	int		check_syntax(const char *input, int type, t_context *context);

#pragma endregion
