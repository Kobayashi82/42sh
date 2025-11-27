/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 13:53:43 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/27 23:46:58 by vzurera-         ###   ########.fr       */
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

		typedef enum e_source {
			SRC_INTERACTIVE,	// The input comes from the standard input typed by the user
			SRC_NO_INTERACTIVE,	// The input comes from the shell itself
			SRC_STDIN,			// The input comes from the standard input (eg. ./42sh < script)
			SRC_ARGUMENT,		// The input comes from an argument (eg. ./42sh -c "command")
			SRC_FILE			// The input comes from the file (eg. ./42sh script)
		} t_source;

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
			t_source	source;
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
