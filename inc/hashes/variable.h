/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 19:11:28 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/10 22:10:29 by vzurera-         ###   ########.fr       */
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
					t_var	**array;	// For numerics and associative arrays (hash table)
				} data;
			t_var_type		flags;		// Type of the variable (exported, readonly, etc.)
			t_var			*next;		// Pointer to the next variable in the slot of the hash table
		} t_var;

	#pragma endregion

#pragma endregion

#pragma region "Methods"

	//	---------- COMMON -----------
	t_var	*variable_find(t_env *env, const char *key);
	int		variable_validate(const char *key, int local_assing);
		//	---------- SCALAR -----------
	char	*variable_scalar_get(t_env *env, const char *key);
	int		variable_scalar_set(t_env *env, const char *key, const char *value, int append, int type, int local);
	char	*variable_scalar_value(t_env *env, const char *key);
	//	---------- ARRAY -----------
	char	*variable_array_get(t_env *env, const char *key, int index);
	int		variable_array_set(t_env *env, const char *key, int index, const char *value, int append, int local);
	int		variable_array_remove(t_env *env, const char *key, int index);
	char	*variable_array_values(t_env *env, const char *key);
	//	-------- ASSOCIATIVE -------
	char	*variable_assoc_get(t_env *env, const char *key, const char *assoc_key);
	int		variable_assoc_set(t_env *env, const char *key, const char *assoc_key, const char *value, int append, int local);
	int		variable_assoc_remove(t_env *env, const char *key, const char *assoc_key);
	char	*variable_assoc_values(t_env *env, const char *key);
	//	---------- DELETE ----------
	void	variable_free(t_var *var);
	int		variable_unset(t_env *env, const char *key);
	void	variable_clear_table(t_var **table);
	void	variable_clear(t_env *env);
	//	---------- EXPORT ----------
	char	**variable_to_array(t_env *env);
	void	variable_print(t_env *env, unsigned int type, int sort, int local);
	//	-------- INITIALIZE --------
	int		variable_initialize(t_env *env, const char **envp);

#pragma endregion
