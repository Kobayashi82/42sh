/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdp.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 16:22:02 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/19 19:35:21 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region Includes

	#define CMDP_HASH_SIZE 101

#pragma endregion

#pragma region Variables

	#pragma region Structures

		typedef struct s_cmdp	t_cmdp;
		typedef struct s_cmdp {
			char	*name;
			char	*path;
			int		hits;
			t_cmdp	*next;
		}	t_cmdp;

	#pragma endregion

	extern t_cmdp	*cmdp_table[CMDP_HASH_SIZE];

#pragma endregion

#pragma region Methods

	//	---------- IMPORT ----------
	int				cmdp_add(const char *path, bool check_file, bool check_exec);
	//	---------- EXPORT ----------
	t_cmdp			*cmdp_find(const char *name, bool ninja);
	char			*cmdp_find_value(const char *name, bool ninja);
	char			**cmdp_to_array(bool sort);
	int				cmdp_print(bool sort);
	size_t			cmdp_length();
	//	---------- DELETE ----------
	int				cmdp_delete(const char *name);
	void			cmdp_clear();
	//	-------- INITIALIZE --------
	int				cmdp_initialize();

#pragma endregion
