/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 14:00:37 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/18 23:41:53 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "utils/libft.h"
	#include "terminal/terminal.h"
	#include "terminal/print.h"
	#include "parser/tokenizer/args.h"

	#include <stdio.h>

#pragma endregion

#pragma region "Insert"

	//	Insert a list into another, replacing the current node
	int args_insert(t_arg *args, t_arg **new_args) {
		if (!args || !new_args || !*new_args) return (1);

		t_arg *current = *new_args;
		while (current->next)
			current = current->next;
		current->next = args->next;

		args_copy(args, *new_args);
		args->next = (*new_args)->next;

		free(*new_args);
		*new_args = NULL;
	
		return (0);
	}

#pragma endregion

#pragma region "Merge"

	//	Merge two args lists
	void args_merge(t_arg **args, t_arg **new_args) {
		if (!args || !new_args || !*new_args) return;

		if (*args) {
			t_arg *current = *args;
			while (current->next)
				current = current->next;
			current->next = *new_args;
			(*new_args)->prev = current;
		} else
			*args = *new_args;

		*new_args = NULL;
	}

#pragma endregion

#pragma region "Copy"

	int args_copy(t_arg *dst, t_arg *src) {
		if (!dst || !src) return (1);

		free(dst->value);

		dst->value = src->value;

		return (0);
	}

#pragma endregion

#pragma region "Swap"

	int args_swap(t_arg *arg1, t_arg *arg2) {
		if (!arg1 || !arg2) return (1);

		char *value = arg1->value;
		arg1->value = arg2->value;

		arg2->value = value;

		return (0);
	}

#pragma endregion

#pragma region "Sort"

	// t_arg *args_sort(t_arg *args) {
	// 	if (!args) return (NULL);

	// 	t_arg *current = args;
	// 	while (current) {
	// 		t_arg *tmp = current->next;
	// 		while (tmp) {
	// 			if (strcmp(current->value, tmp->value) > 0)
	// 				args_swap(current, tmp);
	// 			tmp = tmp->next;
	// 		}
	// 		current = current->next;
	// 	}

	// 	return (args);
	// }

t_arg *args_merge2(t_arg *left, t_arg *right) {
	if (!left) return right;
	if (!right) return left;

	if (strcmp(left->value, right->value) <= 0) {
		left->next = args_merge2(left->next, right);
		if (left->next) left->next->prev = left;
		left->prev = NULL;
		return left;
	} else {
		right->next = args_merge2(left, right->next);
		if (right->next) right->next->prev = right;
		right->prev = NULL;
		return right;
	}
}

t_arg *args_split(t_arg *head) {
	t_arg *fast = head, *slow = head;

	// Encuentra el punto medio con un puntero lento y rápido
	while (fast->next && fast->next->next) {
		slow = slow->next;
		fast = fast->next->next;
	}

	t_arg *mid = slow->next;
	slow->next = NULL; // Divide la lista en dos mitades
	if (mid) mid->prev = NULL;

	return mid;
}

t_arg *args_sort(t_arg *args) {
	if (!args || !args->next) return args;

	// Divide la lista en dos mitades
	t_arg *mid = args_split(args);

	// Ordena recursivamente las dos mitades
	t_arg *left = args_sort(args);
	t_arg *right = args_sort(mid);

	// Fusiona las dos mitades ordenadas
	return args_merge2(left, right);
}


#pragma endregion

#pragma region "Clear"

	//	Clears all the args in an args list
	void args_clear(t_arg **args) {
		if (!args || !*args) return;

		t_arg *next, *current = *args;

		while (current) {
			next = current->next;
			free(current->value);
			free(current);
			current = next;
		} *args = NULL;
	}

#pragma endregion

#pragma region "Print"

	//	Clears all the args in an args list
	// void args_print(t_arg *args) {
	// 	if (!args) return;
	// 	t_arg *current = args;

	// 	while (current) {
	// 		printf("%s", current->value);
	// 		current = current->next;
	// 		if (current)	printf(" ");
	// 		else			printf("\n");
	// 	}
	// }

	//	Clears all the args in an args list
	void args_print(t_arg *args) {
		if (!args) return;
		t_arg *current = args;

		print(STDOUT_FILENO, NULL, RESET);
		while (current) {
			print(STDOUT_FILENO, current->value, JOIN);
			current = current->next;
			if (current)	print(STDOUT_FILENO, " ", JOIN);
			else			print(STDOUT_FILENO, "\n", PRINT);
		}
	}

#pragma endregion
