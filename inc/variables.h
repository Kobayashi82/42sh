/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 19:11:28 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/11 13:41:20 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region Includes

	#define HASH_SIZE 101	//	Using a prime number improves the distribution in the hash table

#pragma endregion

#pragma region Variables

	#pragma region Enumerators

		enum e_var_type { INTERNAL = 1, EXPORTED = 2, READONLY = 4, INTEGER = 8, EXPORTED_LIST = 16 };

	#pragma endregion

	#pragma region Structures

		typedef struct s_var t_var;
		typedef struct s_var {
			char	*name;
			char	*value;
			bool	exported;
			bool	readonly;
			bool	integer;
			t_var	*next;
		}	t_var;

	#pragma endregion

	extern t_var *main_table[HASH_SIZE];

#pragma endregion

#pragma region Methods

		//	----------- HASH -----------
	unsigned int	hash_index(const char *key);
		//	---------- IMPORT ----------
	int				variables_add(t_var **table, const char *name, const char *value, int exported, int readonly, int integer, int force);
	void			variables_from_array(t_var **table, char **array);
	void			variables_join(t_var **dst_table, t_var **src_table);
		//	---------- EXPORT ----------
	t_var			*variables_find(t_var **table, const char *name);
	char			**variables_to_array(t_var **table, int type);
		//	---------- DELETE ----------
	int				variables_delete(t_var **table, const char *name);
	void			variables_clear(t_var **table);
		//	---------- DEFAULT ---------
	void			variables_initialize(t_var **table);
		//	---------- OTROS -----------
	int				get_key_value(char *line, char **key, char **value, char sep);

#pragma endregion
