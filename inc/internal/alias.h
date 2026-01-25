/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 16:22:02 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/25 11:10:47 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include <stddef.h>

#pragma endregion

#pragma region "Variables"

	#pragma region "Structures"

		typedef struct s_alias {
			char			*name;
			char			*value;
			struct s_alias	*next;
		}	t_alias;

	#pragma endregion

#pragma endregion

#pragma region "Methods"

	//	---------- IMPORT ----------
	int		alias_add(const char *key, const char *value);
	int		alias_validate(char *key, int show_msg);
	//	---------- EXPORT ----------
	t_alias	*alias_find(const char *key);
	char	*alias_find_value(const char *key);
	int		alias_print(int sort);
	//	---------- DELETE ----------
	int		alias_delete(const char *key);
	void	alias_clear();
	//	-------- INITIALIZE --------
	int		alias_initialize();

#pragma endregion
