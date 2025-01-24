/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 11:47:53 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/24 12:30:56 by vzurera-         ###   ########.fr       */
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
	bool	brackets(char * input, char *pattern, int i, int *j);

#pragma endregion
