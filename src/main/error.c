/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 13:08:16 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/10 21:41:54 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "terminal/terminal.h"
	#include "terminal/readinput/history.h"
	#include "terminal/readinput/prompt.h"
	#include "parser/parser.h"
	#include "main/shell.h"
	#include "utils/utils.h"

#pragma endregion

#pragma region "Messages"

	#pragma region "Catastrophic"

		//	Print catastrophic error messages
		static void catastrophic_msg(int error, char *value) {
			if (error == SEGQUIT)			print(STDERR_FILENO, "Quit\n", PRINT);
			if (error == SEGFAULT)			print(STDERR_FILENO, "CATASTROPHIC - Segmentation fault\n", PRINT);
			if (error == STDIN_CLOSED)		print(STDERR_FILENO, "CATASTROPHIC - Standard input closed\n", PRINT);
			if (error == STDOUT_CLOSED)		print(STDERR_FILENO, "CATASTROPHIC - Standard output closed\n", PRINT);
			if (error == NO_MEMORY)			print(STDERR_FILENO, "CATASTROPHIC - No memory left on the device\n", PRINT);
			if (error == START_ARGS)		print(STDERR_FILENO, "-c: option requires an argument\n", PRINT);
			if (error == START_BIN)			print(STDERR_FILENO, ft_strjoin(value, ": cannot execute binary file\n", 0), FREE_PRINT);
			if (error == START_DIR)			print(STDERR_FILENO, ft_strjoin(value, ": is a directory\n", 0), FREE_PRINT);
			if (error == SHLVL_HIGH)		print(STDERR_FILENO, ft_strjoin_sep("warning: shell level (", value, ") too high, resetting to 1\n", 0), FREE_PRINT);
		}

	#pragma endregion

	#pragma region "Redirection"

		//	Print redirections error messages
		static void redirection_msg(int error, char *value) {
			if (error == TMP_CREATE)		print(STDERR_FILENO, ft_strjoin_sep(value, ": cannot create temp file - ",   strerror(errno), 0), FREE_JOIN);
			if (error == TMP_WRITE)			print(STDERR_FILENO, ft_strjoin_sep(value, ": cannot write to temp file - ", strerror(errno), 0), FREE_JOIN);
			if (error == TMP_READ)			print(STDERR_FILENO, ft_strjoin_sep(value, ": cannot create temp file - ",   strerror(errno), 0), FREE_JOIN);
			if (error == TMP_CREATE || error == TMP_WRITE || error == TMP_READ)	print(STDERR_FILENO, "\n",                                    PRINT);

			if (error == REDIR_AMB)			print(STDERR_FILENO, ft_strjoin(value, ": ambiguous redirect\n", 0), FREE_PRINT);
			if (error == OPEN_NOT_FOUND)	print(STDERR_FILENO, ft_strjoin(value, ": No such file or directory\n", 0), FREE_PRINT);
			if (error == OPEN_READ)			print(STDERR_FILENO, ft_strjoin(value, ": Permission denied\n", 0), FREE_PRINT);
			if (error == OPEN_DIR)			print(STDERR_FILENO, ft_strjoin(value, ": is a directory\n", 0), FREE_PRINT);
			if (error == OPEN_FAIL)			print(STDERR_FILENO, ft_strjoin(value, ": No such file or directory\n", 0), FREE_PRINT);
			if (error == DUP_FAIL)			print(STDERR_FILENO, "error duplicating file descriptor\n", PRINT);
			if (error == PIPE_FAIL)			print(STDERR_FILENO, "pipe failed\n", PRINT);
			if (error == SUB_HEREDOC)		print(STDERR_FILENO, ft_strjoin_sep("<< ", value, ": here-document in subshell\n", 0), FREE_PRINT);
		}

	#pragma endregion

	#pragma region "Builtin"

		//	Print builtin error messages
		static void builtin_msg(int error, char *value) {
			(void) error;
			(void) value;
		}

	#pragma endregion

	#pragma region "Execution"

		//	Print execution error messages
		static void execution_msg(int error, char *value) {
			if (error == FORK_FAIL)			print(STDERR_FILENO, ft_strjoin_sep(value, ": fork failed - ",   strerror(errno), 0), FREE_JOIN);
			if (error == EXECVE_FAIL)		print(STDERR_FILENO, ft_strjoin_sep(value, ": ", strerror(errno), 0),                 FREE_JOIN);
			if (error == FORK_FAIL || error == EXECVE_FAIL) print(STDERR_FILENO, "\n",                                            PRINT);

			if (error == CMD_NOT_FOUND)		print(STDERR_FILENO, ft_strjoin(value, ": command not found\n", 0),                   FREE_PRINT);
			if (error == CMD_ISDIR)			print(STDERR_FILENO, ft_strjoin(value, ": Is a directory\n", 0),                      FREE_PRINT);
			if (error == CMD_EXEC)			print(STDERR_FILENO, ft_strjoin(value, ": Permission denied\n", 0),                   FREE_PRINT);
		}

	#pragma endregion

#pragma endregion

#pragma region "Exit_Error"

	static void free_and_exit() {
		print(0, NULL, RESET_ALL);
		history_clear();
		alias_clear();
		builtin_clear();
		hash_clear();
		tmp_clear();
		prompt_clear(BOTH);
		ast_free(&shell.ast);
		free(shell.cwd);

		while (shell.env) {
			t_env *parent = shell.env->parent;
			array_free(shell.env->argv);
			variable_clear(shell.env);
			free(shell.env);
			shell.env = parent;
		}

		close(terminal.bk_stdin);
		close(terminal.bk_stdout);
		close(terminal.bk_stderr);
		exit(shell.exit_code % 256);
	}

	//	Print the error message and exit if required
	int	exit_error(int error, int code, char *value, int free_value, int fatal) {
		if (!value) {
			value = "";
			free_value = 0;
		}

		if (error > NOTHING && error < END) {
			print(STDERR_FILENO, shell.name, RESET);
			if (value) print(STDERR_FILENO, ": ", JOIN);
		}

		catastrophic_msg(error, value);
		redirection_msg(error, value);
		builtin_msg(error, value);
		execution_msg(error, value);

		if (free_value) 		free(value);
		if (code)				shell.exit_code = code;
		if (fatal == EE_EXIT)	free_and_exit();

		return (shell.exit_code);
	}

#pragma endregion
