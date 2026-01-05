/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 13:53:43 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/05 20:53:49 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include "parser/parser.h"

	#define PROYECTNAME	"42sh"
	#define VERSION		"1.0"
	#define DEVELOPER	"Kobayashi"

#pragma endregion

#pragma region "Variables"

	#pragma region "Enumerators"

		#pragma region "Source"

			typedef enum e_source {
				SRC_INTERACTIVE,	// The input comes from the standard input typed by the user
				SRC_NO_INTERACTIVE,	// The input comes from the shell itself
				SRC_STDIN,			// The input comes from the standard input (eg. ./42sh < script)
				SRC_ARGUMENT,		// The input comes from an argument (eg. ./42sh -c "command")
				SRC_FILE			// The input comes from the file (eg. ./42sh script)
			} t_source;

		#pragma endregion

		#pragma region "Error"

			enum e_redirection_error {
				OPEN_AMB = 40,
				OPEN_NO_FILE = 41,
				OPEN_READ = 42,
				OPEN_WRITE = 43,
				OPEN_DIR = 44,
				OPEN_FAIL = 45,
				DUP_FAIL = 46,
				DUP2_FAIL = 47,
				PIPE_FAIL = 48,
				SUB_HEREDOC = 49
			};

			enum e_execution_error {
				CMD_LAST = 50,
				CMD_NO_FILE = 51,
				CMD_EXEC = 52,
				CMD_DIR = 53,
				CMD_FAIL = 54,
				FORK_FAIL = 55
			};

			enum e_builtsin_error {
				CD_PATH = 60,
				CD_ARGS = 61,
				CD_HOME = 62,
				CD_OLDPWD = 63,
				CD_ERROR = 64,
				CD_PER = 65,
				HIS_ARGS = 66,
				HIS_DIG = 67,
				EXIT_NUM = 68,
				EXIT_ARGS = 69
			};

			enum e_error {
				NOTHING = 0,
				SEGQUIT = 3,
				SEGFAULT = 11,
				STDIN_CLOSED = 14,
				STDOUT_CLOSED = 15,
				SYNTAX = 20,
				SHLVL_HIGH = 21,
				VAR_NAME = 30,
				START_ARGS = 70,
				START_BIN = 71,
				START_DIR = 72,
				NO_MEMORY = 90,
				END = 256
			};

		#pragma endregion

	#pragma endregion

	#pragma region "Structures"

		typedef struct s_shell {
			int			pid;
			int			parent_pid;
			int			subshell_level;
			int			seconds;
			int			epoch_seconds;
			float		epoch_realtime;
			int			uid, euid;
			char		*cwd;
			long		started;
			t_source	source;
			t_ast		*ast;
			int			exit;
			int			exit_code;
			int			login_shell;
			int			optpos;
			const char	*name;
			const char	*fullname;
			const char	*arg0;
			const char	**argv;
			int			argc;
		} t_shell;

	#pragma endregion

	extern t_shell shell;

#pragma endregion
	
#pragma region "Methods"

	int	initialize(int argc, const char **argv, const char **envp);
	int	exit_error(int error, int code, char *value, int fatal);

#pragma endregion
