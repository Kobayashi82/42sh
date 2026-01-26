/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 13:08:16 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/26 16:34:09 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "terminal/terminal.h"
	#include "terminal/readinput/history.h"
	#include "terminal/readinput/prompt.h"
	#include "parser/parser.h"
	#include "internal/shell.h"
	#include "utils/utils.h"

#pragma endregion

#pragma region "Messages"

	#pragma region "Shell"

		static void shell_msg(int error_type, char *value1, char *value2) {
			if (error_type == E_NO_MEMORY)		print(STDERR_FILENO, ft_strjoin_sep(value1, value2, ": memory allocation failed\n", J_FREE_NONE), P_FREE_PRINT);

			if (error_type == E_STDIN_CLOSED)	print(STDERR_FILENO, "CATASTROPHIC - Standard input closed\n", P_PRINT);
			if (error_type == E_STDOUT_CLOSED)	print(STDERR_FILENO, "CATASTROPHIC - Standard output closed\n", P_PRINT);
			if (error_type == E_START_ARGS)		print(STDERR_FILENO, "-c: option requires an argument\n", P_PRINT);
			if (error_type == E_START_BIN)		print(STDERR_FILENO, ft_strjoin(value1, ": cannot execute binary file\n", J_FREE_NONE), P_FREE_PRINT);
			if (error_type == E_START_DIR)		print(STDERR_FILENO, ft_strjoin(value1, ": is a directory\n", J_FREE_NONE), P_FREE_PRINT);
			// if (error_type == E_SHLVL_HIGH)		print(STDERR_FILENO, ft_strjoin_sep("warning: shell level (", value, ") too high, resetting to 1\n", 0), P_FREE_PRINT);

			if (error_type == E_SOPT_INVALID)	print(STDERR_FILENO, ft_strjoin_sep(value1, value2, ": invalid shell option name\n",  J_FREE_NONE), P_FREE_PRINT);
		}

	#pragma endregion

	#pragma region "Variable"

		static void variable_msg(int error_type, char *value1, char *value2) {
			if (error_type == E_POS_PARAM_ARGS)			print(STDERR_FILENO, ft_strjoin(value1, ": numeric argument required\n", J_FREE_NONE), P_FREE_PRINT);
			if (error_type == E_POS_PARAM_NUMERIC)		print(STDERR_FILENO, ft_strjoin(value1, ": shift: too many arguments\n", J_FREE_NONE), P_FREE_PRINT);
			if (error_type == E_POS_PARAM_RANGE)		print(STDERR_FILENO, ft_strjoin(value1, ": shift count out of range\n",  J_FREE_NONE), P_FREE_PRINT);
			
			if (error_type == E_VAR_IDENTIFIER)			print(STDERR_FILENO, ft_strjoin_sep(value1, value2, ": not a valid identifier\n",  J_FREE_NONE), P_FREE_PRINT);
			if (error_type == E_VAR_MAX_REFERENCES)		print(STDERR_FILENO, ft_strjoin_sep(value1, value2, ": too many references\n",     J_FREE_NONE), P_FREE_PRINT);
			if (error_type == E_VAR_CYCLE_REFERENCE)	print(STDERR_FILENO, ft_strjoin_sep(value1, value2, ": circular name reference\n", J_FREE_NONE), P_FREE_PRINT);
			if (error_type == E_VAR_READONLY)			print(STDERR_FILENO, ft_strjoin_sep(value1, value2, ": readonly variable\n",       J_FREE_NONE), P_FREE_PRINT);
		}

	#pragma endregion

	#pragma region "Builtin"

		static void builtin_msg(int error_type, char *value1, char *value2) {
			// Getopt
			if (error_type == E_OPT_MAX) {
				print(STDERR_FILENO, value1,                                                                 P_JOIN);
				print(STDERR_FILENO, ft_strjoin_sep(": Too many options (max ", value2, ")\n", J_FREE_NONE), P_FREE_PRINT);
			}

			// Builtin
			if (error_type == E_BUILTIN_NOT_FOUND)	print(STDERR_FILENO, ft_strjoin_sep(value1, value2, ": not a shell builtin\n",              J_FREE_NONE), P_FREE_PRINT);

			// Hash
			if (error_type == E_HASH_EMPTY)			print(STDERR_FILENO, ": hash: hash table empty\n",                                                        P_PRINT);
			if (error_type == E_HASH_NOT_FOUND)		print(STDERR_FILENO, ft_strjoin_sep(value1, value2, ": not found\n",                        J_FREE_NONE), P_FREE_PRINT);

			// CD, PWD, Dirs, Pushd, Popd
			if (error_type >= E_DIRS_OFFSET && error_type <= E_DIRS_PERMISSION) print(STDERR_FILENO, value1,                                                  P_JOIN);
			if (error_type == E_DIRS_OFFSET)		print(STDERR_FILENO, ft_strjoin_sep(": ", value2, ": invalid number\n",                     J_FREE_NONE), P_FREE_PRINT);
			if (error_type == E_DIRS_RANGE)			print(STDERR_FILENO, ft_strjoin_sep(": ", value2, ": directory stack index out of range\n", J_FREE_NONE), P_FREE_PRINT);
			if (error_type == E_DIRS_NOT_FOUND)		print(STDERR_FILENO, ft_strjoin_sep(": ", value2, ": No such file or directory\n",          J_FREE_NONE), P_FREE_PRINT);
			if (error_type == E_DIRS_NOT_DIR)		print(STDERR_FILENO, ft_strjoin_sep(": ", value2, ": Not a directory\n",                    J_FREE_NONE), P_FREE_PRINT);
			if (error_type == E_DIRS_PERMISSION)	print(STDERR_FILENO, ft_strjoin_sep(": ", value2, ": Permission denied\n",                  J_FREE_NONE), P_FREE_PRINT);
			if (error_type == E_DIRS_EMPTY)			print(STDERR_FILENO, ft_strjoin(value1, ": directory stack empty\n",                        J_FREE_NONE), P_FREE_PRINT);
			if (error_type == E_DIRS_EMPTY_DIR)		print(STDERR_FILENO, ft_strjoin(value1, ": no other directory\n",                           J_FREE_NONE), P_FREE_PRINT);
			if (error_type == E_DIRS_ARGS)			print(STDERR_FILENO, ft_strjoin(value1, ": too many arguments\n",                           J_FREE_NONE), P_FREE_PRINT);
			if (error_type == E_DIRS_HOME)			print(STDERR_FILENO, ft_strjoin(value1, ": HOME not set\n",                                 J_FREE_NONE), P_FREE_PRINT);
			if (error_type == E_DIRS_OLDPWD)		print(STDERR_FILENO, ft_strjoin(value1, ": OLDPWD not set\n",                               J_FREE_NONE), P_FREE_PRINT);

			// History
			if (error_type == E_HIS_OPT_MORE)		print(STDERR_FILENO, ft_strjoin(value1, ": cannot use more than one of -anrw\n",            J_FREE_NONE), P_FREE_PRINT);
			if (error_type == E_HIS_ARGS)			print(STDERR_FILENO, ft_strjoin(value1, ": too many arguments\n",                           J_FREE_NONE), P_FREE_PRINT);
			if (error_type == E_HIS_READABLE)		print(STDERR_FILENO, ft_strjoin_sep(value1, value2, ": file is not readable\n",             J_FREE_NONE), P_FREE_PRINT);
			if (error_type == E_HIS_WRITABLE)		print(STDERR_FILENO, ft_strjoin_sep(value1, value2, ": file is not writable\n",             J_FREE_NONE), P_FREE_PRINT);
			if (error_type == E_HIS_NOT_FOUND)		print(STDERR_FILENO, ft_strjoin_sep(value1, value2, ": file does not exist\n",              J_FREE_NONE), P_FREE_PRINT);
			if (error_type == E_HIS_RANGE)			print(STDERR_FILENO, ft_strjoin_sep(value1, value2, ": history position out of range\n",    J_FREE_NONE), P_FREE_PRINT);
			if (error_type == E_HIS_NUMERIC)		print(STDERR_FILENO, ft_strjoin_sep(value1, value2, ": numeric argument required\n",        J_FREE_NONE), P_FREE_PRINT);

			// Alias
			if (error_type == E_ALIAS_NOT_FOUND)	print(STDERR_FILENO, ft_strjoin_sep(value1, value2, ": not found\n",                        J_FREE_NONE), P_FREE_PRINT);
			if (error_type == E_ALIAS_INVALID)		print(STDERR_FILENO, ft_strjoin_sep(value1, value2, ": not found\n",                        J_FREE_NONE), P_FREE_PRINT);

			// Exit / Logout
			if (error_type == E_EXIT_ARGS)			print(STDERR_FILENO, ft_strjoin(value1, ": too many arguments\n",                           J_FREE_NONE), P_FREE_PRINT);
			if (error_type == E_EXIT_NUMERIC)		print(STDERR_FILENO, ft_strjoin(value1, ": numeric argument required\n",                    J_FREE_NONE), P_FREE_PRINT);
			if (error_type == E_EXIT_OVERFLOW)		print(STDERR_FILENO, ft_strjoin(value1, ": numeric argument out of range\n",                J_FREE_NONE), P_FREE_PRINT);
			if (error_type == E_EXIT_NO_LOGIN)		print(STDERR_FILENO, ft_strjoin(value1, ": not login shell: use `exit'\n",                  J_FREE_NONE), P_FREE_PRINT);
		}

	#pragma endregion

	#pragma region "Redirection"

		static void redirection_msg(int error_type, char *value) {
			if (error_type == E_TMP_CREATE)		print(STDERR_FILENO, ft_strjoin_sep(value, ": cannot create temp file - ",   strerror(errno), J_FREE_NONE), P_FREE_JOIN);
			if (error_type == E_TMP_WRITE)		print(STDERR_FILENO, ft_strjoin_sep(value, ": cannot write to temp file - ", strerror(errno), J_FREE_NONE), P_FREE_JOIN);
			if (error_type == E_TMP_READ)		print(STDERR_FILENO, ft_strjoin_sep(value, ": cannot create temp file - ",   strerror(errno), J_FREE_NONE), P_FREE_JOIN);
			if (error_type == E_TMP_CREATE || error_type == E_TMP_WRITE || error_type == E_TMP_READ)	print(STDERR_FILENO, "\n",               P_PRINT);

			if (error_type == E_REDIR_AMB)		print(STDERR_FILENO, ft_strjoin(value, ": ambiguous redirect\n", J_FREE_NONE), P_FREE_PRINT);
			if (error_type == E_OPEN_NOT_FOUND)	print(STDERR_FILENO, ft_strjoin(value, ": No such file or directory\n", J_FREE_NONE), P_FREE_PRINT);
			if (error_type == E_OPEN_READ)		print(STDERR_FILENO, ft_strjoin(value, ": Permission denied\n", J_FREE_NONE), P_FREE_PRINT);
			if (error_type == E_OPEN_DIR)		print(STDERR_FILENO, ft_strjoin(value, ": is a directory\n", J_FREE_NONE), P_FREE_PRINT);
			if (error_type == E_OPEN_FAIL)		print(STDERR_FILENO, ft_strjoin(value, ": No such file or directory\n", J_FREE_NONE), P_FREE_PRINT);
			if (error_type == E_DUP_FAIL)		print(STDERR_FILENO, "error duplicating file descriptor\n", P_PRINT);
			if (error_type == E_PIPE_FAIL)		print(STDERR_FILENO, "pipe failed\n", P_PRINT);
			if (error_type == E_SUB_HEREDOC)	print(STDERR_FILENO, ft_strjoin_sep("<< ", value, ": here-document in subshell\n", J_FREE_NONE), P_FREE_PRINT);
		}

	#pragma endregion

	#pragma region "Execution"

		static void execution_msg(int error_type, char *value) {
			if (error_type == E_FORK_FAIL)		print(STDERR_FILENO, ft_strjoin_sep(value, ": fork failed - ",   strerror(errno), J_FREE_NONE), P_FREE_JOIN);
			if (error_type == E_EXECVE_FAIL)	print(STDERR_FILENO, ft_strjoin_sep(value, ": ", strerror(errno), J_FREE_NONE),                 P_FREE_JOIN);
			if (error_type == E_FORK_FAIL || error_type == E_EXECVE_FAIL) print(STDERR_FILENO, "\n",                                 P_PRINT);

			if (error_type == E_CMD_NOT_FOUND)	print(STDERR_FILENO, ft_strjoin(value, ": command not found\n", J_FREE_NONE),                  P_FREE_PRINT);
			if (error_type == E_CMD_ISDIR)		print(STDERR_FILENO, ft_strjoin(value, ": Is a directory\n", J_FREE_NONE),                     P_FREE_PRINT);
			if (error_type == E_CMD_EXEC)		print(STDERR_FILENO, ft_strjoin(value, ": Permission denied\n", J_FREE_NONE),                  P_FREE_PRINT);
		}

	#pragma endregion

#pragma endregion

#pragma region "Free & Exit"

	static void free_and_exit() {
		print(0, NULL, P_RESET_ALL);
		history_clear();
		alias_clear();
		builtin_clear();
		hash_clear();
		tmp_clear();
		prompt_clear(BOTH);
		ast_free(&shell.ast);
		free(shell.dirs.cwd);
		dirs_clear();

		while (shell.env) {
			t_env *parent = shell.env->parent;
			positional_params_clear(shell.env);
			variable_clear(shell.env);
			free(shell.env);
			shell.env = parent;
		}

		close(terminal.bk_stdin);
		close(terminal.bk_stdout);
		close(terminal.bk_stderr);
		exit(shell.exit_code % 256);
	}

#pragma endregion

#pragma region "Exit Error"

	// Prints the error message and exits if required
	//
	// error_type   = Determines the error type (see enum values)
	// exit_code    = If 0, uses 'shell.exit_code'; otherwise set to 'exit_code'
	// value1       = Builtin name or error source
	// value2       = Argument or additional error information
	// free_mode    = Specifies whether 'value1' and/or 'value2' must be freed  (EE_FREE_NONE, EE_FREE_VAL1, EE_FREE_VAL2, EE_FREE_BOTH)
	// exit_mode    = Specifies whether to return or exit the shell             (EE_RETURN, EE_EXIT)
	//
	// Example:
	//
	// value1 ↘            ↙ value2
	//   42sh: cd: Makefile: Not a directory

	int	exit_error(int error_type, int exit_code, char *value1, char *value2, int free_mode, int exit_mode) {
		if (!value1) {
			value1 = "";
			if (free_mode == EE_FREE_VAL1) free_mode = EE_FREE_NONE;
			if (free_mode == EE_FREE_BOTH) free_mode = EE_FREE_VAL2;
		}
		if (!value2) {
			value2 = "";
			if (free_mode == EE_FREE_VAL2) free_mode = EE_FREE_NONE;
			if (free_mode == EE_FREE_BOTH) free_mode = EE_FREE_VAL1;
		}

		if (error_type >= E_START_ARGS && error_type < E_END) {
			print(STDERR_FILENO, shell.name, P_RESET);
			if (value1 && *value1) print(STDERR_FILENO, ": ", P_JOIN);
		}

		shell_msg(error_type, value1, value2);
		variable_msg(error_type, value1, value2);
		redirection_msg(error_type, value1);
		builtin_msg(error_type, value1, value2);
		execution_msg(error_type, value1);

		if (free_mode == EE_FREE_VAL1 || free_mode == EE_FREE_BOTH)	free(value1);
		if (free_mode == EE_FREE_VAL2 || free_mode == EE_FREE_BOTH)	free(value2);

		if (exit_code)				shell.exit_code = exit_code;
		if (exit_mode == EE_EXIT)	free_and_exit();

		return (shell.exit_code);
	}

#pragma endregion
