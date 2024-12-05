/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readinput.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:40:29 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/05 21:36:29 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region Includes

	#include <termios.h>
	#include <termcap.h>
	#include <stdbool.h>

#pragma endregion

#pragma region Variables

	#pragma region Enumerators

		enum e_input_mode { READLINE, VI };
		enum e_vi_mode { INSERT, EDIT };

	#pragma endregion

	#pragma region Structures

		#pragma region Buffer

			typedef struct s_buffer {
				unsigned char	c;
				char			*value, *prompt;
				size_t			size, position, length;
				bool			SHIFT, ALT, CTRL;
				int				row, col, rows, cols, start_row, start_col;
			}	t_buffer;

			typedef struct s_word {
				size_t	start, end, len;
				char	value[1024];
			}	t_word;

		#pragma endregion

		#pragma region History

			typedef struct s_history		t_history;
			typedef struct s_history_list 	t_history_list;

			typedef struct s_history {
				char		*value;
				int			event;
				t_history	*prev;
				t_history	*next;
			}	t_history;

			typedef struct s_history_list {
				t_history	*head;
				t_history	*tail;
				size_t		size;
				int 		file_start;
				int			file_end;
				int			next_event;
			}	t_history_list;

		#pragma endregion

	#pragma endregion

	extern bool		show_control_chars;
	extern bool		fake_segfault;
	extern bool		free_prompt;
	extern int		input_mode;
	extern int		vi_mode;
	extern t_buffer	buffer;

#pragma endregion

#pragma region Methods

	//	--------- TERMCAP ----------
	void	terminal_initialize();
	void	terminal_size();
	void	terminal_start();
	void	cursor_left(int moves);
	void	cursor_right(int moves);
	void	cursor_move(size_t from_pos, size_t to_pos);
	void	cursor_get();
	void	cursor_set(int row, int col);
	void	cursor_hide();
	void	cursor_show();
	int		char_width(size_t position, char *value);

	//	---------- PROMPT ----------
	char	*get_prompt();

	//	------- AUTOCOMPLETE -------

	//	--------- HISTORY ----------
	void	history_prev();
	void	history_next();

	//	-------- UNDO/REDO ---------
	void	undo();
	void	redo();

	//	--------- READLINE ---------

	//	--------- READINPUT --------
	void	disable_raw_mode();
	char	*readinput(char *prompt);

	//	--------- READLINE ---------
	int		readline(int readed);

	//	------------ VI ------------
	int		vi(int readed);

#pragma endregion
