/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 19:11:28 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/07 22:05:40 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define HASH_SIZE 101	//	Using a prime number improves the distribution in the hash table

enum e__var_type { INTERNAL, EXPORTED, EXPORTED_LIST, READONLY };

typedef struct s_var t_var;
typedef struct s_var {
	char	*name;
	char	*value;
	bool	readonly;
	bool	exported;
	bool	integer;
	t_var	*next;
}	t_var;

extern t_var *main_table[HASH_SIZE];

unsigned int	hash_index(const char *key);
void			variable_add(t_var **table, const char *name, const char *value, int exported, int readonly, int integer);
t_var			*variable_find(t_var **table, const char *name);
int				variable_delete(t_var **table, const char *name);
void			variable_clear(t_var **table);
void			variables_add_array(t_var **table, char **array);
char			**variables_to_array(t_var **table, int type);

void			sort_array(char **array);
int				get_key_value(char *line, char **key, char **value, char sep);
//void	variable_list(int type);
