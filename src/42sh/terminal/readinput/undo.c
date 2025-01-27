/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   undo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 10:10:10 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/27 12:36:02 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"

#pragma endregion

#pragma region "Undo"

	void undo() {
		write(1, "undo", 4);
	}

#pragma endregion

#pragma region "Redo"

	void redo() {
		write(1, "redo", 4);
	}

#pragma endregion
