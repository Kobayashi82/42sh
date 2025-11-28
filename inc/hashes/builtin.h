/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 20:59:56 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/28 23:31:47 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include <stddef.h>

	#define BUILTIN_HASH_SIZE	101

#pragma endregion

#pragma region "Variables"

	#pragma region "Structures"

		typedef struct s_arg		t_arg;
		typedef struct s_builtin	t_builtin;
		typedef struct s_builtin{
			char		*name;
			int			disabled;
			int			special;
		    int         (*execute)(t_arg *args);
			t_builtin	*next;
		}	t_builtin;

	#pragma endregion

	extern t_builtin	*builtin_table[BUILTIN_HASH_SIZE];

#pragma endregion

#pragma region "Methods"

	//	======= BUILTIN_HASH =======
	int			builtin_add(const char *name, int disabled, int special, int (*execute)(t_arg *args));
	t_builtin	*builtin_find(const char *name);
	int			builtin_isactive(const char *name);
	char		**builtin_to_array(int disabled, int special, int sort);
	int			builtin_print(int disabled, int special, int sort);
	size_t		builtin_length(int disabled, int special);
	void		builtin_clear();
	int			builtin_delete(const char *name);
	int			builtin_initialize();

#pragma endregion
