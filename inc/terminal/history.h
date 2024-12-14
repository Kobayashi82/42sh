/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 20:50:07 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/14 16:14:27 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region Variables

	#define HIST_MAXSIZE 5000

	#pragma region Enumerators

		enum e_hist_type { HIST_MEM, HIST_FILE };

	#pragma endregion

	#pragma region Structures

		typedef struct {
			char	*line;
			void	*data;
			size_t	event;
			size_t	length;
		}	HIST_ENTRY;

			// 42HISTFILE			~/.42sh_history
			// 42HISTFILESIZE		2000
			// 42HISTSIZE			1000
			// 42HISTCMD			la crea al cargar el historial y le asigna el numero de evento actual (readonly)
			// 42HISTCONTROL		ignorespace:ignoredups:ignoreboth:erasedups

	#pragma endregion

#pragma endregion

#pragma region Methods

	int		history_read(const char *filename);
	int		history_write(const char *filename);

	int		history_add(char *line);

	HIST_ENTRY	*history_get(size_t pos);
	HIST_ENTRY	*history_current();

	void	history_print(bool hide_events);
	size_t	history_length();
	void	history_remove(size_t pos);
	void	history_remove_current();
	void	history_clear();
	void	history_replace(size_t pos, char *line, void *data);
	size_t	history_get_pos();
	void	history_set_pos(size_t pos);
	//	--------- HISTORY ----------
	char	*history_prev();
	char	*history_next();
	//	-------- INITIALIZE --------
	int		history_initialize();

#pragma endregion
