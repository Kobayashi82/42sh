/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:04:02 by vzurera-          #+#    #+#             */
/*   Updated: 2025/02/27 19:25:04 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Variables"

	#pragma region Enumerators

		typedef enum e_syntax_type { FULL, PARTIAL } t_syntax_type;
		typedef enum e_type { CTX_ARITHMETIC, CTX_SUBSHELL, CTX_BACKTICK } t_type;

	#pragma endregion

	#pragma region "Variables"

		typedef struct e_context {
			t_type type;
			int nest_level;
			struct e_context* prev;
		} t_context;

	#pragma endregion

#pragma endregion

#pragma region Methods

	//	---------- INPUT -----------
	char	*get_input();

	//	------- CHECK SYNTAX -------
	int		check_syntax(char *input, int type, t_context **stack, bool *in_quotes, bool *in_dquotes, bool *escape);

	//	------- EXPAND ALIAS -------
	int		expand_alias(char **input, t_context **stack, bool *in_quotes, bool *in_dquotes, bool *escape);
	void	stack_clear(t_context **stack);

	//	------ EXPAND HISTORY ------
	void	expand_history(char **input, int partial_mode);

#pragma endregion
