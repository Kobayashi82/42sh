/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 19:11:28 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/10 14:54:44 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include <stddef.h>

#pragma endregion

#pragma region "Variables"

	#pragma region "Enumerators"

		typedef enum e_var_type {
			VAR_NONE			= 0,
			VAR_EXPORTED		= 1 << 0,	// 0x01
			VAR_READONLY		= 1 << 1,	// 0x02
			VAR_INTEGER			= 1 << 2,	// 0x04
			VAR_ARRAY			= 1 << 3,	// 0x08
			VAR_ASSOCIATIVE		= 1 << 4 	// 0x10
		} t_var_type;

		// Set:			var->flags |= VAR_EXPORTED
		// Remove:		var->flags &= ~VAR_EXPORTED
		// Check:		if (var->flags & VAR_EXPORTED)
		// Múltiples:	var->flags |= (VAR_READONLY | VAR_INTEGER)

	#pragma endregion

	#pragma region "Structures"

		typedef struct s_env t_env;
		typedef struct s_var t_var;
		typedef struct s_var {
			char			*key;		// Name of the variable
			union u_data {
					char	*scalar;	// For scalars
					t_var	**array;	// For numerics arrays (hash table)
					t_var	**assoc;	// For associative arrays (hash table)
				} data;
			t_var_type		flags;		// Type of the variable (exported, readonly, etc.)
			t_var			*next;		// Pointer to the next variable in the slot of the hash table
		} t_var;

	#pragma endregion

#pragma endregion

#pragma region "Methods"

	char	*variables_find_value(t_var **table, const char *key);

	//	---------- IMPORT ----------
	int		variables_add(t_var **table, const char *key, const char *value, int exported, int readonly, int integer, int force);
	int		variables_concatenate(t_var **table, const char *key, const char *value, int exported, int readonly, int integer, int force);
	void	variables_from_array(t_var **table, const char **array);
	void	variables_join(t_var **dst_table, t_var **src_table);
	int		variables_validate(const char *key, int local_assing);
	//	---------- EXPORT ----------
	t_var	*variables_find(t_env *env, const char *key);
	char	**export_to_array(t_var **table);
	void	variables_print(t_env *env, unsigned int type, int sort);

	int		variables_array_add(t_env *env, const char *key, int index, const char *value, int append, int local);

	//	---------- DELETE ----------
	int		variables_delete(t_var **table, const char *key);
	void	variables_clear(t_var **table);
	//	-------- INITIALIZE --------
	int		variables_initialize(t_var **table, const char **envp);

#pragma endregion
