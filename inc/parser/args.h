/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 17:30:31 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/21 22:20:05 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include <stdbool.h>

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

		#pragma region "Tokens"

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

#pragma endregion
