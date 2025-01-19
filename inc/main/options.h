/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:51:51 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/16 23:09:35 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region Includes

	#include <stdbool.h>

#pragma endregion

#pragma region Variables

	#pragma region Structures

		typedef struct {
			bool	emacs;
			bool	vi;
			bool	hist_local;		//	1 = Guardar historial en disco, 0 = No guardar
			bool	hist_on;		//	1 = ON, 0 = OFF
		}	t_options;

	#pragma endregion

	extern t_options	options;

#pragma endregion

#pragma region Methods

	int		options_set(const char *option, bool value);
	void	options_print();
	int		options_initialize();

#pragma endregion
