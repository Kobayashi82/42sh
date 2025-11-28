/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 20:50:07 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/29 00:13:37 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include <stddef.h>

#pragma endregion

#pragma region "Variables"

	#pragma region "Enumerators"

		enum e_hist_type {
			HIST_MEM,
			HIST_FILE
		};

	#pragma endregion

	#pragma region "Structures"

		typedef struct {
			char	*line;
			void	*data;
			size_t	event;
			size_t	length;
		} HIST_ENTRY;

	#pragma endregion

#pragma endregion

#pragma region "Methods"

	//	----------- FILE -----------
	void		history_file_set(const char *filename);
	//	----------- SIZE -----------
	size_t		history_get_size(int type);
	void		history_set_size(size_t value, int type);
	void		history_unset_size(int type);
	//	---------- LOCAL -----------
	int			history_read(const char *filename);
	int			history_write(const char *filename);
	//	----------- ADD ------------
	int			history_add(char *line, int force);
	int			history_replace(size_t pos, char *line, void *data);
	//	--------- DELETE -----------
	void		history_remove_offset(int offset);
	void		history_remove(size_t pos);
	void		history_remove_event(size_t event);
	void		history_remove_current(int remove_event);
	void		history_remove_last_if_added(int remove_event);
	void		history_clear();
	//	---------- GET -------------
	HIST_ENTRY	**history_clone();
	size_t		history_length();
	HIST_ENTRY	*history_get(size_t pos);
	HIST_ENTRY	*history_get(size_t pos);
	HIST_ENTRY	*history_current();
	HIST_ENTRY	*history_event(size_t event);
	HIST_ENTRY	*history_get_last_if_added();
	//	--------- NAVIGATE ---------
	char		*history_prev();
	char		*history_next();
	size_t		history_get_pos();
	int			history_event_pos(size_t event);
	void		history_set_pos(size_t pos);
	void		history_set_pos_end();
	//	---------- PRINT -----------
	int			history_print(size_t offset, int hide_events);
	//	-------- INITIALIZE --------
	int			history_initialize();

#pragma endregion

// 42HISTFILE			~/.42sh_history
// 42HISTFILESIZE		2000
// 42HISTSIZE			1000
// 42HISTCMD			la crea al cargar el historial y le asigna el numero de evento actual (readonly)
// 42HISTCONTROL		ignorespace:ignoredups:ignoreboth:erasedups
