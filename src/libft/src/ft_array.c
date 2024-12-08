/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_array.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 15:48:12 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/08 16:07:56 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

#pragma region Sort

	void array_sort(char **array) {
		if (!array) return;

		size_t count = 0;
		while (array[count]) count++;

		for (size_t i = 0; i < count - 1; i++) {
			for (size_t j = 0; j < count - i - 1; j++) {
				if (ft_strcmp(array[j], array[j + 1]) > 0) {
					char *temp = array[j];
					array[j] = array[j + 1];
					array[j + 1] = temp;
				}
			}
		}
	}

	void array_int_sort(int *array) {
		if (!array) return;

		size_t count = 0;
		while (array[count]) count++;

		for (size_t i = 0; i < count - 1; i++) {
			for (size_t j = 0; j < count - i - 1; j++) {
				if (array[j] > array[j + 1]) {
					int temp = array[j];
					array[j] = array[j + 1];
					array[j + 1] = temp;
				}
			}
		}
	}

#pragma endregion

#pragma region Print

	void array_print(char **array, int numbered) {
		if (!array) return;

		for (size_t i = 0; array[i]; i++) {
			if (numbered)	printf("[%03zu] %s\n", i, array[i]);
			else			printf("%s\n", array[i]);
		}
	}

	void array_int_print(int *array) {
		if (!array) return;

		for (size_t i = 0; array[i]; i++)
			printf("%d\n", array[i]);
	}

#pragma endregion

#pragma region Free

	void array_free(char **array) {
		if (!array) return;
		char **tmp_array = array;

		while (tmp_array && *tmp_array) { free(*(tmp_array)); *(tmp_array++) = NULL; }
		if (array)			free(array);
	}

	void array_free_ptr(char ***array) {
		if (!array || !*array) return;
		char **tmp_array = *array;

		while (tmp_array && *tmp_array) { free(*(tmp_array)); *(tmp_array++) = NULL; }
		if (*array) free(*array);
		*array = NULL;
	}

#pragma endregion
