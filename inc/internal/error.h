/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 11:35:42 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/26 11:09:58 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Enumerators"

	#pragma region "Error"

		enum e_error {
			EE_FREE_NONE			= 290,		// Dont free any value
			EE_FREE_VAL1			= 291,		// Free 'value1'
			EE_FREE_VAL2			= 292,		// Free 'value2'
			EE_FREE_BOTH			= 294,		// Free 'value1' and 'value2'
			EE_RETURN				= 298,		// The shell must continue
			EE_EXIT					= 299		// The shell must exit
		};

	#pragma endregion

	#pragma region "Shell"

		enum e_error_shell {
			NOTHING					= 0,		// No error (used to free and exit)
			E_START_ARGS			= 200,		// 
			E_START_BIN				= 201,		// 
			E_START_DIR				= 202,		// 
			E_NO_MEMORY				= 990,		// No more memory available (usually an allocation failed)
			E_END					= 997,		// 
		};

	#pragma endregion

	#pragma region "Variable"

		enum e_error_variable {
			E_POS_PARAM_ARGS		= 245,		// 
			E_POS_PARAM_NUMERIC		= 246,		// 
			E_POS_PARAM_RANGE		= 247,		// 

			E_VAR_MAX_REFERENCES	= 250,		// Maximum reference depth exceeded
			E_VAR_CYCLE_REFERENCE	= 251,		// Circular reference
			E_VAR_INVALID_TYPE		= 252,		// Invalid type
			E_VAR_INVALID_INDEX		= 253,		// Invalid array index
			E_VAR_IDENTIFIER		= 254,		// Invalid identifier
			E_VAR_READONLY			= 255		// Read-only variable
		};

	#pragma endregion

	#pragma region "Builtin"

		enum e_error_builtin {
			// Getopt
			E_OPT_MAX				= 300,		// Maximum number of options exceeded
			E_OPT_INVALID			= 301,		// Invalid option
			E_OPT_NO_ARGUMENT		= 302,		// Missing required argument for option
			E_OPT_ARGUMENT			= 303,		// Unexpected argument for option
			E_SOPT_INVALID			= 304,		// Invalid shell option

			// Builtin
			E_BUILTIN_NOT_FOUND		= 305,		// Builtin not found

			// Hash
			E_HASH_EMPTY			= 306,		// Hash is empty
			E_HASH_NOT_FOUND		= 307,		// Hash command not found

			// CD, PWD, Dirs, Pushd, Popd
			E_DIRS_EMPTY			= 310,		// Directory stack is empty
			E_DIRS_EMPTY_DIR		= 311,		// Directory stack is empty (used for pushd)
			E_DIRS_ARGS				= 312,		// Too many arguments
			E_DIRS_OFFSET			= 313,		// Invalid offset number
			E_DIRS_RANGE			= 314,		// Offset out of range
			E_DIRS_NOT_FOUND		= 315,		// Directory not found
			E_DIRS_NOT_DIR			= 316,		// Not a directory
			E_DIRS_PERMISSION		= 317,		// Permission denied
			E_DIRS_HOME				= 318,		// HOME variable not set
			E_DIRS_OLDPWD			= 319,		// OLDPWD variable not set

			// History
			E_HIS_ARGS				= 320,		// 
			E_HIS_DIG				= 321,		// 

			// Alias
			E_ALIAS_NOT_FOUND		= 330,		// 
			E_ALIAS_INVALID			= 331,		// 

			// Exit / Logout
			E_EXIT_ARGS				= 380,		// 
			E_EXIT_NUMERIC			= 381,		// 
			E_EXIT_OVERFLOW			= 382,		// 
			E_EXIT_NO_LOGIN			= 383		// 
		};

	#pragma endregion

	#pragma region "Redirection"

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

	#pragma endregion

	#pragma region "Execution"

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

#pragma endregion

#pragma region "Methods"

	int	exit_error(int error_type, int exit_code, char *value1, char *value2, int free_mode, int exit_mode);

#pragma endregion
