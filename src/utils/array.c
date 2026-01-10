/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 15:48:12 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/10 20:51:02 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "utils/utils.h"

#pragma endregion

#pragma region "Sort"

	// Sorts an array of strings alphabetically, ignoring the first 'skip' characters when comparing
	void array_nsort(char **array, int sort, size_t skip) {
		if (!array || !*array || sort == SORT_NONE) return;
		if (skip < 0) skip = 0;

		size_t count = 0;
		while (array[count]) count++;

		for (size_t i = 0; i < count - 1; i++) {
			for (size_t j = 0; j < count - i - 1; j++) {
				const char *str1 = array[j];
				const char *str2 = array[j + 1];

				if (ft_strlen(str1) > skip) str1 += skip;
				if (ft_strlen(str2) > skip) str2 += skip;

				int comparison = strcmp(str1, str2);
				if ((sort == SORT_NORMAL && comparison > 0) || (sort == SORT_REVERSE && comparison < 0)) {
					char *temp = array[j];
					array[j] = array[j + 1];
					array[j + 1] = temp;
				}
			}
		}
	}

	// Sorts an array of strings alphabetically
	void array_sort(char **array, int sort) { array_nsort(array, sort, 0); }

	// Sorts an array of numbers from smallest to largest
	void array_int_sort(int *array, int sort) {
		if (!array ||sort == SORT_NONE) return;

		size_t count = 0;
		while (array[count]) count++;

		for (size_t i = 0; i < count - 1; i++) {
			for (size_t j = 0; j < count - i - 1; j++) {
				int should_swap = SORT_REVERSE ? (array[j] < array[j + 1]) : (array[j] > array[j + 1]);
				if (should_swap) {
					int temp = array[j];
					array[j] = array[j + 1];
					array[j + 1] = temp;
				}
			}
		}
	}

#pragma endregion

#pragma region "Clone"

	char **array_clone(int length, const char **array) {
		if (length <= 0 || !array) return (NULL);

		char **new_array = malloc((length + 1) * sizeof(char *));
		if (!new_array) return (NULL);
		
		int i = 0;
		for (; i < length; ++i) {
			new_array[i] = ft_strdup(array[i]);
			if (!new_array[i]) {
				array_free(new_array);
				return (NULL);
			}
		}
		new_array[i] = NULL;

		return (new_array);
	}

#pragma endregion

#pragma region "Print"

	void array_print(const char **array, int fd, int numbered) {
		if (!array) return;

		print(fd, NULL, RESET);

		for (int i = 0; array[i]; ++i) {
			if (numbered) print(fd, ft_strjoin(ft_itoa(i), " ", 1), FREE_JOIN);
			print(fd, ft_strjoin(array[i], "\n", 0),                FREE_JOIN);
		}

		print(fd, NULL, PRINT);
	}

	void array_int_print(int *array, int fd) {
		if (!array) return;

		print(fd, NULL, RESET);

		for (int i = 0; array[i]; ++i) {
			print(fd, ft_itoa(array[i]), FREE_JOIN);
			print(fd, "\n",              JOIN);
		}

		print(fd, NULL, PRINT);
	}

#pragma endregion

#pragma region "Free"

	void array_free(char **array) {
		if (!array) return;

		for (int i = 0; array[i]; ++i)
			free(array[i]);
		free(array);
	}

#pragma endregion
