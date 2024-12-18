/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 19:11:28 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/18 14:27:16 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region Includes

	#define HASH_SIZE 101	//	Using a prime number improves the distribution in the hash table

#pragma endregion

#pragma region Variables

	#pragma region Enumerators

		enum e_var_type { INTERNAL = 0, EXPORTED = 1, READONLY = 2, INTEGER = 3, EXPORTED_LIST = 4 };

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

		typedef struct s_alias t_alias;
		typedef struct s_alias {
			char	*name;
			char	*value;
			t_alias	*next;
		} t_alias;

	#pragma endregion

	extern t_var	*vars_table[HASH_SIZE];
	extern t_alias	*alias_table[HASH_SIZE];

#pragma endregion

#pragma region Methods

	//	----------- HASH -----------
	unsigned int	hash_index(const char *key);

	//	========== ALIAS ===========
	//	---------- IMPORT ----------
	int				alias_add(const char *key, const char *value);
	void			alias_from_array(const char **array);
	//	---------- EXPORT ----------
	t_alias			*alias_find(const char *key);
	char			**alias_to_array(bool sort);
	int				alias_print(bool sort);
	//	---------- DELETE ----------
	int				alias_delete(const char *key);
	void			alias_clear();
	//	-------- INITIALIZE --------
	int				alias_initialize();

	//	======== VARIABLES =========
	//	---------- IMPORT ----------
	int				variables_add(t_var **table, const char *key, const char *value, int exported, int readonly, int integer, int force);
	void			variables_from_array(t_var **table, const char **array);
	void			variables_join(t_var **dst_table, t_var **src_table);
	//	---------- EXPORT ----------
	t_var			*variables_find(t_var **table, const char *key);
	char			**variables_to_array(t_var **table, int type, bool sort);
	int				variables_print(t_var **table, int type, bool sort);
	//	---------- DELETE ----------
	int				variables_delete(t_var **table, const char *key);
	void			variables_clear(t_var **table);
	//	-------- INITIALIZE --------
	int				variables_initialize(t_var **table);
	//	---------- OTROS -----------
	int				get_key_value(const char *line, char **key, char **value, char sep);

#pragma endregion
