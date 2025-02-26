/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 16:22:02 by vzurera-          #+#    #+#             */
/*   Updated: 2025/02/26 17:16:18 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include <stddef.h>
	#include <stdbool.h>

	#define ALIAS_HASH_SIZE 101

#pragma endregion

#pragma region "Variables"

	#pragma region "Structures"

		typedef struct s_alias	t_alias;
		typedef struct s_alias {
			char	*name;
			char	*value;
			t_alias	*next;
		}	t_alias;

	#pragma endregion

	extern t_alias	*alias_table[ALIAS_HASH_SIZE];

#pragma endregion

#pragma region "Methods"

	//	---------- IMPORT ----------
	int				alias_add(const char *key, const char *value);
	int				alias_validate(char *key, bool show_msg);
	//	---------- EXPORT ----------
	t_alias			*alias_find(const char *key);
	char			*alias_find_value(const char *key);
	char			**alias_to_array(bool sort);
	int				alias_print(bool sort);
	size_t			alias_length();
	//	---------- DELETE ----------
	int				alias_delete(const char *key);
	void			alias_clear();
	//	-------- INITIALIZE --------
	int				alias_initialize();

#pragma endregion
