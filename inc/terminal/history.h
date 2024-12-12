/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 20:50:07 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/12 13:49:23 by vzurera-         ###   ########.fr       */
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

	//	--------- HISTORY ----------
	int		history_prev();
	int		history_next();
	//	-------- INITIALIZE --------
	int		history_initialize();

#pragma endregion
