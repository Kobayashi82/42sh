/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 12:02:57 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/28 17:23:05 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include "terminal/colors.h"

#pragma endregion

#pragma region Variables

	#pragma region Enumerators

		enum e_print {
			RESET,
			RESET_PRINT,
			FREE_RESET,
			FREE_RESET_PRINT,
			FREE_JOIN,
			FREE_PRINT,
			JOIN,
			PRINT,
			RESET_ALL
		};

	#pragma endregion

#pragma endregion

#pragma region Methods

	//	---------- PRINT -----------
	int		print(int fd, const char *str, int mode);

#pragma endregion
