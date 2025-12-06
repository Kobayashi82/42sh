/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:40:29 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/06 20:22:38 by vzurera-         ###   ########.fr       */
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

	void	signals_set();
	int		no_interactive_input(char *value);
	int		interactive_input();

#pragma endregion
