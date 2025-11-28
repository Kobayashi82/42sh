/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arithmetic.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:04:02 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/28 23:30:16 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Variables"

	#pragma region "Structures"

		typedef struct s_context t_context;

	#pragma endregion

#pragma endregion

#pragma region Methods

	//	-------- ARITHMETIC --------
	int	is_arithmetic(const char *input);
	int	syntax_arithmetic(const char *input, size_t *i, t_context *context, char *last_token, int *line);

#pragma endregion
