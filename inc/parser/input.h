/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:04:02 by vzurera-          #+#    #+#             */
/*   Updated: 2025/03/04 18:42:41 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Variables"

	#pragma region "Enumerators"

		typedef enum e_type {
			CTX_QUOTE,
			CTX_DQUOTE,
			CTX_ARITHMETIC,
			CTX_ARITHMETIC_GROUP,
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

		typedef struct e_stack {
			t_type			type;
			char			value[4];
			int				nvalue;
			struct e_stack	*prev;
		} t_stack;

		typedef struct e_context {
			t_stack	*stack;
			bool	in_escape;
			bool	in_token;
			bool	error;
		} t_context;

	#pragma endregion

#pragma endregion

#pragma region Methods

	//	---------- INPUT -----------
	char	*expand_input(char *input);
	char	*get_input();

	//	--------- CONTEXT ----------
	t_stack	*stack_push(t_stack **top, t_type type);
	void	stack_pop(t_stack **top);
	t_stack	*stack_copy(const t_stack *stack);
	void	stack_clear(t_stack **stack);
	int		is_context(t_stack *stack, t_type type);
	void	context_copy(t_context *dst, const t_context *src);

	//	------ EXPAND HISTORY ------
	void	expand_history(char **input, t_context *context);

	//	------- EXPAND ALIAS -------
	int		expand_alias(char **input, t_context *context);
	
	//	------- SYNTAX CHECK -------
	bool	is_separator(const char *input, size_t *i, char *last_token);
	bool	is_not_separator(char c);
	bool	is_arithmetic(const char *input);
	int		syntax_check(const char *input, t_context *context, int line);

#pragma endregion
