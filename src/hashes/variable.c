/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 17:39:40 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/10 17:53:28 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "terminal/terminal.h"
	#include "terminal/readinput/history.h"
	#include "main/shell.h"
	#include "utils/utils.h"

#pragma endregion

#pragma region "Common"

	#pragma region "Specials"

		char *specials_get(const char *key) {
			if (!key) return (NULL);

			static char special[4096];

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

		void specials_set(const char *key, const char *value) {
			if (!key) return;

			// History
			if (!strcmp(key, "42_HISTFILE"))		history_file_set(value);
			if (!strcmp(key, "42_HISTSIZE"))		history_size_set(atol(value), HIST_MEM);
			if (!strcmp(key, "42_HISTFILESIZE"))	history_size_set(atol(value), HIST_FILE);
			if (!strcmp(key, "42_HISTTIMEFORMAT"))	history_hist_timeformat_set(value);
			if (!strcmp(key, "42_HISTCONTROL"))		history_hist_control_set(value);
			if (!strcmp(key, "42_HISTIGNORE"))		history_hist_ignore_set(value);
		}

		void specials_unset(const char *key) {
			if (!key) return;

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
			unsigned int hash = 0;

			while (*key) hash = (hash * 31) + *key++;

			return (hash % HASH_SIZE);
		}

	#pragma endregion

	#pragma region "Find"

		// Find variable by 'key'
		t_var *variable_find(t_env *env, const char *key) {
			if (!env || !key) return (NULL);
			
			// Check specials first
			static t_var special;
			special.data.scalar = specials_get(key);
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

	#pragma region "Validate"

		int variable_validate(const char *key, int local_assing) {
			if (!key) return (0);

			int		ret = 0;
			size_t	len = ft_strlen(key);

			if (!len || (!isalpha(key[0]) && key[0] != '_'))	ret = 1;

			(void) local_assing;
			// Valid only if in shell (!shell.env->parent)
			// 
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

#pragma endregion

#pragma region "Types"

	#pragma region "Scalar"

		#pragma region "Get"

			char *variable_scalar_get(t_env *env, const char *key) {
				t_var *var = variable_find(env, key);
				if (!var || var->flags & (VAR_ARRAY | VAR_ASSOCIATIVE)) return (NULL);

				return (var->data.scalar);
			}

		#pragma endregion

		#pragma region "Set"

			int variable_scalar_set(t_env *env, const char *key, const char *value, int append, int local) {
				if (!env || !key) return (1);

				t_var *var = (local) ? NULL : variable_find(env, key);

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

				if (var && (var->flags & (VAR_ARRAY | VAR_ASSOCIATIVE)))	return (1);  // If exists but is array/associative, error
				if (var && (var->flags & VAR_READONLY))						return (1);  // If exists but is readonly, error

				// If doesn't exist, create new variable
				if (!var) {
					var = calloc(1, sizeof(t_var));
					if (!var) return (1);

					var->key = ft_strdup(key);
					if (!var->key) {
						free(var);
						return (1);
					}

					if (value) {
						var->data.scalar = ft_strdup(value);
						if (!var->data.scalar) {
							free(var->key);
							free(var);
							return (1);
						}
					}

					var->flags = VAR_NONE;

					// Insert into current environment
					unsigned int hash = hash_index(key);
					var->next = env->table[hash];
					env->table[hash] = var;

					return (0);
				}

				// Variable exists, update value
				if (append) {
					char *new_value = ft_strjoin(var->data.scalar, value, 0);
					if (!new_value) return (1);
					free(var->data.scalar);
					var->data.scalar = new_value;
				} else {
					char *new_value = ft_strdup(value);
					if (!new_value) return (1);
					free(var->data.scalar);
					var->data.scalar = new_value;
				}

				return (0);
			}

		#pragma endregion

		#pragma region "Values"

			char *variable_scalar_value(t_env *env, const char *key) {
				if (!env || !key) return (NULL);

				t_var *var = variable_find(env, key);
				if (var && (var->flags & (VAR_ARRAY | VAR_ASSOCIATIVE))) return (NULL);

				return (var->data.scalar);
			}

		#pragma endregion

		#pragma region "From Array"

			void variable_from_array(t_var **table, const char **array) {
				if (!array) return;

				(void) table;
				for (size_t i = 0; array[i]; i++) {
					char *key = NULL, *value = NULL;
					get_key_value(array[i], &key, &value, '=');
					if (!key) continue;
					// variable_add(table, key, value, 1, 0, 0, 0);
					free(key); free(value);
				}
			}

	#pragma endregion

	#pragma endregion

	#pragma region "Array"

		#pragma region "Get"

			char *variable_array_get(t_env *env, const char *key, int index) {
				if (!env || !key || index < 0) return (NULL);

				t_var *var = variable_find(env, key);
				if (!var) return (NULL);

				// If scalar and requesting index 0, return the scalar
				if (!(var->flags & (VAR_ARRAY | VAR_ASSOCIATIVE)) && !index) return (var->data.scalar);

				if (!(var->flags & VAR_ARRAY)) return (NULL);	// Not a numeric array, error

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
				if (!env || !key || index < 0) return (1);

				t_var *var = (local) ? NULL : variable_find(env, key);

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

				if (var && (var->flags & VAR_ASSOCIATIVE))	return (1);		// If exists but is associative, error
				if (var && (var->flags & VAR_READONLY))		return (1);		// If exists but is readonly, error

				// If doesn't exist, create new array variable
				if (!var) {
					var = calloc(1, sizeof(t_var));
					if (!var) return (1);

					char *new_key = ft_strdup(key);
					if (!new_key) {
						free(var);
						return (1);
					}
					var->key = new_key;
					var->flags = VAR_ARRAY;
					var->data.array = calloc(HASH_SIZE, sizeof(t_var *));
					if (!var->data.array) {
						free(var->key);
						free(var);
						return (1);
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
					if (!var->data.array) return (1);

					// Save old value at index 0
					if (old_value) {
						t_var *elem = calloc(1, sizeof(t_var));
						if (!elem) {
							free(var->data.array);
							var->data.scalar = old_value;
							return (1);
						}
						char *new_elem_key = ft_strdup("0");
						if (!new_elem_key) {
							free(elem);
							free(var->data.array);
							var->data.scalar = old_value;
							return (1);
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
							char *new_value = ft_strjoin(elem->data.scalar, value, 0);
							if (!new_value) return (1);
							free(elem->data.scalar);
							elem->data.scalar = new_value;
						} else {
							char *new_value = ft_strdup(value);
							if (!new_value) return (1);
							free(elem->data.scalar);
							elem->data.scalar = new_value;
						}
						return (0);
					}
					elem = elem->next;
				}
				
				// Create new element
				elem = calloc(1, sizeof(t_var));
				if (!elem) return (1);

				char *new_elem_key = ft_strdup(idx_str);
				if (!new_elem_key) {
					free(elem);
					return (1);
				}
				elem->key = new_elem_key;
				char *new_elem_value = ft_strdup(value);
				if (!new_elem_value) {
					free(elem->key);
					free(elem);
					return (1);
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
				if (!env || !key || index < 0) return (1);

				t_var *var = variable_find(env, key);
				if (!var || !(var->flags & VAR_ARRAY)) return (1);

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

				return (1);
			}

		#pragma endregion

		#pragma region "Values"

			// Format array values for display: ([0]="val1" [3]="val2")
			static char *format_array_values(t_var *var) {
				if (!var || !(var->flags & VAR_ARRAY)) return (NULL);

				// First pass: count elements
				int count = 0;
				for (int i = 0; i < HASH_SIZE; ++i) {
					t_var *elem = var->data.array[i];
					while (elem) {
						count++;
						elem = elem->next;
					}
				}

				if (!count) return (ft_strdup("()"));

				// Allocate exact space needed
				t_var **elements = malloc(count * sizeof(t_var *));
				if (!elements) return (NULL);

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
					return (NULL);
				}

				for (int i = 0; i < count; ++i) {
					char buffer[4096];
					char *formatted = format_for_shell(elements[i]->data.scalar, '\"');
					snprintf(buffer, sizeof(buffer), "%s[%s]=\"%s\"", (i > 0) ? " " : "", elements[i]->key, formatted);
					free(formatted);

					// Concatenate to result
					result = ft_strjoin(result, buffer, 1);
					if (!result) {
						free(elements);
						return (NULL);
					}
				}

				// Add closing parenthesis
				result = ft_strjoin(result, ")", 1);
				free(elements);

				return (result);
			}

			char *variable_array_values(t_env *env, const char *key) {
				if (!env || !key) return (NULL);

				t_var *var = variable_find(env, key);
				if (!var || !(var->flags & VAR_ARRAY)) return (NULL);

				return (format_array_values(var));
			}

		#pragma endregion

	#pragma endregion

	#pragma region "Associative"

		#pragma region "Get"

			char *variable_assoc_get(t_env *env, const char *key, const char *assoc_key) {
				t_var *var = variable_find(env, key);
				if (!var || !(var->flags & VAR_ASSOCIATIVE)) return (NULL);

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
				if (!env || !key || !assoc_key)	return (1);
				if (!*assoc_key)				return (2);

				t_var *var = (local) ? NULL : variable_find(env, key);

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

				if (var && !(var->flags & VAR_ASSOCIATIVE))	return (1);		// If exists but is no associative, error
				if (var && (var->flags & VAR_READONLY))		return (1);		// If exists but is readonly, error

				// If doesn't exist, create new associative variable
				if (!var) {
					var = calloc(1, sizeof(t_var));
					if (!var) return (1);

					char *new_key = ft_strdup(key);
					if (!new_key) {
						free(var);
						return (1);
					}
					var->key = new_key;
					var->flags = VAR_ASSOCIATIVE;
					var->data.array = calloc(HASH_SIZE, sizeof(t_var *));
					if (!var->data.array) {
						free(var->key);
						free(var);
						return (1);
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
							char *new_value = ft_strjoin(elem->data.scalar, value, 0);
							if (!new_value) return (1);
							free(elem->data.scalar);
							elem->data.scalar = new_value;
						} else {
							char *new_value = ft_strdup(value);
							if (!new_value) return (1);
							free(elem->data.scalar);
							elem->data.scalar = new_value;
						}
						return (0);
					}
					elem = elem->next;
				}

				// Create new element
				elem = calloc(1, sizeof(t_var));
				if (!elem) return (1);

				char *new_elem_key = ft_strdup(assoc_key);
				if (!new_elem_key) {
					free(elem);
					return (1);
				}
				elem->key = new_elem_key;
				char *new_elem_value = ft_strdup(value);
				if (!new_elem_value) {
					free(elem->key);
					free(elem);
					return (1);
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
				if (!env || !key)	return (1);
				if (!*assoc_key)	return (2);

				t_var *var = variable_find(env, key);
				if (!var || !(var->flags & VAR_ASSOCIATIVE)) return (1);

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

				return (1);
			}

		#pragma endregion

		#pragma region "Values"

			// Format associative array values for display: ([key1]="val1" [key2]="val2")
			static char *format_assoc_values(t_var *var) {
				if (!var || !(var->flags & VAR_ASSOCIATIVE)) return (NULL);

				// First pass: count elements
				int count = 0;
				for (int i = 0; i < HASH_SIZE; ++i) {
					t_var *elem = var->data.array[i];
					while (elem) {
						count++;
						elem = elem->next;
					}
				}

				if (!count) return (ft_strdup("()"));

				// Allocate exact space needed
				t_var **elements = malloc(count * sizeof(t_var *));
				if (!elements) return (NULL);

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
					return (NULL);
				}

				for (int i = 0; i < count; ++i) {
					char *formatted = format_for_shell(elements[i]->data.scalar, '\"');
					
					char buffer[4096];
					snprintf(buffer, sizeof(buffer), "%s[%s]=\"%s\"", 
							(i > 0) ? " " : "", 
							elements[i]->key, 
							formatted);
					
					free(formatted);
					
					// Concatenate to result
					result = ft_strjoin(result, buffer, 1);
					if (!result) {
						free(elements);
						return (NULL);
					}
				}

				// Add closing parenthesis
				result = ft_strjoin(result, ")", 1);
				free(elements);
				
				return (result);
			}

			char *variable_assoc_values(t_env *env, const char *key) {
				if (!env || !key) return (NULL);

				t_var *var = variable_find(env, key);
				if (!var || !(var->flags & VAR_ASSOCIATIVE)) return (NULL);

				return (format_assoc_values(var));
			}

		#pragma endregion

	#pragma endregion

#pragma endregion

#pragma region "Delete"

	#pragma region "Free"

		void variable_free(t_var *var) {
			if (!var) return;

			free(var->key);
			if (var->flags & VAR_ARRAY || var->flags & VAR_ASSOCIATIVE) {
				if (var->data.array) {
					for (int i = 0; var->data.array[i]; ++i)
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

		int variable_unset(t_env *env, const char *key) {
			if (!env || !key) return (1);

			int hash = hash_index(key);
			t_var *current = env->table[hash];
			t_var *prev = NULL;

			while (current) {
				if (!strcmp(current->key, key)) {
					if (current->flags & VAR_READONLY) return (1);

					if (prev)	prev->next = current->next;
					else		env->table[hash] = current->next;

					variable_free(current);
					return (0);
				}
				prev = current;
				current = current->next;
			}

			if (env->parent) return variable_unset(env->parent, key);

			return (1);
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

#pragma region "Export"

	#pragma region "Array"

		char **variable_to_array(t_env *env) {
			if (!env) return NULL;

			// Temporary hash table
			t_var *seen[HASH_SIZE] = {NULL};
			int count = 0;

			// Add to hash while counting
			for (t_env *e = env; e; e = e->parent) {
				for (int i = 0; i < HASH_SIZE; ++i) {
					t_var *var = e->table[i];
					while (var) {
						if (var->flags & VAR_EXPORTED && var->data.scalar && !(var->flags & (VAR_ARRAY | VAR_ASSOCIATIVE))) {

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
									return (NULL);
								}
								entry->key = ft_strdup(var->key);
								entry->data.scalar = ft_strjoin_sep(var->key, "=", var->data.scalar, 0);
								entry->flags = VAR_NONE;
								entry->next = seen[hash];
								seen[hash] = entry;

								if (!entry->key || !entry->data.scalar) {
									variable_clear_table(seen);
									return (NULL);
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
				return (NULL);
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
				char var_type[6]; 
				var_type[j++] = '-';
				if (var->flags & VAR_ARRAY)         var_type[j++] = 'a';
				if (var->flags & VAR_ASSOCIATIVE)   var_type[j++] = 'A';
				if (var->flags & VAR_INTEGER)       var_type[j++] = 'i';
				if (var->flags & VAR_READONLY)      var_type[j++] = 'r';
				if (var->flags & VAR_EXPORTED)      var_type[j++] = 'x';
				if (j == 1) var_type[j++] = '-';
				while (j < 5) var_type[j++] = ' ';
				var_type[j] = '\0';

				// For shell variables (no export flag)
				if (!(var->flags & VAR_EXPORTED))
					array[i] = ft_strdup(var->key);
				else
					array[i] = ft_strjoin_sep("declare ", var_type, var->key, 0);
				
				if (!array[i]) return (0);
				
				// Add value based on type
				if (var->flags & VAR_ARRAY) {
					char *values = format_array_values(var);
					if (values) {
						array[i] = ft_strjoin_sep(array[i], "=", values, 3);
					}
				} else if (var->flags & VAR_ASSOCIATIVE) {
					char *values = format_assoc_values(var);
					if (values) {
						array[i] = ft_strjoin_sep(array[i], "=", values, 3);
					}
				} else if (var->data.scalar) {
					char *formatted = format_for_shell(var->data.scalar, '\"');
					array[i] = ft_strjoin_sep(array[i], "=", formatted, 6);
				}

				return (1);
			}

		#pragma endregion

		#pragma region "Is Shadowed"

			// Check if a key has already been seen (for shadowing)
			static int is_shadowed(const char *key, t_var **seen, int count) {
				if (!key || !seen || !*seen) return (0);

				for (int i = 0; i < count; i++) {
					if (!strcmp(seen[i]->key, key)) return (1);
				}

				return (0);
			}

		#pragma endregion

		#pragma region "Print"

			void variable_print(t_env *env, unsigned int type, int sort) {
				if (!env) return;

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
				if (!seen) return;

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
				if (!count) { free(seen); return; }

				// Create string array for printing
				char **array = malloc((count + 1) * sizeof(char *));
				if (!array) { free(seen); return; }

				// Third pass: format variables
				int i = 0;
				for (int j = 0; j < count; ++j) {
					i += array_value(type, array, i, seen[j]);
				}
				array[i] = NULL;

				// Sort
				if (sort) array_nsort(array, sort, 13);

				// Print
				if (array && array[0]) {
					print(STDOUT_FILENO, NULL, RESET);

					for (int j = 0; array[j]; ++j) {
						print(STDOUT_FILENO, array[j], JOIN);
						print(STDOUT_FILENO, "\n",     JOIN);
					}

					print(STDOUT_FILENO, NULL, PRINT);
				}

				// Cleanup
				free(seen);
				array_free(array);
			}

		#pragma endregion

	#pragma endregion

#pragma endregion

#pragma region "Initialize"

	static void default_add(t_var **table, const char *name, char *value, int exported, int readonly, int integer, int force, int free_value) {
		if (!value) return;
		(void) table;
		(void) name;
		(void) exported;
		(void) readonly;
		(void) integer;
		(void) force;
		// if (force || !variable_find(table, name)) variable_add(table, name, value, exported, readonly, integer, 1);
		if (value && free_value) free(value);
	}

	int variable_initialize(t_var **table, const char **envp) {
		variable_from_array(table, envp);

		// History
		char *home = get_home();
		if (home) {
			if (*home && home[ft_strlen(home) - 1] == '/') home[ft_strlen(home) - 1] = '\0';
			home = ft_strjoin(home, "/.42sh_history", 1);
			default_add(table, "42_HISTFILE", home, 0, 0, 0, 0, 1);								//	
		}
		default_add(table, "42_HISTSIZE", "5", 0, 0, 0, 0, 0);									//	
		default_add(table, "42_HISTFILESIZE", "2000", 0, 0, 0, 0, 0);							//	
		default_add(table, "42_HISTCONTROL", "ignoreboth", 0, 0, 0, 0, 0);						//	

		// Shell
		default_add(table, "42_SH", "PATH OF 42SH", 0, 0, 0, 1, 0);								//	Normal var but set value on start always
		default_add(table, "42_SUBSHELL", "0", 0, 0, 0, 1, 0);									//	When modified, update (shell_level with value too) - Increment subshell_level in child when fork() or subshell
		default_add(table, "42_VERSION", VERSION, 0, 0, 0, 1, 0);								//	Normal var but set value on start always
		default_add(table, "42_PID", ft_itoa(shell.pid), 0, 0, 0, 1, 1);						//	Can be modified, but expand dinamic value
		default_add(table, "PPID", ft_itoa(shell.parent_pid), 0, 0, 1, 1, 1);					//	Update var when expanded (parent_pid) VAR_READONLY

		// Terminal
		default_add(table, "COLUMNS", ft_itoa(terminal.cols), 0, 0, 1, 1, 1);					//	Update var when expanded (terminal_columns)
		default_add(table, "LINES", ft_itoa(terminal.rows), 0, 0, 1, 1, 1);						//	Update var when expanded (terminal_rows)
		default_add(table, "SECONDS", "0", 0, 0, 0, 1, 0);										//	Can be modified, but expand dinamic value
		default_add(table, "EPOCHSECONDS", ft_itoa(shell.epoch_seconds), 0, 1, 1, 1, 1);		//	Update everytime (even with env)
		default_add(table, "EPOCHREALTIME", ft_itoa(shell.epoch_realtime), 0, 1, 1, 1, 1);		//	Update everytime (even with env)
		default_add(table, "UID", ft_itoa(shell.uid), 0, 0, 1, 1, 1);							//	Update var when expanded (shell_uid) VAR_READONLY
		default_add(table, "EUID", ft_itoa(shell.euid), 0, 0, 1, 1, 1);							//	Update var when expanded (shell_euid) VAR_READONLY

		// Prompt
		default_add(table, "PS1", terminal.PS1, 0, 0, 0, 1, 0);									//	Normal var but set value on start always
		default_add(table, "PS2", terminal.PS2, 0, 0, 0, 1, 0);									//	Normal var but set value on start always
		default_add(table, "PS3", terminal.PS3, 0, 0, 0, 1, 0);									//	Normal var but set value on start always
		default_add(table, "PS4", terminal.PS4, 0, 0, 0, 1, 0);									//	Normal var but set value on start always
		//	BASH_COMMAND																		//	Can be modified, but expand dinamic value (dont create on startup)

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
