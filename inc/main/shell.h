/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 13:53:43 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/09 12:32:02 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include "hashes/alias.h"
	#include "hashes/builtin.h"
	#include "hashes/hash.h"
	#include "hashes/variable.h"
	#include "main/options.h"

#pragma endregion

#pragma region "Variables"

	#define PROYECTNAME	"42sh"
	#define VERSION		"1.0"
	#define DEVELOPER	"Kobayashi82"
	#define LOGIN42		"vzurera-"

	#define HASH_SIZE 101

	#pragma region "Enumerators"

		#pragma region "Error"

			enum e_error_redirection {
				TMP_CREATE = 37,		// Temporary file creation failed
				TMP_WRITE = 38,			// Temporary file write failed
				TMP_READ = 39,			// Temporary file read failed
				OPEN_NOT_FOUND = 40,	// File not found
				OPEN_READ = 41,			// ??
				OPEN_WRITE = 42,		// ??
				OPEN_DIR = 43,			// File is a directory
				OPEN_FAIL = 44,			// Open failed
				DUP_FAIL = 45,			// Dup failed
				DUP2_FAIL = 46,			// Dup2 failed
				PIPE_FAIL = 47,			// Pipe failed
				REDIR_AMB = 48,			// Ambiguous redirection (more than one argumment)
				SUB_HEREDOC = 49		// ??
			};

			enum e_error_execution {
				CMD_LAST = 50,			// ??
				CMD_NOT_FOUND = 51,		// Command not found
				CMD_EXEC = 52,			// Permission for execution
				CMD_ISDIR = 53,			// Command is a directory
				CMD_FAIL = 54,			// ??
				FORK_FAIL = 55,			// Fork failed
				EXECVE_FAIL = 56		// Execve failed
			};

			enum e_error_builtsin {
				CD_PATH = 60,			// 
				CD_ARGS = 61,			// 
				CD_HOME = 62,			// 
				CD_OLDPWD = 63,			// 
				CD_ERROR = 64,			// 
				CD_PER = 65,			// 
				HIS_ARGS = 66,			// 
				HIS_DIG = 67,			// 
				EXIT_NUM = 68,			// 
				EXIT_ARGS = 69			// 
			};

			enum e_error {
				NOTHING = 0,			// No error (used to free and exit)
				SEGQUIT = 3,			// Not used
				SEGFAULT = 11,			// Not used
				STDIN_CLOSED = 14,		// 
				STDOUT_CLOSED = 15,		// 
				SYNTAX = 20,			// 
				SHLVL_HIGH = 21,		// 
				VAR_NAME = 30,			// 
				START_ARGS = 70,		// 
				START_BIN = 71,			// 
				START_DIR = 72,			// 
				NO_MEMORY = 90,			// No more memory available (usually an allocation failed)
				END = 256,				// 
				EE_RETURN = 299,		// Flag for exit_error() indicating that the shell must continue
				EE_EXIT = 300			// Flag for exit_error() indicating that the shell must exit
			};

		#pragma endregion

		#pragma region "Shell"

			typedef enum e_mode {
				MD_INTERACTIVE,			// The input comes from the standard input typed by the user
				MD_NO_INTERACTIVE,		// The input comes from the shell itself
				MD_STDIN,				// The input comes from the standard input (eg. ./42sh < script)
				MD_ARGUMENT,			// The input comes from an argument (eg. ./42sh -c "command")
				MD_FILE					// The input comes from the file (eg. ./42sh script)
			} t_mode;

			typedef enum e_source {
				SRC_SHELL,				// The global shell environmet
				SRC_SOURCE,				// For sourced scripts (independent positional parameters only)
				SRC_FUNCTION,			// For functions (independent positional parameters and variables)
			} t_source;

		#pragma endregion

	#pragma endregion

	#pragma region "Structures"

		typedef struct s_env {
			int				argc;						// $# - Number of positional parameters
			char			**argv;						// $1, $2, ... - Positional parameters
			const char		*argv0;						// $0 - Shell name or script name
			int				optpos;						// Index used by the 'getopts' builtin
			t_source		sourced;					// Environment source (SRC_SHELL, SRC_SOURCE, SRC_FUNCTION)
			t_var			*table[HASH_SIZE];			// Local variables (or global if base environment)
			struct s_env	*parent;					// Previous execution environment (NULL for base environment)
		} t_env;

		typedef struct s_ast t_ast;
		typedef struct s_shell {
			t_mode		mode;							// Execution mode (MD_INTERACTIVE, SRC_NON_INTERACTIVE, MD_STDIN, MD_ARGUMENT, MD_FILE)
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
			t_alias		*alias_table[HASH_SIZE];		// 
			t_builtin	*builtin_table[HASH_SIZE];		// 
			t_hash		*hash_table[HASH_SIZE];			// 
			t_options	options;						// 
			t_ast		*ast;							// Abstract syntax tree of the current command
			t_env		*env;							// Pointer to current execution environment
		} t_shell;

	#pragma endregion

	extern t_shell shell;								// Global variable for the shell

#pragma endregion
	
#pragma region "Methods"

	int	initialize(int argc, const char **argv, const char **envp);
	int	exit_error(int error, int code, char *value, int free_value, int fatal);

#pragma endregion
