/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 19:11:28 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/08 21:38:36 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include <stddef.h>

	#define VARS_HASH_SIZE 101

#pragma endregion

#pragma region "Variables"

	#pragma region "Enumerators"

		enum e_var_type {
			INTERNAL,
			EXPORTED,
			READONLY,
			INTEGER,
			EXPORTED_LIST
		};

	#pragma endregion

	#pragma region "Structures"

		typedef struct s_var {
			char			*key;
			char			*value;
			int				exported;
			int				readonly;
			int				integer;
			struct s_var	*next;
		} t_var;

	#pragma endregion

#pragma endregion

#pragma region "Methods"

	//	---------- IMPORT ----------
	int		variables_add(t_var **table, const char *key, const char *value, int exported, int readonly, int integer, int force);
	int		variables_concatenate(t_var **table, const char *key, const char *value, int exported, int readonly, int integer, int force);
	void	variables_from_array(t_var **table, const char **array);
	void	variables_join(t_var **dst_table, t_var **src_table);
	int		variables_validate(const char *key, const char *value, const char *name, int is_asign, int show_msg);
	//	---------- EXPORT ----------
	t_var	*variables_find(t_var **table, const char *key);
	char	*variables_find_value(t_var **table, const char *key);
	char	**variables_to_array(t_var **table, int type, int sort);
	int		variables_print(t_var **table, int type, int sort);
	size_t	variables_length(t_var **table, int type);
	//	---------- DELETE ----------
	int		variables_delete(t_var **table, const char *key);
	void	variables_clear(t_var **table);
	//	-------- INITIALIZE --------
	int		variables_initialize(t_var **table, const char **envp);

#pragma endregion
