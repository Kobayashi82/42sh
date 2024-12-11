/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 20:50:07 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/11 13:57:18 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region Variables

	#pragma region Structures

		typedef struct {
			char	*line;
			void	*data;
			size_t	event;
			size_t	length;
			time_t	timestamp;
		}	HIST_ENTRY;

			// HISTFILE=/home/kobayashi/.bash_history
			// HISTFILESIZE=2000
			// HISTCONTROL=ignoredups:ignorespace

	#pragma endregion

#pragma endregion

#pragma region Methods

	//	--------- HISTORY ----------
	void	history_prev();
	void	history_next();
	//	-------- INITIALIZE --------
	int		history_initialize();

#pragma endregion
