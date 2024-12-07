/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   42sh.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:18:26 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/07 19:48:23 by vzurera-         ###   ########.fr       */
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
	#include "variables.h"
	#include "builtin.h"

	#include <sys/wait.h>
	#include <time.h>
	#include <fcntl.h>
	#include <stdio.h>
	#include <unistd.h>
	#include <errno.h>
	#include <stdlib.h>
	#include <string.h>
	#include <limits.h>

#pragma endregion

#pragma region Variables

	#pragma region Enumerators

		typedef enum { SHELL = 0, SUBSHELL = 1, CHILD = 2 } e_process;
		typedef enum { CMD, LT, GT, DLT, TLT, DGT, OPAR, CPAR, PIPE, AND, OR, SMCOL, SMAND } e_type;

	#pragma endregion

	#pragma region Structures

		#pragma region Tokens

			typedef struct s_data	t_data;
			typedef struct s_token	t_token;
			typedef struct s_args	t_args;
			typedef struct s_redir	t_redir;

			typedef struct {
				int bk_stdin; int bk_stdout; int bk_stderr;
			}	t_bk_std;

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

		#pragma region Terminal

			typedef struct {
				char			*input;
				char			quoted;
				char			*prompt;
				char			*msg;
				int				signal;
				struct termios	term;
			} t_terminal;

		#pragma endregion

		#pragma region Shell

			typedef struct {
				int			pid;
				time_t		started;
				e_process	process;
				t_bk_std	bk_std;
				bool		_inline;
				bool		exit;
				int			exit_code;
			} t_shell;

		#pragma endregion

		#pragma region Data

			typedef struct s_data {
				t_shell			shell;
				t_terminal		terminal;
				t_tokens		tokens;
			} t_data;

		#pragma endregion

	#pragma endregion

	extern t_data	data;

#pragma endregion

#pragma region Methods



#pragma endregion

char	*format_timestamp(time_t timestamp);
time_t	get_timestamp(const char *date);
