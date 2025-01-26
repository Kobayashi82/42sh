/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 14:00:37 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/26 14:43:50 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"
	#include "args.h"

#pragma endregion

#pragma region "Merge"

	//	Merge two args lists
	void args_merge(t_arg **args, t_arg **new_args) {
		if (!args || !new_args || !*new_args) return;

		if (*args) {
			if ((*args)->prev) (*args)->prev->next = *new_args;
			t_arg *next = (*args)->next;

			(*new_args)->prev = (*args)->prev;
			*args = *new_args;

			t_arg *current = *new_args;
			while (current->next)
				current = current->next;
			current->next = next;
		} else
			*args = *new_args;

		*new_args = NULL;
	}

#pragma endregion

#pragma region "Sort"

	//	Sort the files lists in alphabetical order
	void args_sort(t_arg **args) {
		t_arg	*current, *tmp;
		char	*temp;

		if (!args || !*args) return ;
		current = *args;
		while (current) {
			tmp = current->next;
			while (tmp) {
				if (ft_strcmp(current->value, tmp->value) > 0) {
					temp = current->value;
					current->value = tmp->value;
					tmp->value = temp;
				} tmp = tmp->next;
			} current = current->next;
		}
	}

#pragma endregion

#pragma region "Clear"

	//	Clears all the args in an args list
	void args_clear(t_arg **args) {
		t_arg *next, *current = *args;

		while (current) {
			next = current->next;
			sfree(current->value);
			sfree(current);
			current = next;
		} *args = NULL;
	}

#pragma endregion
