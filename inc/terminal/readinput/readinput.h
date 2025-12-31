/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readinput.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:40:29 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/31 15:44:21 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include <stddef.h>

#pragma endregion

#pragma region "Variables"

	#pragma region "Enumerators"

		enum e_input_mode {
			READLINE,
			VI
		};

	#pragma endregion

	#pragma region "Structures"

		typedef struct s_buffer {
			unsigned char	c;
			char			*value;
			size_t			size, position, length;
			int				SHIFT, ALT, CTRL;
		} t_buffer;

	#pragma endregion

	extern t_buffer	buffer;
	extern char		*term_prompt;
	extern int		raw_mode;
	extern int		hist_searching;
	extern int		vi_mode;

#pragma endregion

#pragma region "Methods"

	//	--------- READINPUT --------
	void	disable_raw_mode();
	char	*readinput(char *prompt);

	//	---------- MODES -----------
	int		readline(int readed);
	int		vi();
	int		dumb(int readed);

	//	------- AUTOCOMPLETE -------
	void	autocomplete();

	//	-------- UNDO/REDO ---------
	void	undo_push(int push);
	void	undo_pop();
	void	undo_all();
	void	undo_clear();

#pragma endregion
