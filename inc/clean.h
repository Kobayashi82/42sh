/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 10:43:33 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/01 14:02:21 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdbool.h>

#pragma region Enumerators

	enum e_syntax_error {
		NL,
		PTK,
		TK,
		SOPAR,
		SCPAR,
		QUOTE
	};

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
		SYNTAX = 20,
		SHLVL_HIGH = 21,
		VAR_NAME = 30,
		START_ARGS = 70,
		START_BIN = 71,
		START_DIR = 72,
		NO_MEMORY = 90,
		END = 99
	};

#pragma endregion

char	*safe_strdup(char *str);
int		safe_dup(int fd);

void	data_free();
int		exit_error(int error, int code, char *value, bool exit);
