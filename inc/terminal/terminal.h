/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:40:29 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/23 23:27:04 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region Includes

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

#pragma endregion
