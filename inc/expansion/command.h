/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:04:02 by vzurera-          #+#    #+#             */
/*   Updated: 2025/03/06 13:21:03 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Variables"

	#pragma region "Structures"

		typedef struct s_context t_context;

	#pragma endregion

#pragma endregion

#pragma region Methods

	//	---------- SHELL -----------
	bool	is_separator(const char *input, size_t *i, char *last_token);
	bool	is_not_separator(char c);
	int		syntax_shell(const char *input, size_t *i, t_context *context, char *last_token, int *line);

#pragma endregion
