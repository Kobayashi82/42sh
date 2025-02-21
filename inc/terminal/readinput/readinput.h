/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readinput.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:40:29 by vzurera-          #+#    #+#             */
/*   Updated: 2025/02/21 19:06:00 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include <stddef.h>
	#include <stdbool.h>

#pragma endregion

#pragma region "Variables"

	#pragma region "Enumerators"

		enum e_input_mode { READLINE, VI };
		enum e_vi_mode { INSERT, EDIT };

	#pragma endregion

	#pragma region "Structures"

		typedef struct s_buffer {
			unsigned char	c;
			char			*value;
			size_t			size, position, length;
			bool			SHIFT, ALT, CTRL;
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
	extern bool		searching;
	extern bool		pushed;

#pragma endregion

#pragma region "Methods"

	//	--------- READINPUT --------
	void	disable_raw_mode();
	char	*readinput(char *prompt);

	//	---------- MODES -----------
	int		readline(int readed);
	int		vi(int readed);
	int		dumb(int readed);

	//	------- AUTOCOMPLETE -------
	void	autocomplete();

	//	------ HISTORY SEARCH ------
	void	search_history();
	void	search_init();

	//	-------- UNDO/REDO ---------
	void	undo_push();
	void	undo_pop();
	void	undo_all();
	void	undo_clear();

#pragma endregion
