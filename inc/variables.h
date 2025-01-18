/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 19:11:28 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/18 18:57:03 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region Includes

	#define HASH_SIZE 101

#pragma endregion

#pragma region Variables

	#pragma region Enumerators

		enum e_var_type { INTERNAL = 0, EXPORTED = 1, READONLY = 2, INTEGER = 3, EXPORTED_LIST = 4 };

	#pragma endregion

	#pragma region Structures

		typedef struct s_alias	t_alias;
		typedef struct s_cmdp	t_cmdp;
		typedef struct s_var	t_var;

		typedef struct s_alias {
			char	*name;
			char	*value;
			t_alias	*next;
		}	t_alias;

		typedef struct s_cmdp {
			char	*name;
			char	*path;
			int		hits;
			t_cmdp	*next;
		}	t_cmdp;

		typedef struct s_var {
			char	*name;
			char	*value;
			bool	exported;
			bool	readonly;
			bool	integer;
			t_var	*next;
		}	t_var;

	#pragma endregion

	extern t_alias	*alias_table[HASH_SIZE];
	extern t_cmdp	*cmdp_table[HASH_SIZE];
	extern t_var	*vars_table[HASH_SIZE];

#pragma endregion

#pragma region Methods

	//	----------- HASH -----------
	unsigned int	hash_index(const char *key);

	//	========== ALIAS ===========
	//	---------- IMPORT ----------
	int				alias_add(const char *key, const char *value);
	int				alias_validate(char *key, bool show_msg);
	//	---------- EXPORT ----------
	t_alias			*alias_find(const char *key);
	char			**alias_to_array(bool sort);
	int				alias_print(bool sort);
	size_t			alias_length();
	//	---------- DELETE ----------
	int				alias_delete(const char *key);
	void			alias_clear();
	//	-------- INITIALIZE --------
	int				alias_initialize();

	//	=========== CMD ============
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

	//	======== VARIABLES =========
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
	//	---------- OTROS -----------
	int				get_key_value(const char *line, char **key, char **value, char sep);

#pragma endregion
