/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:04:02 by vzurera-          #+#    #+#             */
/*   Updated: 2025/03/06 13:30:11 by vzurera-         ###   ########.fr       */
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

		typedef enum e_syntax_error {
			IN_TOKEN,
			IN_TOKEN_EOF,
			TOKEN_NEAR,
			ARGS_ARITHMETIC,
			ARGS_SUBSHELL,
		} t_syntax_error;

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
			bool	in_escape;
			bool	in_token;
			bool	error;
		} t_context;

	#pragma endregion

#pragma endregion

#pragma region Methods

	//	--------- CONTEXT ----------
	t_stack	*stack_push(t_stack **top, t_type type);
	void	stack_pop(t_stack **top);
	t_stack	*stack_copy(const t_stack *stack);
	void	stack_clear(t_stack **stack);
	int		is_context(t_stack *stack, t_type type);
	void	context_copy(t_context *dst, const t_context *src);

	//	---------- ERROR -----------
	void	syntax_error(t_syntax_error error, char *value, int line);

	//	---------- CHECK -----------
	int		syntax_check(const char *input, t_context *context, int line);

#pragma endregion
