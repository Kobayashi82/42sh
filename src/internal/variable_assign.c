/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_assign.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 20:46:40 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/15 22:46:09 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/utils.h"

typedef struct s_assign {
	char			*key;
	char			*index;
	char			*value;
	int				append;
	struct s_assign	*next;
} t_assign;

void free_assign(t_assign *assign) {
	while (assign) {
		t_assign *current = assign;
		assign = assign->next;
		free(current->key);
		free(current->index);
		free(current->value);
		free(current);
	}
}

t_assign *parse_assign(const char *str) {
	int key_end = 0, index_start = 0, index_end = 0;
	int i = 0, is_index = 0;

	for (; str[i]; ++i) {
		if (str[i] == '=' && is_index < 1)  { key_end = i;		break;					}
		if (str[i] == '[' && is_index == 0)	{ is_index = 1;		index_start = i + 1;	}
		if (str[i] == ']' && is_index == 1)	{ is_index = -1;	index_end   = i;		}
	}

	char *key = NULL;
	char *index = NULL;
	char *value = NULL;

	if (!key_end && str[0] != '=') {
		key = ft_strdup(str);
	} else {
		if (index_start > 0 && index_end > 0 && str[index_end + 1]) {
			if ((str[index_end + 1] == '+' && str[index_end + 2] == '=') || str[index_end + 1] == '=') {
				index = ft_substr(str, index_start, index_end - index_start);
			}
		}

		key = (index) ? ft_strndup(str, index_start - 1) : ft_strndup(str, key_end);
		value = ft_strdup(str + key_end + 1);
	}

	t_assign *node = malloc(sizeof(t_assign));
	if (!node) {
		free(key);
		free(index);
		free(value);
		return (NULL);
	}
	node->key   = key;
	node->index = index;
	node->value = value;
	node->append = 0;
	node->next = NULL;
	if (ft_strlen(node->key) > 1 && node->key[ft_strlen(node->key) - 1] == '+') {
		node->key[ft_strlen(node->key) - 1] = '\0';
		node->append = 1;
	}

	if (node->value && *node->value == '(') {
		if (index) {
			free(node->key);
			free(node->index);
			free(node->value);
			free(node);
			// Error: cannot assign list to array member
			return (NULL);
		}
		// node->next = create_sub_array();	// crear subnodos con mismo key
	}

	return (node);
}
