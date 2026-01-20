/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 17:39:40 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/20 23:24:04 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "terminal/terminal.h"
	#include "terminal/readinput/history.h"
	#include "main/shell.h"
	#include "utils/utils.h"
	
	#define MAX_REFERENCES	100

#pragma endregion

#pragma region "Common"

	#pragma region "Specials"

		char *specials_get(t_env *env, const char *key) {
			if (!env || !key || env->sourced != SRC_SHELL) return (NULL);

			static char special[4096];

			if (!strcmp(key, "DIRSTACK")) {
				return (shell.dirs.cwd);
			}

			// History
			if (!strcmp(key, "42_HISTCMD")) {
				snprintf(special, sizeof(special), "%zu", history_histcmd());
				return (special);
			}

			// if (!strcmp(key, "42_HISTFILE"))			history_file_set(value);
			// if (!strcmp(key, "42_HISTSIZE"))			history_size_set(atol(value), HIST_MEM);
			// if (!strcmp(key, "42_HISTFILESIZE"))		history_size_set(atol(value), HIST_FILE);
			// if (!strcmp(key, "42_HISTTIMEFORMAT"))	history_hist_timeformat_set(value);
			// if (!strcmp(key, "42_HISTCONTROL"))		history_hist_control_set(value);
			// if (!strcmp(key, "42_HISTIGNORE"))		history_hist_ignore_set(value);

			return (NULL);
		}

		void specials_set(t_env *env, const char *key, const char *value) {
			if (!env || !key || env->sourced != SRC_SHELL) return;

			// History
			if (!strcmp(key, "42_HISTFILE"))		history_file_set(value);
			if (!strcmp(key, "42_HISTSIZE"))		history_size_set(atol(value), HIST_MEM);
			if (!strcmp(key, "42_HISTFILESIZE"))	history_size_set(atol(value), HIST_FILE);
			if (!strcmp(key, "42_HISTTIMEFORMAT"))	history_hist_timeformat_set(value);
			if (!strcmp(key, "42_HISTCONTROL"))		history_hist_control_set(value);
			if (!strcmp(key, "42_HISTIGNORE"))		history_hist_ignore_set(value);
		}

		void specials_unset(t_env *env, const char *key) {
			if (!env || !key || env->sourced != SRC_SHELL) return;

			// History
			if (!strcmp(key, "42_HISTFILE"))		history_file_set(NULL);
			if (!strcmp(key, "42_HISTSIZE"))		history_size_set(1000, HIST_MEM);
			if (!strcmp(key, "42_HISTFILESIZE"))	history_size_set(2000, HIST_FILE);
			if (!strcmp(key, "42_HISTTIMEFORMAT"))	history_hist_timeformat_set(NULL);
			if (!strcmp(key, "42_HISTIGNORE"))		history_hist_ignore_set(NULL);
		}

	#pragma endregion

	#pragma region "Index"

		static unsigned int hash_index(const char *key) {
			if (!key) return (errno = E_VAR_INVALID_INDEX, 0);

			unsigned int hash = 0;

			while (*key) hash = (hash * 31) + *key++;

			return (hash % HASH_SIZE);
		}

	#pragma endregion

	#pragma region "Validate"

		int variable_validate(const char *key) {
			if (!key) return (0);

			int		ret = 0;
			size_t	len = ft_strlen(key);

			if (!len || (!isalpha(key[0]) && key[0] != '_'))	ret = 1;
			if (!strcmp(key, "42_HISTFILE"))					ret = 0;
			if (!strcmp(key, "42_HISTSIZE"))					ret = 0;
			if (!strcmp(key, "42_HISTFILESIZE"))				ret = 0;
			if (!strcmp(key, "42_HISTTIMEFORMAT"))				ret = 0;
			if (!strcmp(key, "42_HISTCONTROL"))					ret = 0;
			if (!strcmp(key, "42_HISTIGNORE"))					ret = 0;
			if (!strcmp(key, "42_SH"))							ret = 0;
			if (!strcmp(key, "42_SUBSHELL"))					ret = 0;
			if (!strcmp(key, "42_VERSION"))						ret = 0;
			if (!strcmp(key, "42_PID"))							ret = 0;

			for (size_t i = 1; i < len; ++i) {
				if (!isalnum(key[i]) && key[i] != '_') {		ret = 1;		break; }
			}

			return (ret);
		}

	#pragma endregion

	#pragma region "Find"

		// Find variable by 'key'
		static t_var *variable_find(t_env *env, const char *key) {
			if (!env || !key) return (NULL);

			// Check specials first
			static t_var special;
			special.data.scalar = specials_get(env, key);
			if (special.data.scalar) {
				special.key = (char *)key;
				special.flags = VAR_READONLY;
				return (&special);
			}

			// Search in current environment and parents
			while (env) {
				t_var *var = env->table[hash_index(key)];
				while (var) {
					if (!strcmp(var->key, key)) return (var);
					var = var->next;
				}
				env = env->parent;
			}

			return (NULL);
		}

	#pragma endregion

	#pragma region "Get"

		// Find variable by 'key' (support referenced variables)
		t_var *variable_get(t_env *env, const char *key, int reference) {
			if (!env || !key) return (NULL);

			errno = 0;
			int count = 0;
			if (variable_validate(key)) {
				errno = E_VAR_IDENTIFIER;
				return (NULL);
			}

			t_var *var = variable_find(env, key);
			if (reference && var && var->flags & VAR_REFERENCE) {
				t_var*visited[HASH_SIZE] = {NULL};
				t_var *visited_var = malloc(sizeof(t_var));
				char *key_copy = ft_strdup(key);
				if (!key_copy || !visited_var) {
					free(key_copy);
					free(visited_var);
					variable_clear_table(visited);
					errno = E_NO_MEMORY;
					return (NULL);
				}
				visited_var->key = key_copy;
				unsigned int hash = hash_index(key_copy);
				visited_var->next = visited[hash];
				visited[hash] = visited_var;
				count = 1;
				while (count < MAX_REFERENCES) {
					char *target = var->data.scalar;

					// Check target in visited
					t_var *check_var = visited[hash_index(target)];
					while (check_var) {
						if (!strcmp(check_var->key, target)) {
							variable_clear_table(visited);
							errno = E_VAR_CYCLE_REFERENCE;
							return (NULL);
						}
						check_var = check_var->next;
					}

					// Find target
					var = variable_find(env, target);
					if (!var || !(var->flags & VAR_REFERENCE)) {
						variable_clear_table(visited);
						break;
					}

					// Add target to visited
					visited_var = malloc(sizeof(t_var));
					char *target_copy = ft_strdup(target);
					if (!target_copy || !visited_var) {
						free(target_copy);
						free(visited_var);
						variable_clear_table(visited);
						errno = E_NO_MEMORY;
						return (NULL);
					}
					visited_var->key = target_copy;
					unsigned int hash = hash_index(target_copy);
					visited_var->next = visited[hash];
					visited[hash] = visited_var;
					count++;
				}

				if (count == MAX_REFERENCES) {
					variable_clear_table(visited);
					errno = E_VAR_MAX_REFERENCES;
					return (NULL);
				}
			}

			return (var);
		}

	#pragma endregion

#pragma endregion

#pragma region "Types"

	#pragma region "Scalar"

		#pragma region "Get"

			char *variable_scalar_get(t_env *env, const char *key) {
				if (!env || !key) return (NULL);

				t_var *var = variable_get(env, key, 1);
				if (!var) return (NULL);

				if (var->flags & (VAR_ARRAY | VAR_ASSOCIATIVE)) return (errno = E_VAR_INVALID_TYPE, NULL);

				return (var->data.scalar);
			}

		#pragma endregion

		#pragma region "Set"

			int variable_scalar_set(t_env *env, const char *key, const char *value, int append, int type, int local) {
				if (!env || !key) return (0);

				type &= (VAR_EXPORTED | VAR_READONLY | VAR_INTEGER);

				errno = 0;
				t_var *var = (local) ? NULL : variable_get(env, key, 1);
				if (errno) return (errno);

				// If not found or local, search only in current environment
				if (!var) {
					t_var *local_var = env->table[hash_index(key)];
					while (local_var) {
						if (!strcmp(local_var->key, key)) {
							var = local_var;
							break;
						}
						local_var = local_var->next;
					}
				}

				if (var && (var->flags & (VAR_ARRAY | VAR_ASSOCIATIVE)))	return (errno = E_VAR_INVALID_TYPE, E_VAR_INVALID_TYPE);
				if (var && (var->flags & VAR_READONLY))						return (errno = E_VAR_READONLY, E_VAR_READONLY);

				// If doesn't exist, create new variable
				if (!var) {
					var = calloc(1, sizeof(t_var));
					if (!var) return (errno = E_NO_MEMORY, E_NO_MEMORY);

					var->key = ft_strdup(key);
					if (!var->key) {
						free(var);
						return (errno = E_NO_MEMORY, E_NO_MEMORY);
					}

					if (value) {
						var->data.scalar = ft_strdup(value);
						if (!var->data.scalar) {
							free(var->key);
							free(var);
							return (errno = E_NO_MEMORY, E_NO_MEMORY);
						}
					}

					var->flags = type;

					unsigned int hash = hash_index(key);
					var->next = env->table[hash];
					env->table[hash] = var;

					return (0);
				}

				// Variable exists, update value
				if (append) {
					char *new_value = ft_strjoin(var->data.scalar, value, J_FREE_NONE);
					if (!new_value) return (errno = E_NO_MEMORY, E_NO_MEMORY);
					free(var->data.scalar);
					var->data.scalar = new_value;
				} else {
					char *new_value = ft_strdup(value);
					if (!new_value) return (errno = E_NO_MEMORY, E_NO_MEMORY);
					free(var->data.scalar);
					var->data.scalar = new_value;
				}

				var->flags |= type;

				specials_set(env, key, value);

				return (0);
			}

		#pragma endregion

	#pragma endregion

	#pragma region "Array"

		#pragma region "Get"

			char *variable_array_get(t_env *env, const char *key, int index) {
				if (!env || !key)	return (NULL);
				if (index < 0)		return (errno = E_VAR_INVALID_INDEX, NULL);

				t_var *var = variable_get(env, key, 1);
				if (!var) return (NULL);

				// If scalar and requesting index 0, return the scalar
				if (!(var->flags & (VAR_ARRAY | VAR_ASSOCIATIVE)) && !index)	return (var->data.scalar);
				if (!(var->flags & VAR_ARRAY))									return (errno = E_VAR_INVALID_TYPE, NULL);

				// Search in array hash
				char idx_str[32];
				snprintf(idx_str, sizeof(idx_str), "%d", index);

				t_var *elem = var->data.array[hash_index(idx_str)];
				while (elem) {
					if (!strcmp(elem->key, idx_str)) return (elem->data.scalar);
					elem = elem->next;
				}

				return (NULL);
			}


		#pragma endregion

		#pragma region "Set"

			int variable_array_set(t_env *env, const char *key, int index, const char *value, int append, int local) {
				if (!env || !key)	return (0);
				if (index < 0)		return (errno = E_VAR_INVALID_INDEX, E_VAR_INVALID_INDEX);

				errno = 0;
				t_var *var = (local) ? NULL : variable_get(env, key, 1);
				if (errno) return (errno);

				// If not found or local, search only in current environment
				if (!var) {
					t_var *local_var = env->table[hash_index(key)];
					while (local_var) {
						if (!strcmp(local_var->key, key)) {
							var = local_var;
							break;
						}
						local_var = local_var->next;
					}
				}

				if (var && (var->flags & VAR_ASSOCIATIVE))	return (errno = E_VAR_INVALID_TYPE, E_VAR_INVALID_TYPE);
				if (var && (var->flags & VAR_READONLY))		return (errno = E_VAR_READONLY, E_VAR_READONLY);

				// If doesn't exist, create new array variable
				if (!var) {
					var = calloc(1, sizeof(t_var));
					if (!var) return (errno = E_NO_MEMORY, E_NO_MEMORY);

					char *new_key = ft_strdup(key);
					if (!new_key) {
						free(var);
						return (errno = E_NO_MEMORY, E_NO_MEMORY);
					}
					var->key = new_key;
					var->flags = VAR_ARRAY;
					var->data.array = calloc(HASH_SIZE, sizeof(t_var *));
					if (!var->data.array) {
						free(var->key);
						free(var);
						return (errno = E_NO_MEMORY, E_NO_MEMORY);
					}

					// Insert into current environment
					unsigned int hash = hash_index(key);
					var->next = env->table[hash];
					env->table[hash] = var;
				}

				// If scalar, convert to array
				if (!(var->flags & VAR_ARRAY)) {
					char *old_value = var->data.scalar;
					var->data.array = calloc(HASH_SIZE, sizeof(t_var *));
					if (!var->data.array) return (errno = E_NO_MEMORY, E_NO_MEMORY);

					// Save old value at index 0
					if (old_value) {
						t_var *elem = calloc(1, sizeof(t_var));
						if (!elem) {
							free(var->data.array);
							var->data.scalar = old_value;
							return (errno = E_NO_MEMORY, E_NO_MEMORY);
						}
						char *new_elem_key = ft_strdup("0");
						if (!new_elem_key) {
							free(elem);
							free(var->data.array);
							var->data.scalar = old_value;
							return (errno = E_NO_MEMORY, E_NO_MEMORY);
						}
						elem->key = new_elem_key;
						elem->data.scalar = old_value;
						elem->flags = 0;
						var->data.array[hash_index("0")] = elem;
					}

					var->flags |= VAR_ARRAY;
				}

				// Search if element already exists
				char idx_str[32];
				snprintf(idx_str, sizeof(idx_str), "%d", index);

				unsigned int hash = hash_index(idx_str);
				t_var *elem = var->data.array[hash];

				while (elem) {
					if (!strcmp(elem->key, idx_str)) {
						if (append) {
							char *new_value = ft_strjoin(elem->data.scalar, value, J_FREE_NONE);
							if (!new_value) return (errno = E_NO_MEMORY, E_NO_MEMORY);
							free(elem->data.scalar);
							elem->data.scalar = new_value;
						} else {
							char *new_value = ft_strdup(value);
							if (!new_value) return (errno = E_NO_MEMORY, E_NO_MEMORY);
							free(elem->data.scalar);
							elem->data.scalar = new_value;
						}
						return (0);
					}
					elem = elem->next;
				}
				
				// Create new element
				elem = calloc(1, sizeof(t_var));
				if (!elem) return (errno = E_NO_MEMORY, E_NO_MEMORY);

				char *new_elem_key = ft_strdup(idx_str);
				if (!new_elem_key) {
					free(elem);
					return (errno = E_NO_MEMORY, E_NO_MEMORY);
				}
				elem->key = new_elem_key;
				char *new_elem_value = ft_strdup(value);
				if (!new_elem_value) {
					free(elem->key);
					free(elem);
					return (errno = E_NO_MEMORY, E_NO_MEMORY);
				}
				elem->data.scalar = new_elem_value;
				elem->flags = 0;
				elem->next = var->data.array[hash];
				var->data.array[hash] = elem;

				return (0);
			}

		#pragma endregion

		#pragma region "Remove"

			int variable_array_remove(t_env *env, const char *key, int index) {
				if (!env || !key)	return (0);
				if (index < 0)		return (errno = E_VAR_INVALID_INDEX, E_VAR_INVALID_INDEX);

				t_var *var = variable_get(env, key, 1);
				if (!var || errno) return (errno);

				if (!(var->flags & VAR_ARRAY)) return (errno = E_VAR_INVALID_TYPE, E_VAR_INVALID_TYPE);

				char idx_str[32];
				snprintf(idx_str, sizeof(idx_str), "%d", index);

				unsigned int hash = hash_index(idx_str);
				t_var *elem = var->data.array[hash];
				t_var *prev = NULL;

				while (elem) {
					if (!strcmp(elem->key, idx_str)) {
						if (prev)	prev->next = elem->next;
						else		var->data.array[hash] = elem->next;

						free(elem->key);
						free(elem->data.scalar);
						free(elem);
						return (0);
					}
					prev = elem;
					elem = elem->next;
				}

				return (0);
			}

		#pragma endregion

		#pragma region "Values"

			// Format array values for display: ([0]="val1" [3]="val2")
			static char *format_array_values(t_var *var) {
				if (!var)								return (NULL);
				if (!(var->flags & VAR_ASSOCIATIVE))	return (errno = E_VAR_INVALID_TYPE, NULL);

				// First pass: count elements
				int count = 0;
				for (int i = 0; i < HASH_SIZE; ++i) {
					t_var *elem = var->data.array[i];
					while (elem) {
						count++;
						elem = elem->next;
					}
				}

				if (!count) {
					char *result = ft_strdup("()");
					if (!result) return (errno = E_NO_MEMORY, result);
				}

				// Allocate exact space needed
				t_var **elements = malloc(count * sizeof(t_var *));
				if (!elements) return (errno = E_NO_MEMORY, NULL);

				// Second pass: collect elements
				int index = 0;
				for (int i = 0; i < HASH_SIZE; ++i) {
					t_var *elem = var->data.array[i];
					while (elem) {
						elements[index++] = elem;
						elem = elem->next;
					}
				}

				// Sort by numeric index
				for (int i = 0; i < count - 1; ++i) {
					for (int j = 0; j < count - i - 1; ++j) {
						if (atoi(elements[j]->key) > atoi(elements[j + 1]->key)) {
							t_var *temp = elements[j];
							elements[j] = elements[j + 1];
							elements[j + 1] = temp;
						}
					}
				}

				// Build result string: ([0]="val1" [3]="val2")
				char *result = ft_strdup("(");
				if (!result) {
					free(elements);
					return (errno = E_NO_MEMORY, NULL);
				}

				for (int i = 0; i < count; ++i) {
					char buffer[4096];
					char *formatted = format_for_shell(elements[i]->data.scalar, '\"');
					snprintf(buffer, sizeof(buffer), "%s[%s]=\"%s\"", (i > 0) ? " " : "", elements[i]->key, formatted);
					free(formatted);

					// Concatenate to result
					result = ft_strjoin(result, buffer, J_FREE_VAL_1);
					if (!result) {
						free(elements);
						return (errno = E_NO_MEMORY, NULL);
					}
				}

				// Add closing parenthesis
				result = ft_strjoin(result, ")", J_FREE_VAL_1);
				if (!result) errno = E_NO_MEMORY;
				free(elements);

				return (result);
			}

			char *variable_array_values(t_env *env, const char *key) {
				if (!env || !key) return (NULL);

				t_var *var = variable_get(env, key, 1);
				if (!var) return (NULL);

				if (!(var->flags & VAR_ARRAY)) return (errno = E_VAR_INVALID_TYPE, NULL);

				return (format_array_values(var));
			}

		#pragma endregion

	#pragma endregion

	#pragma region "Associative"

		#pragma region "Get"

			char *variable_assoc_get(t_env *env, const char *key, const char *assoc_key) {
				if (!env || !key)				return (NULL);
				if (!assoc_key || !*assoc_key)	return (errno = E_VAR_INVALID_INDEX, NULL);

				t_var *var = variable_get(env, key, 1);
				if (!var) return (NULL);

				if (!(var->flags & VAR_ASSOCIATIVE)) return (errno = E_VAR_INVALID_INDEX, NULL);

				// Search in assoc hash
				t_var *elem = var->data.array[hash_index(assoc_key)];
				while (elem) {
					if (!strcmp(elem->key, assoc_key)) return (elem->data.scalar);
					elem = elem->next;
				}

				return (NULL);
			}

		#pragma endregion

		#pragma region "Set"

			int variable_assoc_set(t_env *env, const char *key, const char *assoc_key, const char *value, int append, int local) {
				if (!env || !key )				return (0);
				if (!assoc_key || !*assoc_key)	return (errno = E_VAR_INVALID_INDEX, E_VAR_INVALID_INDEX);

				errno = 0;
				t_var *var = (local) ? NULL : variable_get(env, key, 1);
				if (errno) return (errno);

				// If not found or local, search only in current environment
				if (!var) {
					t_var *local_var = env->table[hash_index(key)];
					while (local_var) {
						if (!strcmp(local_var->key, key)) {
							var = local_var;
							break;
						}
						local_var = local_var->next;
					}
				}

				if (var && !(var->flags & VAR_ASSOCIATIVE))	return (errno = E_VAR_INVALID_TYPE, E_VAR_INVALID_TYPE);
				if (var && (var->flags & VAR_READONLY))		return (errno = E_VAR_READONLY, E_VAR_READONLY);

				// If doesn't exist, create new associative variable
				if (!var) {
					var = calloc(1, sizeof(t_var));
					if (!var) return (errno = E_NO_MEMORY, E_NO_MEMORY);

					char *new_key = ft_strdup(key);
					if (!new_key) {
						free(var);
						return (errno = E_NO_MEMORY, E_NO_MEMORY);
					}
					var->key = new_key;
					var->flags = VAR_ASSOCIATIVE;
					var->data.array = calloc(HASH_SIZE, sizeof(t_var *));
					if (!var->data.array) {
						free(var->key);
						free(var);
						return (errno = E_NO_MEMORY, E_NO_MEMORY);
					}

					// Insert into current environment
					unsigned int hash = hash_index(key);
					var->next = env->table[hash];
					env->table[hash] = var;
				}

				unsigned int hash = hash_index(assoc_key);
				t_var *elem = var->data.array[hash];

				while (elem) {
					if (!strcmp(elem->key, assoc_key)) {
						if (append) {
							char *new_value = ft_strjoin(elem->data.scalar, value, J_FREE_NONE);
							if (!new_value) return (errno = E_NO_MEMORY, E_NO_MEMORY);
							free(elem->data.scalar);
							elem->data.scalar = new_value;
						} else {
							char *new_value = ft_strdup(value);
							if (!new_value) return (errno = E_NO_MEMORY, E_NO_MEMORY);
							free(elem->data.scalar);
							elem->data.scalar = new_value;
						}
						return (0);
					}
					elem = elem->next;
				}

				// Create new element
				elem = calloc(1, sizeof(t_var));
				if (!elem) return (errno = E_NO_MEMORY, E_NO_MEMORY);

				char *new_elem_key = ft_strdup(assoc_key);
				if (!new_elem_key) {
					free(elem);
					return (errno = E_NO_MEMORY, E_NO_MEMORY);
				}
				elem->key = new_elem_key;
				char *new_elem_value = ft_strdup(value);
				if (!new_elem_value) {
					free(elem->key);
					free(elem);
					return (errno = E_NO_MEMORY, E_NO_MEMORY);
				}
				elem->data.scalar = new_elem_value;
				elem->flags = 0;
				elem->next = var->data.array[hash];
				var->data.array[hash] = elem;

				return (0);
			}

		#pragma endregion

		#pragma region "Remove"

			int variable_assoc_remove(t_env *env, const char *key, const char *assoc_key) {
				if (!env || !key )				return (0);
				if (!assoc_key || !*assoc_key)	return (errno = E_VAR_INVALID_INDEX, E_VAR_INVALID_INDEX);

				t_var *var = variable_get(env, key, 1);
				if (!var || errno) return (errno);

				if (!(var->flags & VAR_ASSOCIATIVE)) return (errno = E_VAR_INVALID_TYPE, E_VAR_INVALID_TYPE);

				unsigned int hash = hash_index(assoc_key);
				t_var *elem = var->data.array[hash];
				t_var *prev = NULL;

				while (elem) {
					if (!strcmp(elem->key, assoc_key)) {
						if (prev)	prev->next = elem->next;
						else		var->data.array[hash] = elem->next;

						free(elem->key);
						free(elem->data.scalar);
						free(elem);
						return (0);
					}
					prev = elem;
					elem = elem->next;
				}

				return (0);
			}

		#pragma endregion

		#pragma region "Values"

			// Format associative array values for display: ([key1]="val1" [key2]="val2")
			static char *format_assoc_values(t_var *var) {
				if (!var)								return (NULL);
				if (!(var->flags & VAR_ASSOCIATIVE))	return (errno = E_VAR_INVALID_TYPE, NULL);

				// First pass: count elements
				int count = 0;
				for (int i = 0; i < HASH_SIZE; ++i) {
					t_var *elem = var->data.array[i];
					while (elem) {
						count++;
						elem = elem->next;
					}
				}

				if (!count) {
					char *result = ft_strdup("()");
					if (result) errno = E_NO_MEMORY;
					return (result);
				}

				// Allocate exact space needed
				t_var **elements = malloc(count * sizeof(t_var *));
				if (!elements) return (errno = E_NO_MEMORY, NULL);

				// Second pass: collect elements
				int idx = 0;
				for (int i = 0; i < HASH_SIZE; ++i) {
					t_var *elem = var->data.array[i];
					while (elem) {
						elements[idx++] = elem;
						elem = elem->next;
					}
				}

				// Sort alphabetically by key
				for (int i = 0; i < count - 1; ++i) {
					for (int j = 0; j < count - i - 1; ++j) {
						if (strcmp(elements[j]->key, elements[j + 1]->key) > 0) {
							t_var *temp = elements[j];
							elements[j] = elements[j + 1];
							elements[j + 1] = temp;
						}
					}
				}

				// Build result string: ([key1]="val1" [key2]="val2")
				char *result = ft_strdup("(");
				if (!result) {
					free(elements);
					return (errno = E_NO_MEMORY, NULL);
				}

				for (int i = 0; i < count; ++i) {
					char buffer[4096];
					char *formatted = format_for_shell(elements[i]->data.scalar, '\"');
					snprintf(buffer, sizeof(buffer), "%s[%s]=\"%s\"", (i > 0) ? " " : "", elements[i]->key, formatted);
					free(formatted);
					
					// Concatenate to result
					result = ft_strjoin(result, buffer, J_FREE_VAL_1);
					if (!result) {
						free(elements);
						return (errno = E_NO_MEMORY, NULL);
					}
				}

				// Add closing parenthesis
				result = ft_strjoin(result, ")", J_FREE_VAL_1);
				if (!result) errno = E_NO_MEMORY;
				free(elements);
				
				return (result);
			}

			char *variable_assoc_values(t_env *env, const char *key) {
				if (!env || !key) return (NULL);

				t_var *var = variable_get(env, key, 1);
				if (!var) return (NULL);

				if (!(var->flags & VAR_ASSOCIATIVE)) return (errno = E_VAR_INVALID_TYPE, NULL);

				return (format_assoc_values(var));
			}

		#pragma endregion

	#pragma endregion

	#pragma region "Reference"

		#pragma region "Get"

			char *variable_reference_get(t_env *env, const char *key) {
				if (!env || !key) return (NULL);

				t_var *var = variable_get(env, key, 0);
				if (!var) return (NULL);

				if (!(var->flags & VAR_REFERENCE)) return (errno = E_VAR_INVALID_TYPE, NULL);

				return (var->data.scalar);
			}

		#pragma endregion

		#pragma region "Set"

			int variable_reference_set(t_env *env, const char *key, const char *value, int append, int type, int local) {
				if (!env || !key) return (0);

				type &= (VAR_EXPORTED | VAR_READONLY | VAR_INTEGER | VAR_REFERENCE);

				errno = 0;
				t_var *var = (local) ? NULL : variable_get(env, key, 1);
				if (errno) return (errno);

				// If not found or local, search only in current environment
				if (!var) {
					t_var *local_var = env->table[hash_index(key)];
					while (local_var) {
						if (!strcmp(local_var->key, key)) {
							var = local_var;
							break;
						}
						local_var = local_var->next;
					}
				}

				if (var && (var->flags & (VAR_ARRAY | VAR_ASSOCIATIVE)))	return (errno = E_VAR_INVALID_TYPE, E_VAR_INVALID_TYPE);
				if (var && (var->flags & VAR_READONLY))						return (errno = E_VAR_READONLY, E_VAR_READONLY);

				// If doesn't exist, create new variable
				if (!var) {
					var = calloc(1, sizeof(t_var));
					if (!var) return (errno = E_NO_MEMORY, E_NO_MEMORY);

					var->key = ft_strdup(key);
					if (!var->key) {
						free(var);
						return (errno = E_NO_MEMORY, E_NO_MEMORY);
					}

					if (value) {
						var->data.scalar = ft_strdup(value);
						if (!var->data.scalar) {
							free(var->key);
							free(var);
							return (errno = E_NO_MEMORY, E_NO_MEMORY);
						}
					}

					var->flags = type;

					unsigned int hash = hash_index(key);
					var->next = env->table[hash];
					env->table[hash] = var;

					return (0);
				}

				// Variable exists, update value
				if (append) {
					char *new_value = ft_strjoin(var->data.scalar, value, J_FREE_NONE);
					if (!new_value) return (errno = E_NO_MEMORY, E_NO_MEMORY);
					free(var->data.scalar);
					var->data.scalar = new_value;
				} else {
					char *new_value = ft_strdup(value);
					if (!new_value) return (errno = E_NO_MEMORY, E_NO_MEMORY);
					free(var->data.scalar);
					var->data.scalar = new_value;
				}

				var->flags |= type;

				return (0);
			}

		#pragma endregion

		#pragma region "Del"

			int variable_reference_del(t_env *env, const char *key) {
				if (!env || !key) return (0);

				t_var *var = variable_get(env, key, 1);
				if (errno) return (errno);

				if (var) var->flags &= ~VAR_REFERENCE;

				return (0);
			}

		#pragma endregion

	#pragma endregion

#pragma endregion

#pragma region "Export"

	#pragma region "Array"

		char **variable_to_array(t_env *env) {
			if (!env) return (NULL);

			// Temporary hash table
			t_var *seen[HASH_SIZE] = {NULL};
			int count = 0;

			// Add to hash while counting
			for (t_env *e = env; e; e = e->parent) {
				for (int i = 0; i < HASH_SIZE; ++i) {
					t_var *var = e->table[i];
					while (var) {
						if (var->flags & VAR_EXPORTED && !(var->flags & (VAR_ARRAY | VAR_ASSOCIATIVE)) && var->data.scalar) {

							// Check if already in hash
							int hash = hash_index(var->key);
							int already_seen = 0;

							t_var *current = seen[hash];
							while (current) {
								if (!strcmp(current->key, var->key)) {
									already_seen = 1;
									break;
								}
								current = current->next;
							}

							// If not seen, add it to the hash
							if (!already_seen) {
								t_var *entry = malloc(sizeof(t_var));
								if (!entry) {
									variable_clear_table(seen);
									return (errno = E_NO_MEMORY, NULL);
								}
								entry->key = ft_strdup(var->key);
								entry->data.scalar = ft_strjoin_sep(var->key, "=", var->data.scalar, J_FREE_NONE);
								entry->flags = VAR_NONE;
								entry->next = seen[hash];
								seen[hash] = entry;

								if (!entry->key || !entry->data.scalar) {
									variable_clear_table(seen);
									return (errno = E_NO_MEMORY, NULL);
								}

								count++;
							}
						}
						var = var->next;
					}
				}
			}

			// Allocate array
			char **array = malloc((count + 1) * sizeof(char *));
			if (!array) {
				variable_clear_table(seen);
				return (errno = E_NO_MEMORY, NULL);
			}

			// Extract values from hash
			int index = 0;
			for (int i = 0; i < HASH_SIZE; ++i) {
				t_var *entry = seen[i];
				while (entry) {
					array[index++] = entry->data.scalar;
					t_var *next = entry->next;
					free(entry->key);
					free(entry);
					entry = next;
				}
			}
			array[index] = NULL;

			return (array);
		}

	#pragma endregion

	#pragma region "Print"

		#pragma region "Array Value"

			// Format a variable into a string
			static int array_value(unsigned int type, char **array, size_t i, t_var *var) {
				if (type && (var->flags & type) != type) return (0);

				int j = 0;
				char var_type[7]; 
				var_type[j++] = '-';
				if		(var->flags & VAR_ARRAY)		var_type[j++] = 'a';
				else if (var->flags & VAR_ASSOCIATIVE)	var_type[j++] = 'A';
				if (var->flags & VAR_INTEGER)			var_type[j++] = 'i';
				if (var->flags & VAR_READONLY)			var_type[j++] = 'r';
				if (var->flags & VAR_EXPORTED)			var_type[j++] = 'x';
				if (j == 1)								var_type[j++] = '-';
				while (j < 6)							var_type[j++] = ' ';
				var_type[j] = '\0';

				array[i] = ft_strjoin_sep("declare ", var_type, var->key, 0);
				if (!array[i]) return (errno = E_NO_MEMORY, 0);

				// Add value based on type
				if (var->flags & VAR_ARRAY) {
					char *values = format_array_values(var);
					if (values) array[i] = ft_strjoin_sep(array[i], "=", values, J_FREE_VAL_3);
				} else if (var->flags & VAR_ASSOCIATIVE) {
					char *values = format_assoc_values(var);
					if (values) array[i] = ft_strjoin_sep(array[i], "=", values, J_FREE_VAL_3);
				} else if (var->data.scalar) {
					char *value = format_for_shell(var->data.scalar, '\"');
					array[i] = ft_strjoin_sep(array[i], "=", value, J_FREE_VAL_1_3);
				}

				return (1);
			}

		#pragma endregion

		#pragma region "Is Shadowed"

			// Check if a key has already been seen (for shadowing)
			static int is_shadowed(const char *key, t_var **seen, int count) {
				if (!key || !seen || !count) return (0);

				for (int i = 0; i < count; i++) {
					if (seen[i] && !strcmp(seen[i]->key, key)) return (1);
				}

				return (0);
			}

		#pragma endregion

		#pragma region "Print"

			void variable_print(t_env *env, unsigned int type, int sort, int local) {
				if (!env) return;

				errno = 0;
				if (local) env->parent = NULL;

				// First pass: count total variables
				int count = 0;
				t_env *current = env;
				while (current) {
					for (int index = 0; index < HASH_SIZE; ++index) {
						t_var *var = current->table[index];
						while (var) {
							count++;
							var = var->next;
						}
					}
					current = current->parent;
				}
				if (!count) return;

				// Allocate memory for tracking seen variables
				t_var **seen = malloc(count * sizeof(t_var *));
				if (!seen) {
					errno = E_NO_MEMORY;
					return;
				}

				// Second pass: collect unique variables respecting shadowing
				count = 0;
				current = env;
				while (current) {
					for (int index = 0; index < HASH_SIZE; ++index) {
						t_var *var = current->table[index];
						while (var) {
							if (!is_shadowed(var->key, seen, count) && (!type || (var->flags & type) == type))
								seen[count++] = var;
							var = var->next;
						}
					}
					current = current->parent;
				}
				if (!count) {
					free(seen);
					errno = E_NO_MEMORY;
					return;
				}

				// Create string array for printing
				char **array = malloc((count + 1) * sizeof(char *));
				if (!array) {
					free(seen);
					errno = E_NO_MEMORY;
					return;
				}

				// Third pass: format variables
				int i = 0;
				for (int j = 0; j < count; ++j) {
					i += array_value(type, array, i, seen[j]);
					if (errno) {
						array_free(array);
						errno = E_NO_MEMORY;
						return ;
					}
				}
				array[i] = NULL;

				// Sort
				if (sort) array_nsort(array, sort, 13);

				// Print
				if (array && array[0]) {
					print(STDOUT_FILENO, NULL, P_RESET);

					for (int j = 0; array[j]; ++j) {
						print(STDOUT_FILENO, array[j], P_JOIN);
						print(STDOUT_FILENO, "\n",     P_JOIN);
					}

					print(STDOUT_FILENO, NULL, P_PRINT);
				}

				// Cleanup
				free(seen);
				array_free(array);
			}

		#pragma endregion

	#pragma endregion

#pragma endregion

#pragma region "Delete"

	#pragma region "Free"

		static void variable_free(t_var *var) {
			if (!var) return;

			free(var->key);
			if (var->flags & (VAR_ARRAY | VAR_ASSOCIATIVE)) {
				if (var->data.array) {
					for (int i = 0; i < HASH_SIZE; ++i)
						variable_free(var->data.array[i]);
					free(var->data.array);
				}
			} else {
				free(var->data.scalar);
			}

			free(var);
		}

	#pragma endregion

	#pragma region "Unset"

		int variable_unset(t_env *env, const char *key, int reference) {
			if (!env || !key) return (0);

			t_var *var = variable_get(env, key, reference);
			if (!var || errno) return (errno);

			char *last_key = var->key;

			while (env) {
				unsigned int hash = hash_index(last_key);
				var = env->table[hash];
				t_var *prev = NULL;

				while (var) {
					if (!strcmp(var->key, last_key)) {
						if (var->flags & VAR_READONLY) return (errno = E_VAR_READONLY, E_VAR_READONLY);

						if (prev)	prev->next = var->next;
						else		env->table[hash] = var->next;

						variable_free(var);
						specials_unset(env, last_key);

						return (0);
					}
					var = var->next;
				}
				env = env->parent;
			}

			return (0);
		}

	#pragma endregion

	#pragma region "Clear Table"

		void variable_clear_table(t_var **table) {
			for (int i = 0; i < HASH_SIZE; ++i) {
				t_var *var = table[i];
				while (var) {
					t_var *next = var->next;
					variable_free(var);
					var = next;
				}
				table[i] = NULL;
			}
		}

	#pragma endregion

	#pragma region "Clear"

		void variable_clear(t_env *env) {
			if (!env) return;

			variable_clear_table(env->table);
			if (env->parent) variable_clear(env->parent);
		}

	#pragma endregion

#pragma endregion

#pragma region "Initialize"

	#pragma region "From Array"

		void variable_from_array(t_env *env, const char **array) {
			if (!env || !array) return;

			for (size_t i = 0; array[i]; i++) {
				char *key = NULL, *value = NULL;
				get_key_value(array[i], &key, &value, '=');
				if (!key) continue;
				variable_scalar_set(env, key, value, 0, VAR_EXPORTED, 0);
				free(key); free(value);
			}
		}

	#pragma endregion

	// exported, readonly, integer, force, free_value
	static void default_add(t_env *env, const char *key, char *value, int type, int force, int free_value) {
		if (!env || !key || !value) return;

		if (force || !variable_get(env, key, 1)) variable_scalar_set(env, key, value, 0, type, 0);
		if (free_value) free(value);
	}

	int variable_initialize(t_env *env, const char **envp) {
		if (!env) return (0);

		variable_from_array(env, envp);

		// History
		char *home = get_home();
		if (home) {
			if (*home && home[ft_strlen(home) - 1] == '/') home[ft_strlen(home) - 1] = '\0';
			home = ft_strjoin(home, "/.42sh_history", J_FREE_VAL_1);
			default_add(env,	"42_HISTFILE",		home,							VAR_NONE,					0, 1);		//	
		}
		default_add(env,		"42_HISTSIZE",		"5",							VAR_NONE,					0, 0);		//	
		default_add(env,		"42_HISTFILESIZE",	"2000",							VAR_NONE,					0, 0);		//	
		default_add(env,		"42_HISTCONTROL",	"ignoreboth",					VAR_NONE,					0, 0);		//	

		// Shell
		default_add(env,		"42_SH",			"PATH OF 42SH",					VAR_NONE,					1, 0);		//	Normal var but set value on start always
		default_add(env,		"42_SUBSHELL",		"0",							VAR_NONE,					1, 0);		//	When modified, update (shell_level with value too) - Increment subshell_level in child when fork() or subshell
		default_add(env,		"42_VERSION",		VERSION,						VAR_NONE,					1, 0);		//	Normal var but set value on start always
		default_add(env,		"42_PID",			ft_itoa(shell.pid),				VAR_NONE,					1, 1);		//	Can be modified, but expand dinamic value
		default_add(env,		"PPID",				ft_itoa(shell.parent_pid),		VAR_INTEGER,				1, 1);		//	Update var when expanded (parent_pid) VAR_READONLY

		// Terminal
		default_add(env,		"COLUMNS",			ft_itoa(terminal.cols),			VAR_INTEGER,				1, 1);		//	Update var when expanded (terminal_columns)
		default_add(env,		"LINES",			ft_itoa(terminal.rows),			VAR_INTEGER,				1, 1);		//	Update var when expanded (terminal_rows)
		default_add(env,		"SECONDS",			"0",							VAR_NONE,					1, 0);		//	Can be modified, but expand dinamic value
		default_add(env,		"EPOCHSECONDS",		ft_itoa(shell.epoch_seconds),	VAR_READONLY | VAR_INTEGER,	1, 1);		//	Update everytime (even with env)
		default_add(env,		"EPOCHREALTIME",	ft_itoa(shell.epoch_realtime),	VAR_READONLY | VAR_INTEGER,	1, 1);		//	Update everytime (even with env)
		default_add(env,		"UID",				ft_itoa(shell.uid),				VAR_INTEGER,				1, 1);		//	Update var when expanded (shell_uid) VAR_READONLY
		default_add(env,		"EUID",				ft_itoa(shell.euid),			VAR_INTEGER,				1, 1);		//	Update var when expanded (shell_euid) VAR_READONLY

		// Prompt
		default_add(env,		"PS1",				terminal.PS1,					VAR_NONE,					1, 0);		//	Normal var but set value on start always
		default_add(env,		"PS2",				terminal.PS2,					VAR_NONE,					1, 0);		//	Normal var but set value on start always
		default_add(env,		"PS3",				terminal.PS3,					VAR_NONE,					1, 0);		//	Normal var but set value on start always
		default_add(env,		"PS4",				terminal.PS4,					VAR_NONE,					1, 0);		//	Normal var but set value on start always
		//	BASH_COMMAND																										//	Can be modified, but expand dinamic value (dont create on startup)

		return (0);
	}

#pragma endregion

#pragma region "Information"

	//	VARIABLES
	//
	// Readonly	La variable es de solo lectura. No se puede modificar ni eliminar.
	// Exported	La variable está exportada al entorno y será visible para procesos hijo.
	// Integer	La variable solo almacena valores numéricos (realiza cálculos automáticamente).

	//	42HISTCMD			la crea al cargar el historial y le asigna el numero de evento actual (readonly)
	//	42HISTSIZE			1000
	//	42HISTFILESIZE		2000
	//	42HISTCONTROL		ignorespace:ignoredups:ignoreboth:erasedups
	//	42HISTFILE			~/.42sh_history


	//	VARIABLES DINAMICAS (no se añaden a la lista de variables)

	//	$$				Current process PID
	//	$!				Last background process PID
	//	EPOCHSECONDS
	//	EPOCHREALTIME
	//	SECONDS
	//	BASH_COMMAND	Contiene el comando que está siendo ejecutado en ese momento.
	//	$?				Código de salida del último comando ejecutado.
	//	$#				Número de argumentos pasados al script o función.
	//	$@ y $*			Lista de todos los argumentos pasados al script o función.
	//	$0				Nombre del script o shell en ejecución.


	//	HISTCMD			Número del comando actual en el historial.
	//	HISTFILESIZE	Número máximo de líneas permitidas en el archivo del historial.
	//	HISTSIZE		Número máximo de líneas guardadas en el historial de la sesión.

	//	BASH_VERSION	La versión de Bash que estás ejecutando.
	//	BASHPID			PID del proceso del shell actual.
	//	PPID			Parent process PID.

	//	COLUMNS			Número de columnas del terminal.
	//	LINES			Número de líneas del terminal.

	//	UID y EUID		El UID (User ID) y EUID (Effective User ID) del usuario que ejecuta el shell.
	//	PS1, PS2, PS4	Variables relacionadas con los prompts del shell.

	// var=(valor1 valor2) 							# crea array numerado [0]=valor1 [1]=valor2
	// var=([3]=valor1 [5]=valor2)  				# crea array numerado [3]=valor1 [5]=valor2
	// declare -a var=([3]=valor1 [5]=valor2)  		# crea array numerado [3]=valor1 [5]=valor2
	// declare -A var=([key1]=valor1 [key2]=valor2)	# crea array asociativo

#pragma endregion


	// Nueva idea:
	//
	// Funcion que pasandole una asignacion, detecta si tiene [] y devuelva el indice como char *
	// Funcion que pasandole value, devuelva lista con indices y values (para cuando es =(...))
	// Si key tiene indice [] y asignacion tambien: 'cannot assign list to array member'

	// Entonces una funcion que le pasas el arg y te devuelve key, indices y values 
	// o key, indice 0 y value si no tiene indices


	// Tendría que detectar [ y si el caracter anterior no es isspace o es ([ y buscando el final (cumpliendo contextos) termina en ]= entonces es un indice
	// Todo esto se hace en el parseo, de forma que cuando llega como argumento, tenemos algo... no se que, pero algo
	// Quién me mandaría meterme en ese lío
