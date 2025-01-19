/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readinput.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:40:29 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/19 14:58:57 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region Includes

	#include <stdbool.h>

#pragma endregion

#pragma region Variables

	#pragma region Enumerators

		enum e_input_mode { READLINE, VI };
		enum e_vi_mode { INSERT, EDIT };

	#pragma endregion

	#pragma region Structures

		typedef struct s_buffer {
			unsigned char	c;
			char			*value;
			size_t			size, position, length;
			bool			SHIFT, ALT, CTRL;
			int				row, col;
		}	t_buffer;

		typedef struct s_word {
			size_t	start, end, len;
			char	value[1024];
		}	t_word;

	#pragma endregion

	extern t_buffer	buffer;

	extern bool		show_control_chars;
	extern bool		fake_segfault;
	extern int		vi_mode;
	extern char		*tmp_line;

#pragma endregion

#pragma region Methods

	//	------- AUTOCOMPLETE -------

	//	-------- UNDO/REDO ---------
	void	undo();
	void	redo();

	//	--------- READLINE ---------

	//	--------- READINPUT --------
	void	disable_raw_mode();
	char	*readinput(char *prompt);

	//	--------- READLINE ---------
	int		readline(int readed);

	//	----------- DUMB -----------
	int		dumb(int readed);

	//	------------ VI ------------
	int		vi(int readed);

#pragma endregion
