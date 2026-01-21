/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 16:22:02 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/09 12:18:17 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include <stddef.h>

#pragma endregion

#pragma region "Variables"

	#pragma region "Structures"

		typedef struct s_hash {
			char			*name;
			char			*path;
			int				hits;
			struct s_hash	*next;
		} t_hash;

	#pragma endregion

#pragma endregion

#pragma region "Methods"

	//	---------- IMPORT ----------
	int		hash_add(const char *path, int check_file, int check_exec);
	//	---------- EXPORT ----------
	t_hash	*hash_find(const char *name, int ninja);
	char	*hash_find_value(const char *name, int ninja);
	char	**hash_to_array(int sort);
	int		hash_print(int sort);
	size_t	hash_length();
	//	---------- DELETE ----------
	int		hash_delete(const char *name);
	void	hash_clear();
	//	-------- INITIALIZE --------
	int		hash_initialize();

#pragma endregion
