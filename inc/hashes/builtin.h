/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 20:59:56 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/21 22:17:41 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include "parser/args.h"
	#include <stddef.h>

	#define BUILTIN_HASH_SIZE	101

#pragma endregion

#pragma region "Variables"

	#pragma region "Structures"

		typedef struct s_builtin t_builtin;
		typedef struct s_builtin{
			char		*name;
			bool		disabled;
			bool		special;
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
	bool		builtin_isactive(const char *name);
	char		**builtin_to_array(int disabled, bool special, bool sort);
	int			builtin_print(int disabled, bool special, bool sort);
	size_t		builtin_length(int disabled, bool special);
	void		builtin_clear();
	int			builtin_delete(const char *name);
	int			builtin_initialize();

#pragma endregion
