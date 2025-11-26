/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 13:53:43 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/26 20:13:13 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include "parser/parser.h"

	#include <time.h>
	#include <stdbool.h>
	#include <sys/types.h>

#pragma endregion

#pragma region "Variables"

	#pragma region "Enumerators"

		enum { NOTHING = 0, FREE = 64, FORCE = 128, END = 256 };
		typedef enum e_process { SHELL = 0, SUBSHELL = 1, CHILD = 2 } t_process;

	#pragma endregion

	#pragma region "Structures"

		typedef struct s_shell {
			pid_t		pid;
			pid_t		parent_pid;
			int			subshell_level;
			int			seconds;
			int			epoch_seconds;
			float		epoch_realtime;
			int			uid, euid;
			char		*cwd;
			time_t		started;
			t_process	process;
			bool		interactive;
			bool		as_argument;
			t_ast_node	*ast;
			bool		exit;
			int			exit_code;
		} t_shell;

	#pragma endregion

	extern t_shell	shell;

#pragma endregion
	
#pragma region "Methods"

	//	----------- SHELL ----------
	int		shell_initialize();

#pragma endregion
