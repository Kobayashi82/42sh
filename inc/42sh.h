/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   42sh.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:18:26 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/11 15:56:52 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region Includes

	#include "libft.h"
	#include "colors.h"
	#include "clean.h"
	#include "options.h"
	#include "terminal.h"
	#include "readinput.h"
	#include "history.h"
	#include "prompt.h"
	#include "shell.h"
	#include "variables.h"
	#include "builtin.h"
	#include "utils.h"

	#include <sys/wait.h>
	#include <fcntl.h>
	#include <stdio.h>
	#include <unistd.h>
	#include <errno.h>
	#include <stdlib.h>
	#include <string.h>
	#include <limits.h>

	#include "tests.h"

	#define VERSION "1.0"

#pragma endregion

#pragma region Variables

	#pragma region Enumerators

		typedef enum { CMD, LT, GT, DLT, TLT, DGT, OPAR, CPAR, PIPE, AND, OR, SMCOL, SMAND } e_type;

	#pragma endregion

	#pragma region Structures

		#pragma region Tokens

			typedef struct s_data	t_data;
			typedef struct s_token	t_token;
			typedef struct s_args	t_args;
			typedef struct s_redir	t_redir;

			typedef struct s_redir {
				e_type	type;
				char	*file;
				bool	ambiguous;
				bool	expand;
				t_redir	*prev;
				t_redir	*next;
			}	t_redir;

			typedef struct s_args {
				char	*arg;
				t_token	*token;
				t_data	*data;
				t_args	*prev;
				t_args	*next;
			}	t_args;

			typedef struct s_token {
				int		pid;
				int		fd[2];
				char	*cmd;
				e_type	type;
				t_args	*args;
				t_redir	*redir;
				t_data	*data;
				t_token	*prev;
				t_token	*next;
			}	t_token;

			typedef struct {
				t_token	*tokens;
				t_token	*last_token;
				t_args	*files;
			}	t_tokens;

		#pragma endregion

	#pragma endregion

	extern t_tokens		tokens;

#pragma endregion

#pragma region Methods

	//	-------- INITIALIZE --------
	int		initialize(int argc, const char **argv, const char **envp);

	//	---------- VARIOS ----------
	char	*home_path(const char *user);

#pragma endregion
