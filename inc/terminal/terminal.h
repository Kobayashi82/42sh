/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:40:29 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/19 15:01:00 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region Includes

	#include "libft.h"
	#include "colors.h"

	#include <termcap.h>
	#include <termios.h>
	#include <signal.h>

#pragma endregion

#pragma region Variables

	#pragma region Enumerators

		enum e_print {
			RESET,
			RESET_PRINT,
			FREE_RESET,
			FREE_RESET_PRINT,
			FREE_JOIN,
			FREE_PRINT,
			JOIN,
			PRINT
		};

	#pragma endregion

	#pragma region Structures

		typedef struct termios t_termios;
		typedef struct {
			t_termios	term;
			size_t		rows;
			size_t		columns;
			char		*input;
			int			signal;

			char 		*PS1;
			char 		*PS2;
			char		*prompt;

			char		*msg[1024];

			int			bk_stdin;
			int			bk_stdout;
			int			bk_stderr;

			char		quoted;
		} t_terminal;

	#pragma endregion

	extern t_terminal	terminal;

#pragma endregion

#pragma region Methods

	//	---------- BANNER ----------
	void	banner();
	void	welcome();

	//	---------- PRINT -----------
	int		print(int fd, char *str, int mode);

	//	---------- SIGNAL ----------
	void	signals_set();
	void	sigint_handler(int sig);
	void	sigquit_handler(int sig);

	//	--------- TERMCAP ----------
	int		char_width(size_t position, char *value);
	
	void	cursor_left(int moves);
	void	cursor_right(int moves);
	void	cursor_move(size_t from_pos, size_t to_pos);
	void	cursor_get();
	void	cursor_set(int row, int col);
	void	cursor_hide();
	void	cursor_show();
	int		terminal_initialize();

#pragma endregion
