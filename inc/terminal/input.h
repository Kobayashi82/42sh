/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:04:02 by vzurera-          #+#    #+#             */
/*   Updated: 2025/02/27 22:19:05 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Variables"

	#pragma region "Enumerators"

		typedef enum e_syntax_type { FULL, PARTIAL } t_syntax_type;
		typedef enum e_type { CTX_ARITHMETIC, CTX_SUBSHELL, CTX_BACKTICK } t_type;

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

	//	------- CHECK SYNTAX -------
	int		check_syntax(const char *input, int type, t_context *main);

	//	------- EXPAND ALIAS -------
	int		expand_alias(char **input, t_context *alias);
	void	stack_clear(t_stack **stack);
	void	context_copy(t_context *dst, const t_context *src);

	//	------ EXPAND HISTORY ------
	void	expand_history(char **input, t_context *main);

#pragma endregion
