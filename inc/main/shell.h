/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 13:53:43 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/11 20:58:10 by vzurera-         ###   ########.fr       */
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

			enum e_error {
				NOTHING					= 0,		// No error (used to free and exit)
				E_START_ARGS			= 200,		// 
				E_START_BIN				= 201,		// 
				E_START_DIR				= 202,		// 
				E_NO_MEMORY				= 290,		// No more memory available (usually an allocation failed)
				E_END					= 297,		// 
				EE_RETURN				= 298,		// Flag for exit_error() indicating that the shell must continue
				EE_EXIT					= 299		// Flag for exit_error() indicating that the shell must exit
			};

			enum e_error_variables {
				E_VAR_MAX_REFERENCES	= 250,		// 
				E_VAR_CYCLE_REFERENCE	= 251,		// 
				E_VAR_INVALID_TYPE		= 252,		// 
				E_VAR_INVALID_INDEX		= 253,		// 
				E_VAR_READONLY			= 254		// 
			};

			enum e_error_builtsin {
				E_CD_PATH				= 310,		// 
				E_CD_ARGS				= 311,		// 
				E_CD_HOME				= 312,		// 
				E_CD_OLDPWD				= 313,		// 
				E_CD_ERROR				= 314,		// 
				E_CD_PER				= 315,		// 
				E_HIS_ARGS				= 340,		// 
				E_HIS_DIG				= 341,		// 
				E_EXIT_NUM				= 380,		// 
				E_EXIT_ARGS				= 381		// 
			};

			enum e_error_redirection {
				E_STDIN_CLOSED			= 520,		// 
				E_STDOUT_CLOSED			= 521,		// 
				E_TMP_CREATE			= 525,		// Temporary file creation failed
				E_TMP_WRITE				= 526,		// Temporary file write failed
				E_TMP_READ				= 527,		// Temporary file read failed
				E_OPEN_NOT_FOUND		= 530,		// File not found
				E_OPEN_READ				= 531,		// ??
				E_OPEN_WRITE			= 532,		// ??
				E_OPEN_DIR				= 533,		// File is a directory
				E_OPEN_FAIL				= 534,		// Open failed
				E_DUP_FAIL				= 536,		// Dup failed
				E_DUP2_FAIL				= 537,		// Dup2 failed
				E_PIPE_FAIL				= 538,		// Pipe failed
				E_REDIR_AMB				= 540,		// Ambiguous redirection (more than one argumment)
				E_SUB_HEREDOC			= 541		// ??
			};

			enum e_error_execution {
				E_CMD_LAST				= 600,		// ??
				E_CMD_NOT_FOUND			= 601,		// Command not found
				E_CMD_EXEC				= 602,		// Permission for execution
				E_CMD_ISDIR				= 603,		// Command is a directory
				E_CMD_FAIL				= 604,		// ??
				E_FORK_FAIL				= 605,		// Fork failed
				E_EXECVE_FAIL			= 606		// Execve failed
			};

		#pragma endregion

		#pragma region "Shell"

			typedef enum e_mode {
				MD_INTERACTIVE,						// The input comes from the standard input typed by the user
				MD_NO_INTERACTIVE,					// The input comes from the shell itself
				MD_STDIN,							// The input comes from the standard input (eg. ./42sh < script)
				MD_ARGUMENT,						// The input comes from an argument (eg. ./42sh -c "command")
				MD_FILE								// The input comes from the file (eg. ./42sh script)
			} t_mode;

			typedef enum e_source {
				SRC_SHELL,							// The global shell environmet
				SRC_SOURCE,							// For sourced scripts (independent positional parameters only)
				SRC_FUNCTION,						// For functions (independent positional parameters and variables)
			} t_source;

		#pragma endregion

	#pragma endregion

	#pragma region "Structures"

		typedef struct s_env {
			int				argc;					// $# - Number of positional parameters
			char			**argv;					// $1, $2, ... - Positional parameters
			const char		*argv0;					// $0 - Shell name or script name
			int				optpos;					// Index used by the 'getopts' builtin
			t_source		sourced;				// Environment source (SRC_SHELL, SRC_SOURCE, SRC_FUNCTION)
			t_var			*table[HASH_SIZE];		// Local variables (or global if base environment)
			struct s_env	*parent;				// Previous execution environment (NULL for base environment)
		} t_env;

		typedef struct s_ast t_ast;
		typedef struct s_shell {
			t_mode		mode;						// Execution mode (MD_INTERACTIVE, SRC_NON_INTERACTIVE, MD_STDIN, MD_ARGUMENT, MD_FILE)
			int			pid;						// Process ID of the current shell instance
			int			parent_pid;					// Parent process ID (PPID)
			int			uid, euid;					// Real user ID and effective user ID
			int			login_shell;				// True if started as a login shell (argv[0] starts with '-' or invoked with '-l')
			int			subshell_level;				// Current subshell nesting level
			int			exit_code;					// Exit status of the last executed command or final shell exit status
			int			exit;						// Non-zero when the shell is requested to terminate
			long		started;					// Timestamp when the shell was started
			int			seconds;					// Elapsed time since shell start (in seconds)
			int			epoch_seconds;				// Current UNIX time in seconds since the Epoch
			float		epoch_realtime;				// High-resolution current time since the Epoch (seconds, fractional)
			char		*cwd;						// Internal representation of the current working directory
			const char	*name_exec;					// Command used to invoke the shell (argv[0] without leading '-' for login shells)
			const char	*name_bin;					// Binary name only (extracted after the last '/' of 'name_exec')
			const char	*name;						// Name used in messages ("42sh" or "-42sh" for login shells)
			t_alias		*alias_table[HASH_SIZE];	// 
			t_builtin	*builtin_table[HASH_SIZE];	// 
			t_hash		*hash_table[HASH_SIZE];		// 
			t_options	options;					// 
			t_ast		*ast;						// Abstract syntax tree of the current command
			t_env		*env;						// Pointer to current execution environment
		} t_shell;

	#pragma endregion

	extern t_shell shell;							// Global variable for the shell

#pragma endregion
	
#pragma region "Methods"

	int	initialize(int argc, const char **argv, const char **envp);
	int	exit_error(int error, int code, char *value, int free_value, int fatal);

#pragma endregion
