/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 13:53:43 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/09 10:40:44 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include "hashes/variable.h"

	#define PROYECTNAME	"42sh"
	#define VERSION		"1.0"
	#define DEVELOPER	"Kobayashi82"
	#define LOGIN42		"vzurera-"

#pragma endregion

#pragma region "Variables"

	#pragma region "Enumerators"

		#pragma region "Mode"

			typedef enum e_mode {
				SRC_INTERACTIVE,				// The input comes from the standard input typed by the user
				SRC_NO_INTERACTIVE,				// The input comes from the shell itself
				SRC_STDIN,						// The input comes from the standard input (eg. ./42sh < script)
				SRC_ARGUMENT,					// The input comes from an argument (eg. ./42sh -c "command")
				SRC_FILE						// The input comes from the file (eg. ./42sh script)
			} t_mode;

		#pragma endregion

		#pragma region "Error"

			enum e_error_redirection {
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

			enum e_error_execution {
				CMD_LAST = 50,
				CMD_NO_FILE = 51,
				CMD_EXEC = 52,
				CMD_DIR = 53,
				CMD_FAIL = 54,
				FORK_FAIL = 55
			};

			enum e_error_builtsin {
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

		typedef struct s_env {
			int				argc;						// $# - Number of positional parameters
			char			**argv;						// $1, $2, ... - Positional parameters
			const char		*argv0;						// $0 - Shell name or script name
			int				optpos;						// Index used by the 'getopts' builtin
			int				sourced;					// 1 if sourced script, 0 if function
			t_var			*table[VARS_HASH_SIZE];		// Local variables (or global if base environment)
			struct s_env	*parent;					// Previous execution environment (NULL for base environment)
		} t_env;

		typedef struct s_ast t_ast;
		typedef struct s_shell {
			t_mode		mode;							// Execution mode (SRC_INTERACTIVE, SRC_NON_INTERACTIVE, SRC_STDIN, SRC_ARGUMENT, SRC_FILE)
			int			pid;							// Process ID of the current shell instance
			int			parent_pid;						// Parent process ID (PPID)
			int			uid, euid;						// Real user ID and effective user ID
			int			login_shell;					// True if started as a login shell (argv[0] starts with '-' or invoked with '-l')
			int			subshell_level;					// Current subshell nesting level
			int			exit_code;						// Exit status of the last executed command or final shell exit status
			int			exit;							// Non-zero when the shell is requested to terminate
			long		started;						// Timestamp when the shell was started
			int			seconds;						// Elapsed time since shell start (in seconds)
			int			epoch_seconds;					// Current UNIX time in seconds since the Epoch
			float		epoch_realtime;					// High-resolution current time since the Epoch (seconds, fractional)
			char		*cwd;							// Internal representation of the current working directory
			const char	*name_exec;						// Command used to invoke the shell (argv[0] without leading '-' for login shells)
			const char	*name_bin;						// Binary name only (extracted after the last '/' of 'name_exec')
			const char	*name;							// Name used in messages ("42sh" or "-42sh" for login shells)
			t_ast		*ast;							// Abstract syntax tree of the current command
			t_env		*env;							// Pointer to current execution environment
		} t_shell;

	#pragma endregion

	extern t_shell shell;

#pragma endregion
	
#pragma region "Methods"

	int	initialize(int argc, const char **argv, const char **envp);
	int	exit_error(int error, int code, char *value, int fatal);

#pragma endregion
