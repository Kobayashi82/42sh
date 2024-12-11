/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 13:53:43 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/11 14:08:06 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region Includes

	#include <time.h>

#pragma endregion

#pragma region Variables

	#pragma region Enumerators

		enum { NOTHING = 0, FREE = 64, FORCE = 128, END = 256 };
		typedef enum { SHELL = 0, SUBSHELL = 1, CHILD = 2 } e_process;

	#pragma endregion

	#pragma region Structures

		typedef struct {
			int			pid;
			int			parent_pid;
			int			subshell_level;
			int			seconds;
			int			epoch_seconds;
			float		epoch_realtime;
			int			uid, euid;
			time_t		started;
			e_process	process;
			bool		_inline;
			bool		exit;
			int			exit_code;
		} t_shell;

	#pragma endregion

	extern t_shell		shell;

#pragma endregion
	
#pragma region Methods

	//	----------- SHELL ----------
	int	shell_initialize();

#pragma endregion
