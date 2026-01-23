/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 16:22:02 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/23 15:39:24 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

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

	//	----------- GET ------------
	t_hash	*hash_find(const char *name, int no_hit);
	char	*hash_get(const char *name, int no_hit);
	//	----------- ADD ------------
	int		hash_add(const char *path);
	//	---------- DELETE ----------
	int		hash_delete(const char *name);
	void	hash_clear();
	//	---------- PRINT -----------
	int		hash_print(char *name, int reusable, int is_name, int print_name);

#pragma endregion
