/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 13:08:16 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/06 20:24:37 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Messages

	#pragma region Redirection

		//	Print redirections error messages
		static void redirection_msg(int error, char *val) {
			if (error == OPEN_AMB)		print(2, ft_strjoin(val, ": ambiguous redirect\n", 0), FREE_PRINT);
			if (error == OPEN_NO_FILE)	print(2, ft_strjoin(val, ": No such file or directory\n", 0), FREE_PRINT);
			if (error == OPEN_READ)		print(2, ft_strjoin(val, ": Permission denied\n", 0), FREE_PRINT);
			if (error == OPEN_DIR)		print(2, ft_strjoin(val, ": is a directory\n", 0), FREE_PRINT);
			if (error == OPEN_FAIL)		print(2, ft_strjoin(val, ": No such file or directory\n", 0), FREE_PRINT);
			if (error == DUP_FAIL)		print(2, "error duplicating file descriptor\n", PRINT);
			if (error == PIPE_FAIL)		print(2, "pipe failed\n", PRINT);
			if (error == SUB_HEREDOC)	print(2, ft_strjoin_sep("<< ", val, ": here-document in subshell\n", 0), FREE_PRINT);
		}

	#pragma endregion

	#pragma region Catastrophic

		//	Print main process error messages
		static void catastrophic_msg(int error, char *value) {
			if (error == NO_MEMORY)		print(2, "CATASTROPHIC - No memory left on the device\n", PRINT);
			if (error == FORK_FAIL)		print(2, "CATASTROPHIC - fork failed\n", PRINT);
			if (error == START_ARGS)	print(2, "-c: option requires an argument\n", PRINT);
			if (error == START_BIN)		print(2, ft_strjoin(value, ": cannot execute binary file\n", 0), FREE_PRINT);
			if (error == START_DIR)		print(2, ft_strjoin(value, ": is a directory\n", 0), FREE_PRINT);
			if (error == SHLVL_HIGH)	print(2, ft_strjoin_sep("warning: shell level (", value, ") too high, resetting to 1\n", 0), FREE_PRINT);
		}

	#pragma endregion

#pragma endregion

#pragma region Exit_Error

	//	Print the error message and exit if required
	int	exit_error(int error, int code, char *value, bool fatal) {
		if (!value) value = "";
		print(2, "42sh: ", RESET);
		catastrophic_msg(error, value);
		redirection_msg(error, value);
		// builtsin_msg(error, value);
		// execution_msg(error, value);

		if (code) data.shell.exit_code = code;
		if (fatal) { exit(data.shell.exit_code % 256); }

		return (data.shell.exit_code);
	}

#pragma endregion

#pragma region Exit

void exit(int code) {
	disable_raw_mode();
	data_free();
	_exit(code);
}

#pragma endregion
