/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 09:43:32 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/05 10:09:09 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

	#pragma region History

		#pragma region Prev

			void history_prev() {
				write(1, "History Prev", 13);
			}

		#pragma endregion

		#pragma region Next

			void history_next() {
				write(1, "History Next", 13);
			}

		#pragma endregion

	#pragma endregion
