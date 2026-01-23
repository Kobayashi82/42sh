/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 13:53:43 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/23 14:03:30 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include "internal/alias.h"
	#include "internal/builtin.h"
	#include "internal/dirs.h"
	#include "internal/hash.h"
	#include "internal/variable.h"
	#include "internal/options.h"
	#include "internal/error.h"

#pragma endregion

#pragma region "Variables"

	#define PROYECTNAME	"42sh"
	#define VERSION		"1.0"
	#define DEVELOPER	"Kobayashi82"
	#define LOGIN42		"vzurera-"
	#define HASH_SIZE	101

	#pragma region "Enumerators"

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

	#pragma region "Structures"

		typedef struct s_env {
			int				argc;					// $# - Number of positional parameters
			char			**argv;					// $1, $2, ... - Positional parameters
			const char		*argv0;					// $0 - Shell name or script name
			int				optpos;					// Index used by the 'getopts' builtin
			t_source		sourced;				// Environment source (SRC_SHELL, SRC_SOURCE, SRC_FUNCTION)
			t_var			*variable[HASH_SIZE];	// Local variables (or global if base environment)
			struct s_env	*parent;				// Previous execution environment (NULL for base environment)
		} t_env;

		typedef struct s_ast t_ast;
		typedef struct s_shell {
			t_mode			mode;					// Execution mode (MD_INTERACTIVE, SRC_NON_INTERACTIVE, MD_STDIN, MD_ARGUMENT, MD_FILE)
			int				pid;					// Process ID of the current shell instance
			int				parent_pid;				// Parent process ID (PPID)
			int				uid, euid;				// Real user ID and effective user ID
			int				login_shell;			// True if started as a login shell (argv[0] starts with '-' or invoked with '-l')
			int				subshell_level;			// Current subshell nesting level
			int				error;					// Error code (use as my own errno)
			int				exit_code;				// Exit status of the last executed command or final shell exit status
			int				exit;					// Non-zero when the shell is requested to terminate
			long			started;				// Timestamp when the shell was started
			int				seconds;				// Elapsed time since shell start (in seconds)
			int				epoch_seconds;			// Current UNIX time in seconds since the Epoch
			float			epoch_realtime;			// High-resolution current time since the Epoch (seconds, fractional)
			const char		*name_exec;				// Command used to invoke the shell (argv[0] without leading '-' for login shells)
			const char		*name_bin;				// Binary name only (extracted after the last '/' of 'name_exec')
			const char		*name;					// Name used in messages ("42sh" or "-42sh" for login shells)
			t_dirs			dirs;					// Internal representation of the current working directory and directory stack
			t_alias			*alias[HASH_SIZE];		// 
			t_builtin		*builtin[HASH_SIZE];	// 
			t_hash			*function[HASH_SIZE];	// Template para funcions
			t_dirs			jobs;					// Template para jobs
			t_hash			*hash[HASH_SIZE];		// 
			t_options		options;				// 
			t_ast			*ast;					// Abstract syntax tree of the current command
			t_env			*env;					// Pointer to current execution environment
		} t_shell;

	#pragma endregion

	extern t_shell shell;							// Global variable for the shell

#pragma endregion
	
#pragma region "Methods"

	int	initialize(int argc, const char **argv, const char **envp);

#pragma endregion
