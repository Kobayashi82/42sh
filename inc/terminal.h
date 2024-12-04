/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:40:29 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/04 23:09:28 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region Includes

	#pragma once

	#include <signal.h>
	#include <sys/ioctl.h>
	#include <termios.h>
	#include <termcap.h>
	#include <stdbool.h>

#pragma endregion

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

	typedef struct s_buffer {
		unsigned char	c;
		char			*value;
		size_t			size, position, length;
		bool			SHIFT, ALT, CTRL;
		int				col, row, cols, rows;
	}	t_buffer;

	typedef struct s_word {
		size_t	start, end, len;
		char	value[1024];
	}	t_word;

#pragma endregion

#pragma region Variables

	extern bool fake_segfault;
	extern bool show_control_chars;
	extern t_buffer	buffer;

#pragma endregion

#pragma region Methods

	//	---------- SIGNAL ----------
	void	sigquit_handler(int sig);
	void	sigint_handler(int sig);
	void	initialize_signals();

	//	---------- PRINT -----------
	void	print_banner();
	void	print_welcome();
	int		print(int fd, char *str, int mode);

	//	--------- READLINE ---------
	char	*readline(const char* prompt);

	void	init_terminal_data();
	void	cursor_left(int moves);
	void	cursor_right(int moves);
	void	cursor_to_pos(size_t from_pos, size_t to_pos);
	int		char_width(size_t position, char *value);

	void	get_cursor_position();

#pragma endregion
