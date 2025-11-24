/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 13:26:01 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/22 15:11:46 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include <stddef.h>
	#include <stdbool.h>
	#include <sys/types.h>

	#define VARS_HASH_SIZE 101

#pragma endregion

#pragma region "Variables"

	#pragma region "Enumerators"

		// OPAR, CPAR, AND, OR, SMCOL, SMAND
		typedef enum {
			CMD,				// Normal command
			// CMD_SHELL,		// Executed in a subshell			()
			// BLOCK,			// Execute in the current shell		{}
			// ARITH			// Arithmetic						(())
		} e_token;

	#pragma endregion

	#pragma region "Structures"

		#pragma region "Tokens"

			typedef struct s_token	t_token;
			typedef struct s_arg	t_arg;
			typedef struct s_redir	t_redir;
			typedef struct s_var	t_var;

			typedef struct s_token {
				char	*value;
				int		fd[2];
				pid_t	pid;
				t_arg	*arg;
				t_redir	*redir;
				t_var	*vars[VARS_HASH_SIZE];
				e_token	type;
				t_token	*prev;
				t_token	*next;
			}	t_token;

		#pragma endregion

	#pragma endregion

	extern t_token	tokens;

#pragma endregion

#pragma region "Methods"

	char	*get_next_word(const char *input, size_t *pos, bool only_space);
	void	first_step();

#pragma endregion
