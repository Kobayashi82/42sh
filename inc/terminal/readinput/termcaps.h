/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcaps.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 13:30:40 by vzurera-          #+#    #+#             */
/*   Updated: 2025/02/01 18:24:29 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region Includes

	#include <stddef.h>

#pragma endregion

#pragma region Methods

	//	--------- TERMCAP ----------
	int		char_width(size_t position, char *value);
	int		chars_width(size_t from, size_t to, char *value);
	size_t	char_prev(size_t position, char *value);
	
	void	cursor_left(int moves);
	void	cursor_right(int moves);
	void	cursor_move(size_t from_pos, size_t to_pos);
	void	cursor_get();
	void	cursor_set(int row, int col);
	void	cursor_hide();
	void	cursor_show();
	// void	update_cursor(char *value);
	// void	update_cursor_length(int length);
	int		terminal_initialize();

#pragma endregion
