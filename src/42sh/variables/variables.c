/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 17:39:40 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/08 14:34:29 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "42sh.h"

#pragma region Variables

	t_var *main_table[HASH_SIZE];

#pragma endregion

#pragma region Hash

	#pragma region Index

		unsigned int hash_index(const char *key) {
			unsigned int hash = 0;

			//	31 is a commonly used prime number
			while (*key) hash = (hash * 31) + *key++;
			return (hash % HASH_SIZE);
		}

	#pragma endregion

#pragma endregion

#pragma region Import

	#pragma region Variable

		void variables_add(t_var **table, const char *name, const char *value, int exported, int readonly, int integer) {
			if (!name) return;

			t_var *new_var = variables_find(table, name);
			if (new_var) {
				free(new_var->value);
				new_var->value = ft_strdup(value);
				new_var->readonly = readonly;
				new_var->exported = exported;
				new_var->integer = integer;
				return;
			}

			unsigned int index = hash_index(name);
			new_var = safe_malloc(sizeof(t_var));

			new_var->name = ft_strdup(name);
			new_var->value = NULL;
			if (value) new_var->value = ft_strdup(value);
			if (!new_var->name || (value && !new_var->value)) {
				free(new_var->name); free(new_var->value);
				exit_error(NO_MEMORY, 1, NULL, true);
			}

			new_var->readonly = readonly;
			new_var->exported = exported;
			new_var->integer = integer;
			new_var->next = table[index];
			table[index] = new_var;
		}

	#pragma endregion

	#pragma region Array

		void variables_from_array(t_var **table, char **array) {
			if (!array) return;

			for (size_t i = 0; array[i]; i++) {
				char *key = NULL, *value = NULL;
				get_key_value(array[i], &key, &value, '=');
				if (!key) continue;
				variables_add(table, key, value, 1, 0, 0);
				free(key); free(value);
			}
		}

#pragma endregion

	#pragma region Join

		void variables_join(t_var **dst_table, t_var **src_table) {
			for (int index = 0; index < HASH_SIZE; index++) {
				t_var *var = src_table[index];
				while (var) {
					variables_add(dst_table, var->name, var->value, var->exported, var->readonly, var->integer);
					var = var->next;
				}
			}
		}

	#pragma endregion

#pragma endregion

#pragma region Export

	#pragma region Variable

		t_var *variables_find(t_var **table, const char *name) {
			if (!name) return (NULL);

			unsigned int index = hash_index(name);
			t_var *var = table[index];

			while (var) {
				if (!ft_strcmp(var->name, name)) return (var);
				var = var->next;
			}

			return (NULL);
		}

	#pragma endregion

	#pragma region Array

		char **variables_to_array(t_var **table, int type) {
			size_t i = 0;

			for (unsigned int index = 0; index < HASH_SIZE; index++) {
				t_var *var = table[index];
				while (var) {
					if ((type == EXPORTED_LIST && var->exported) || (type == EXPORTED  && var->exported && var->value)) i++;
					if (type == INTERNAL && !var->exported && var->value) i++;
					if (type == READONLY && var->readonly && var->value) i++;
					var = var->next;
				}
			}

			if (i == 0) return (NULL);
			char **array = safe_malloc((i + 1) * sizeof(char *));

			i = 0;
			for (unsigned int index = 0; index < HASH_SIZE; index++) {
				t_var *var = table[index];
				while (var) {
					if (var->value || (!var->value && type == EXPORTED_LIST)) {
						if ((type == EXPORTED_LIST || type == EXPORTED) && !var->exported)	{ var = var->next; continue; }
						if (type == INTERNAL && var->exported) 								{ var = var->next; continue; }
						if (type == READONLY && !var->readonly)								{ var = var->next; continue; }
						size_t len = ft_strlen(var->name) + ft_strlen(var->value) + 2;
						array[i] = malloc(len);
						if (!array[i]) {
							for (size_t j = 0; j < i; j++) free(array[j]);
							free(array); exit_error(NO_MEMORY, 1, NULL, true);
						}

						ft_strcpy(array[i], var->name);
						if (var->value) {
							ft_strcat(array[i], "=");
							ft_strcat(array[i], var->value);
						}

						i++;
					}
					var = var->next;
				}
			} array[i] = NULL;

			array_sort(array);
			return (array);
		}

	#pragma endregion

#pragma endregion

#pragma region Delete

	#pragma region Variable

		int variables_delete(t_var **table, const char *name) {
			if (!name) return (1);

			unsigned int index = hash_index(name);
			t_var *var = table[index];
			t_var *prev = NULL;

			while (var) {
				if (!ft_strcmp(var->name, name)) {
					if (prev)	prev->next = var->next;
					else		table[index] = var->next;
					free(var->name); free(var->value); free(var);
					return (0);
				}
				prev = var;
				var = var->next;
			}
			
			return (1);
		}

	#pragma endregion

	#pragma region Clear

		void variables_clear(t_var **table) {
			for (unsigned int index = 0; index < HASH_SIZE; index++) {
				if (table[index]) {
					t_var *var = table[index];
					while (var) {
						t_var *next = var->next;
						free(var->name);
						free(var->value);
						free(var);
						var = next;
					}
					table[index] = NULL;
				}
			}
		}

	#pragma endregion

#pragma endregion

#pragma region Arrays

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

	#pragma endregion

	#pragma region Print

		void array_print(char **array, bool numbered) {
			if (!array) return;

			for (size_t i = 0; array[i]; i++){
				if (numbered)	printf("[%03zu] %s\n", i, array[i]);
				else			printf("%s\n", array[i]);
			}
		}

	#pragma endregion

	#pragma region Free

		//	Free a char array
		void array_free(char **array) {
			char **tmp_array = array;

			while (array && *array) free(*(array++));
			if (tmp_array)			free(tmp_array);
		}

	#pragma endregion

#pragma endregion
