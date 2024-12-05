/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   undo_redo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 10:10:10 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/05 21:50:07 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Undo

	void undo() {
		write(1, "undo", 4);
	}

#pragma endregion

#pragma region Redo

	void redo() {
		write(1, "redo", 4);
	}

#pragma endregion
