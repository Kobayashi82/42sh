/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 11:47:53 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/23 18:25:56 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include "args.h"

#pragma endregion

#pragma region "Variables"

	#pragma region "Enumerators"


	#pragma endregion

	#pragma region "Structures"

		typedef struct s_wc {
			char	*input;
			char	*pattern;
			int		input_len;
			int		pattern_len;
			int		match;
			int		start;
			int		i;
			int		j;
		}	t_wc;

	#pragma endregion

	extern t_arg	*files;
	extern int		extra;

#pragma endregion

#pragma region "Methods"

	//	WILDCARDS
	bool	is_directory(char *path);
	void	tilde(char **cpattern);
	void	wildcards(char **cpattern);
	void	expand_wildcards(t_arg *args);
	//	PATTERN
	void	dir_get(char *cpattern, char *dir);
	//	BRACKETS
	bool	brackets(t_wc *wc);

#pragma endregion
