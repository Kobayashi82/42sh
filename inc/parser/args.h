/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 17:30:31 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/26 14:51:46 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"


#pragma endregion

#pragma region "Variables"

	#pragma region "Enumerators"

		typedef enum e_arg_type {
			STRING,				// Literal string
			CMD_SHELL,			// Executed in a subshell			()
			CMD_SUB,			// Modern command substitution		$()
			CMD_SUB_LGCY,		// Legacy command substitution		``
			BLOCK,				// Execute in the current shell		{}
			ARITH,				// Arithmetic						(())
			ARITH_SUB,			// Arithmetic substitution			$(())
			PROCESS_SUB_IN,		// Process substitution (input)		<()
			PROCESS_SUB_OUT,	// Process substitution (output)	>()
			VAR_EXP,			// Variable expansion				$VAR
			PARAM_EXP,			// Parameter expansion				${VAR:-VAL}
			TILDE_EXP,			// Tilde expansion					~
			QUOTE_SGL,			// Single quote						'
			QUOTE_DBL,			// Double quote						"
			GLOB,				// Pattern matching					*, ?, [...]
		}	e_arg;

	#pragma endregion

	#pragma region "Structures"

		typedef struct s_cmd	t_cmd;
		typedef struct s_arg	t_arg;
		typedef struct s_arg {
			char	*value;
			int		extra;
			t_cmd	*cmd;
			t_arg	*subarg;
			t_arg	*prev;
			t_arg	*next;			
			e_arg	type;
		}	t_arg;

	#pragma endregion

#pragma endregion

#pragma region "Methods"

	void	args_merge(t_arg **args, t_arg **new_args);
	void	args_sort(t_arg **files);
	void	args_clear(t_arg **args);

#pragma endregion