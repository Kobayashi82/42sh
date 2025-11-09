/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 19:11:28 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/21 22:18:15 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include <stddef.h>
	#include <stdbool.h>

	#define VARS_HASH_SIZE 101

#pragma endregion

#pragma region "Variables"

	#pragma region "Enumerators"

		enum e_var_type { INTERNAL = 0, EXPORTED = 1, READONLY = 2, INTEGER = 3, EXPORTED_LIST = 4 };

	#pragma endregion

	#pragma region "Structures"

		typedef struct s_var	t_var;
		typedef struct s_var {
			char	*name;
			char	*value;
			bool	exported;
			bool	readonly;
			bool	integer;
			t_var	*next;
		}	t_var;

	#pragma endregion

	extern t_var	*vars_table[VARS_HASH_SIZE];

#pragma endregion

#pragma region "Methods"

	//	---------- IMPORT ----------
	int				variables_add(t_var **table, const char *key, const char *value, int exported, int readonly, int integer, int force);
	int				variables_concatenate(t_var **table, const char *key, char *value, int exported, int readonly, int integer, int force);
	void			variables_from_array(t_var **table, const char **array);
	void			variables_join(t_var **dst_table, t_var **src_table);
	int				variables_validate(char *key, char *value, char *name, bool is_asign, bool show_msg);
	//	---------- EXPORT ----------
	t_var			*variables_find(t_var **table, const char *key);
	char			*variables_find_value(t_var **table, const char *key);
	char			**variables_to_array(t_var **table, int type, bool sort);
	int				variables_print(t_var **table, int type, bool sort);
	size_t			variables_length(t_var **table, int type);
	//	---------- DELETE ----------
	int				variables_delete(t_var **table, const char *key);
	void			variables_clear(t_var **table);
	//	-------- INITIALIZE --------
	int				variables_initialize(t_var **table, const char **envp);

#pragma endregion
