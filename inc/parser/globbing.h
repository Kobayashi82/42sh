/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 11:47:53 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/26 14:49:40 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include <stdbool.h>

#pragma endregion

#pragma region "Variables"

	#pragma region "Enumerators"


	#pragma endregion

	#pragma region "Structures"

		typedef struct s_arg		t_arg;
		typedef struct s_pattern	t_pattern;
		typedef struct s_pattern {
			char 		*value;
			t_pattern	*next;
		}	t_pattern;

	#pragma endregion

#pragma endregion

#pragma region "Methods"

	//	--------- WILDCARDS --------
	void	wildcards(char *pattern);
	void	expand_wildcards(t_arg *args);

	//	--------- PATTERN ----------
	t_pattern	*pattern_create(char *pattern);
	void		pattern_clear(t_pattern **patterns);

	//	---------- MATCH -----------
	t_arg	*dir_get(char *pattern, char *dir);

	//	--------- BRACKETS ---------
	bool	brackets(char * input, char *pattern, int i, int *j);

#pragma endregion
