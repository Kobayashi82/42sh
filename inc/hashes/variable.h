/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 19:11:28 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/10 11:36:16 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include <stddef.h>

#pragma endregion

#pragma region "Variables"

	#pragma region "Enumerators"

		enum e_var_type {
			VAR_SHELL,
			VAR_EXPORTED,
			VAR_READONLY,
			VAR_INTEGER,
			VAR_ARRAY,
			VAR_ASSOCIATIVE
		};

	#pragma endregion

	#pragma region "Structures"

		typedef struct s_env t_env;
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
	int		variables_validate(const char *key, int local_assing);
	//	---------- EXPORT ----------
	t_var	*variables_find(t_var **table, const char *key);
	char	*variables_find_value(t_var **table, const char *key);
	char	**variables_to_array(t_var **table, int type, int sort);
	void	variables_print(t_env *env, int type, int sort);
	//	---------- DELETE ----------
	int		variables_delete(t_var **table, const char *key);
	void	variables_clear(t_var **table);
	//	-------- INITIALIZE --------
	int		variables_initialize(t_var **table, const char **envp);

#pragma endregion
