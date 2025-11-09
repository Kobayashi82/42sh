/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brace.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:04:02 by vzurera-          #+#    #+#             */
/*   Updated: 2025/03/06 12:36:05 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Variables"

	#pragma region "Structures"

		typedef struct s_context t_context;

	#pragma endregion

#pragma endregion

#pragma region Methods
	
	//	---------- BRACE -----------
	int		syntax_brace(const char *input, size_t *i, t_context *context, char *last_token, int *line);

#pragma endregion
