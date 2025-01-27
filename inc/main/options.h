/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:51:51 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/27 20:41:50 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include <stdbool.h>

#pragma endregion

#pragma region "Variables"

	#pragma region "Structures"

		typedef struct {
			bool	emacs;			//	1 = Emacs mode ON											0 = OFF
			bool	vi;				//	1 = Vi mode ON												0 = OFF

			bool	hist_on;		//	1 = History enabled											0 = Disabled
			bool	hist_local;		//	1 = Save history to disk									0 = Do not save

			bool	noglob;			//	1 = Globbing disabled										0 = Enabled
			bool	dotglob;		//	1 = Include hidden files (starting with .)					0 = Exclude
			bool	nullglob;		//	1 = Patterns that do not match expand to an empty string	0 = Keep pattern
			bool	failglob;		//	1 = Patterns that do not match cause an error				0 = No error
			bool	nocaseglob;		//	1 = Case-insensitive globbing								0 = Case-sensitive
		}	t_options;

	#pragma endregion

	extern t_options	options;

#pragma endregion

#pragma region "Methods"

	int		options_set(const char *option, bool value);
	void	options_print();
	int		options_initialize();

#pragma endregion
