/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 20:50:07 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/04 20:07:33 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include <time.h>

#pragma endregion

#pragma region "Variables"

	#pragma region "Enumerators"

		enum e_hist_type {
			HIST_MEM,
			HIST_FILE
		};

	#pragma endregion

	#pragma region "Structures"

		typedef struct s_HIST_ENTRY {
			char	*line;
			time_t	timestamp;
			size_t	length;
		} HIST_ENTRY;

	#pragma endregion

#pragma endregion

#pragma region "Methods"

	//	-------- EXPANSION ---------
	int			expand_history(char **input, int show_expansion);
	//	---------- SEARCH ----------
	int			history_search();
	//	----------- SIZE -----------
	size_t		history_size_get(int type);
	void		history_size_set(size_t value, int type);
	void		history_size_unset(int type);
	//	---------- LOCAL -----------
	void		history_file_set(const char *filename);
	int			history_read_append(const char *filename);
	int			history_read(const char *filename);
	int			history_write(const char *filename, int append);
	//	----------- ADD ------------
	void		history_hist_control_set(const char *value);
	void		history_hist_ignore_set(const char *value);
	int			history_add(char *line, int force);
	//	---------- REMOVE ----------
	void		history_remove_position(size_t pos);
	void		history_remove_offset_range(size_t start, size_t end);
	int			history_remove_offset(int offset);
	void		history_remove_current(int remove_event);
	void		history_remove_last_if_added(int remove_event);
	//	---------- CLEAR -----------
	void		history_clear();
		//	---------- GET -------------
	HIST_ENTRY	*history_entry_position(size_t pos);
	HIST_ENTRY	*history_entry_offset(int offset);
	HIST_ENTRY	*history_entry_current();
	HIST_ENTRY	*history_entry_last_if_added();
	size_t		history_position();
	size_t		history_position_offset(int offset, size_t *out);
	size_t		history_position_query(int offset, const char *query, size_t *out);
	size_t		history_length();
	size_t		history_event();
	size_t		history_histcmd();
	//	--------- NAVIGATE ---------
	char		*history_prev();
	char		*history_next();
	void		history_set_pos(size_t pos);
	void		history_set_pos_last();
	//	---------- PRINT -----------
	void		history_hist_timeformat_set(const char *format);
	int			history_print_range(size_t start, size_t end, int reverse, int hide_pos);
	int			history_print(size_t offset, int hide_pos);
	//	-------- INITIALIZE --------
	int			history_initialize();

#pragma endregion

// 42HISTFILE			~/.42sh_history
// 42HISTFILESIZE		2000
// 42HISTSIZE			1000
// 42HISTCMD			la crea al cargar el historial y le asigna el numero de evento actual (readonly)
// 42HISTCONTROL		ignorespace:ignoredups:ignoreboth:erasedups
