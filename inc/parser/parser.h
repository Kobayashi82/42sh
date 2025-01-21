/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 17:30:31 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/21 22:19:45 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#define HASH_SIZE 101

	#include "args.h"

#pragma endregion

#pragma region "Variables"

	#pragma region "Enumerators"

		typedef enum {
			CMD,				// Normal command
			// CMD_SHELL,			// Executed in a subshell			()
			// BLOCK,				// Execute in the current shell		{}
			// ARITH				// Arithmetic						(())
		} e_cmd;

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

	#pragma region "Structures"

		#pragma region "Tokens"

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

			typedef struct s_redir {
				char	*value;
				t_cmd	*cmd;
				t_redir	*prev;
				t_redir	*next;
				e_redir	type;
			}	t_redir;

			typedef struct s_token {
				t_cmd	*cmd;
				t_cmd	*last_cmd;
				t_arg	*files;
				t_arg	*args;
			}	t_token;

		#pragma endregion

	#pragma endregion

	extern t_token		tokens;

#pragma endregion

#pragma region "Methods"

	char	*get_next_word(const char *input, size_t *pos, bool only_space);
	void	first_step();

#pragma endregion
