/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 13:08:16 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/01 13:12:43 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

// //	Print builtsin error messages
// static void	builtsin_msg(t_data *int error, char *val)
// {
// 	if (error == CD_PATH)
// 	{
// 		print(2, ft_strjoin("cd: ", val, 0), FJ);
// 		print(2, ": No such file or directory\n", P);
// 	}
// 	if (error == CD_HOME)
// 		print(2, "cd: HOME not set\n", P);
// 	if (error == CD_OLDPWD)
// 		print(2, "cd: OLDPWD not set\n", P);
// 	if (error == CD_ARGS)
// 		print(2, "cd: too many arguments\n", P);
// 	if (error == CD_PER)
// 	{
// 		print(2, ft_strjoin("cd: ", val, 0), FJ);
// 		print(2, ": Permission denied\n", P);
// 	}
// 	if (error == HIS_ARGS)
// 		print(2, "history: too many arguments\n", P);
// 	if (error == HIS_DIG)
// 	{
// 		print(2, ft_strjoin("history: ", val, 0), FJ);
// 		print(2, ": numeric argument required\n", P);
// 	}
// }

// //	Print execution error messages
// static void	execution_msg(t_data *int error, char *val)
// {
// 	if (error == CD_ERROR)
// 	{
// 		print(2, ft_strjoin("cd: ", val, 0), FJ);
// 		print(2, ": No such file or directory\n", P);
// 	}
// 	if (error == CMD_LAST)
// 		print(2, "!!: event not found\n", P);
// 	if (error == CMD_DIR)
// 		print(2, ft_strjoin(val, ": is a directory\n", 0), FP);
// 	if (error == CMD_NO_FILE)
// 		print(2, ft_strjoin(val, ": command not found\n", 0), FP);
// 	if (error == CMD_EXEC)
// 		print(2, ft_strjoin(val, ": Permission denied\n", 0), FP);
// 	if (error == CMD_FAIL)
// 		print(2, ft_strjoin(val, ": command failed\n", 0), FP);
// 	if (error == EXIT_NUM)
// 	{
// 		print(2, ft_strjoin("exit: ", val, 0), FJ);
// 		print(2, ": numeric argument required\n", P);
// 	}
// 	if (error == EXIT_ARGS)
// 		print(2, "exit: too many arguments\n", P);
// }


//	Print redirections error messages
static void redirection_msg(int error, char *val) {
	if (error == OPEN_AMB)		print(2, ft_strjoin(val, ": ambiguous redirect\n", 0), FREE_PRINT);
	if (error == OPEN_NO_FILE)	print(2, ft_strjoin(val, ": No such file or directory\n", 0), FREE_PRINT);
	if (error == OPEN_READ)		print(2, ft_strjoin(val, ": Permission denied\n", 0), FREE_PRINT);
	if (error == OPEN_DIR)		print(2, ft_strjoin(val, ": is a directory\n", 0), FREE_PRINT);
	if (error == OPEN_FAIL)		print(2, ft_strjoin(val, ": No such file or directory\n", 0), FREE_PRINT);
	if (error == DUP_FAIL)		print(2, "error duplicating file descriptor\n", PRINT);
	if (error == PIPE_FAIL)		print(2, "pipe failed\n", PRINT);
	if (error == SUB_HEREDOC)	print(2, ft_strjoin_c("<< ", val, ": here-document in subshell\n", 0), FREE_PRINT);
}

//	Print main process error messages
static void catastrophic_msg(int error, char *value) {
	if (error == NO_MEMORY)		print(2, "CATASTROPHIC - No memory left on the device\n", PRINT);
	if (error == FORK_FAIL)		print(2, "CATASTROPHIC - fork failed\n", PRINT);
	if (error == START_ARGS)	print(2, "-c: option requires an argument\n", PRINT);
	if (error == START_BIN)		print(2, ft_strjoin(value, ": cannot execute binary file\n", 0), FREE_PRINT);
	if (error == START_DIR)		print(2, ft_strjoin(value, ": is a directory\n", 0), FREE_PRINT);
	if (error == SHLVL_HIGH)	print(2, ft_strjoin_c("warning: shell level (", value, ") too high, resetting to 1\n", 0), FREE_PRINT);
}

//	Print the error message and exit if required
int exit_error(int error, int code, char *value, bool exit)
{
	if (!value) value = "";
	print(2, "42sh: ", RESET);
	catastrophic_msg(error, value);
	redirection_msg(error, value);
	// builtsin_msg(error, value);
	// execution_msg(error, value);

	if (code) data.shell.exit_code = code;
	if (exit) { data_free(); _exit(data.shell.exit_code % 256); }

	return (data.shell.exit_code);
}
