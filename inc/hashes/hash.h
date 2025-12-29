/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 16:22:02 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/29 18:50:14 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include <stddef.h>

	#define CMDP_HASH_SIZE	101

#pragma endregion

#pragma region "Variables"

	#pragma region "Structures"

		typedef struct s_cmdp {
			char			*name;
			char			*path;
			int				hits;
			struct s_cmdp	*next;
		} t_cmdp;

	#pragma endregion

	extern t_cmdp *hash_table[CMDP_HASH_SIZE];

#pragma endregion

#pragma region "Methods"

	//	---------- IMPORT ----------
	int		cmdp_add(const char *path, int check_file, int check_exec);
	//	---------- EXPORT ----------
	t_cmdp	*cmdp_find(const char *name, int ninja);
	char	*cmdp_find_value(const char *name, int ninja);
	char	**cmdp_to_array(int sort);
	int		cmdp_print(int sort);
	size_t	cmdp_length();
	//	---------- DELETE ----------
	int		cmdp_delete(const char *name);
	void	cmdp_clear();
	//	-------- INITIALIZE --------
	int		cmdp_initialize();

#pragma endregion
