/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:40:29 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/28 22:19:35 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region Includes

	#include <stddef.h>
	#include <termios.h>

#pragma endregion

#pragma region Variables

	#pragma region Structures

		typedef struct termios	t_termios;
		typedef struct {
			t_termios	term;
			size_t		rows;
			size_t		cols;
			char		*input;

			char 		*PS1;
			char 		*PS2;
			char		*prompt;

			int			bk_stdin;
			int			bk_stdout;
			int			bk_stderr;

			char		quoted;
		} t_terminal;

	#pragma endregion

	extern t_terminal	terminal;

#pragma endregion

#pragma region Methods

	int	no_interactive_input(char *value);
	int	interactive_input();

#pragma endregion
