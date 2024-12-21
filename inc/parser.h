/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 17:30:31 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/21 17:45:53 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region Includes

	#define HASH_SIZE 101

#pragma endregion

#pragma region Variables

	#pragma region Enumerators

		typedef enum {
			CMD,				// Normal command
			// CMD_SHELL,			// Executed in a subshell			()
			// BLOCK,				// Execute in the current shell		{}
			// ARITH				// Arithmetic						(())
		} e_cmd;

		typedef enum {
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

		// OPAR, CPAR, AND, OR, SMCOL, SMAND
		typedef enum {
			PIPE,				// 
			LT,					// 
			GT,					// 
			DLT,				// 
			TLT,				// 
			DGT					// 
		} e_redir;

	#pragma endregion

	#pragma region Structures

		#pragma region Tokens

			typedef struct s_token	t_token;
			typedef struct s_cmd	t_cmd;
			typedef struct s_arg	t_arg;
			typedef struct s_redir	t_redir;
			typedef struct s_var	t_var;

			typedef struct s_cmd {
				char	*value;
				int		pid;
				int		fd[2];
				t_arg	*arg;
				t_redir	*redir;
				t_var	*vars[HASH_SIZE];
				t_cmd	*prev;
				t_cmd	*next;
				e_cmd	type;
			}	t_cmd;

			typedef struct s_arg {
				char	*value;
				t_cmd	*cmd;
				t_arg	*subarg;
				t_arg	*prev;
				t_arg	*next;			
				e_arg	type;
			}	t_arg;

			typedef struct s_redir {
				char	*value;
				t_cmd	*cmd;
				t_redir	*prev;
				t_redir	*next;
				e_redir	type;
			}	t_redir;

			
			// typedef struct s_token {
			// 	int		pid;
			// 	int		fd[2];
			// }	t_token;

			typedef struct s_token {
				t_cmd	*cmd;
				t_cmd	*last_cmd;
				t_arg	*files;
			}	t_token;

		#pragma endregion

	#pragma endregion

	extern t_token		tokens;

#pragma endregion
