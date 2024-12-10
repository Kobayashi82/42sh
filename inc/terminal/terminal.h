/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:40:29 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/10 21:07:10 by vzurera-         ###   ########.fr       */
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

		#pragma region Terminal

			typedef struct termios t_termios;
			typedef struct {
				t_termios	term;
				size_t		columns;
				size_t		rows;
				char		*input;
				int			signal;

				char 		*PS1;
				char 		*PS2;
				char		*prompt;

				char		*msg;

				int			bk_stdin;
				int			bk_stdout;
				int			bk_stderr;

				char		quoted;
			} t_terminal;

		#pragma endregion

	#pragma endregion

#pragma endregion

#pragma region Methods

	//	---------- BANNER ----------
	void	banner();
	void	welcome();

	//	---------- PRINT -----------
	int		print(int fd, char *str, int mode);

	//	---------- SIGNAL ----------
	void	initialize_signals();
	void	sigint_handler(int sig);
	void	sigquit_handler(int sig);

#pragma endregion
