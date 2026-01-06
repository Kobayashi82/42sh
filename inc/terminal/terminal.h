/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:40:29 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/06 18:29:19 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region Includes

	#include <stddef.h>
	#include <termios.h>

#pragma endregion

#pragma region Variables

	#pragma region Structures

		typedef struct s_terminal {
			struct termios	term;
			size_t			rows;
			size_t			cols;
			char			*input;

			char 			*PS1;
			char 			*PS2;
			char 			*PS3;
			char 			*PS4;
			char			*prompt;

			int				bk_stdin;
			int				bk_stdout;
			int				bk_stderr;

			int				signal;
		} t_terminal;

	#pragma endregion

	extern t_terminal terminal;

#pragma endregion

#pragma region Methods

	const char	*signal_to_str(int sig);
	int			str_to_signal(const char *sig);
	void		signals_set();
	int			no_interactive_input(char *value);
	int			interactive_input();

#pragma endregion
