/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 17:39:40 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/19 18:19:03 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "terminal.h"
#include "history.h"
#include "variables.h"
#include "key_value.h"
#include "builtins/options.h"
#include "shell.h"
#include "project.h"

#pragma region Variables

	t_var *vars_table[VARS_HASH_SIZE];

#pragma endregion

#pragma region Index

	static unsigned int hash_index(const char *key) {
		unsigned int hash = 0;

		while (*key) hash = (hash * 31) + *key++;
		return (hash % VARS_HASH_SIZE);
	}

#pragma endregion

#pragma region Import

	#pragma region Variable

		int variables_add(t_var **table, const char *key, const char *value, int exported, int readonly, int integer, int force) {
			if (!key) return (0);
			if (force == -1) force = 0;

			t_var *new_var = variables_find(table, key);
			if (new_var) {
				if (!new_var->readonly || force) {
					sfree(new_var->value);
					new_var->value = ft_strdup(value);
					if (readonly != -1) new_var->readonly = readonly;
					if (exported != -1) new_var->exported = exported;
					if (integer != -1) new_var->integer = integer;
					return (0);
				} else return (1);
			}

			unsigned int index = hash_index(key);
			new_var = ft_calloc(1, sizeof(t_var));

			new_var->name = ft_strdup(key);
			if (value) new_var->value = ft_strdup(value);

			if (readonly != -1) new_var->readonly = readonly;
			if (exported != -1) new_var->exported = exported;
			if (integer != -1) new_var->integer = integer;
			new_var->next = table[index];
			table[index] = new_var;

			return (0);
		}

	#pragma endregion

	#pragma region Concatenate

		int variables_concatenate(t_var **table, const char *key, char *value, int exported, int readonly, int integer, int force) {
			if (!key) return (0);
			if (force == -1) force = 0;

			t_var *new_var = variables_find(table, key);
			if (new_var) {
				if (!new_var->readonly || force) {
					new_var->value = ft_strjoin(new_var->value, value, 1);
					if (readonly != -1) new_var->readonly = readonly;
					if (exported != -1) new_var->exported = exported;
					if (integer != -1) new_var->integer = integer;
					return (0);
				} else return (1);
			}

			unsigned int index = hash_index(key);
			new_var = smalloc(sizeof(t_var));

			new_var->name = ft_strdup(key);
			new_var->value = NULL;
			if (value) new_var->value = ft_strdup(value);

			if (readonly != -1) new_var->readonly = readonly;
			if (exported != -1) new_var->exported = exported;
			if (integer != -1) new_var->integer = integer;
			new_var->next = table[index];
			table[index] = new_var;

			return (0);
		}

	#pragma endregion

	#pragma region Array

		void variables_from_array(t_var **table, const char **array) {
			if (!array) return;

			for (size_t i = 0; array[i]; i++) {
				char *key = NULL, *value = NULL;
				get_key_value(array[i], &key, &value, '=');
				if (!key) continue;
				variables_add(table, key, value, 1, 0, 0, 0);
				sfree(key); sfree(value);
			}
		}

#pragma endregion

	#pragma region Join

		void variables_join(t_var **dst_table, t_var **src_table) {
			if (dst_table == src_table) return;

			for (int index = 0; index < VARS_HASH_SIZE; index++) {
				t_var *var = src_table[index];
				while (var) {
					variables_add(dst_table, var->name, var->value, var->exported, var->readonly, var->integer, 0);
					var = var->next;
				}
			}
			variables_clear(src_table);
		}

	#pragma endregion

	#pragma region Validate

		int variables_validate(char *key, char *value, char *name, bool is_asign, bool show_msg) {
			if (!key) return (0);
			int result = 0;

			size_t len = ft_strlen(key);

			if (!len || (!ft_isalpha(key[0]) && key[0] != '_')) result = 1;

			for (size_t i = 1; i < len; ++i)
				if (!ft_isalnum(key[i]) && key[i] != '_') { result = 1; break; }

			if (result && show_msg) {
				if (!name) name = PROYECTNAME;
				print(STDERR_FILENO, ft_strjoin_sep(name, ": `", key, 0), FREE_RESET);
				if (is_asign)	print(STDERR_FILENO, ft_strjoin_sep("=", value, "': not a valid identifier\n", 0), FREE_PRINT);
				else			print(STDERR_FILENO, "': not a valid identifier\n", PRINT);
			}

			return (result);
		}

	#pragma endregion

#pragma endregion

#pragma region Export

	#pragma region Variable

		t_var *variables_find(t_var **table, const char *key) {
			if (!key) return (NULL);

			unsigned int index = hash_index(key);
			t_var *var = table[index];

			while (var) {
				if (!ft_strcmp(var->name, key)) return (var);
				var = var->next;
			}

			return (NULL);
		}

		char *variables_find_value(t_var **table, const char *key) {
			if (!key) return (NULL);

			unsigned int index = hash_index(key);
			t_var *var = table[index];

			while (var) {
				if (!ft_strcmp(var->name, key)) return (var->value);
				var = var->next;
			}

			return (NULL);
		}

	#pragma endregion

	#pragma region Array

		char **variables_to_array(t_var **table, int type, bool sort) {
			size_t i = 0;

			for (unsigned int index = 0; index < VARS_HASH_SIZE; index++) {
				t_var *var = table[index];
				while (var) {
					if (var->name) {
						if ((type == EXPORTED_LIST && var->exported) || (type == EXPORTED  && var->exported && var->value)) i++;
						if (type == INTERNAL && !var->exported) i++;
						if (type == READONLY && var->readonly) i++;
						var = var->next;
					}
				}
			}

			if (i == 0) return (NULL);
			char **array = smalloc((i + 1) * sizeof(char *));

			i = 0;
			for (unsigned int index = 0; index < VARS_HASH_SIZE; index++) {
				t_var *var = table[index];
				while (var) {
					if (var->value || (!var->value && type == EXPORTED_LIST)) {
						if ((type == EXPORTED_LIST || type == EXPORTED) && !var->exported)	{ var = var->next; continue; }
						if (type == INTERNAL && var->exported) 								{ var = var->next; continue; }
						if (type == READONLY && !var->readonly)								{ var = var->next; continue; }

						if (var->name) {
							array[i] = ft_strjoin_sep(var->name, "=", var->value, 0);
							i++;
						}
					}
					var = var->next;
				}
			} array[i] = NULL;

			if (sort) array_sort(array);
			return (array);
		}

	#pragma endregion

	#pragma region Print

		#pragma region Array Value

			static int array_value(int type, char **array, size_t i, t_var *var) {
				if (type < 0 || type > 4) 									return (0);
				if (!var->name)												return (0);
				if (type == EXPORTED_LIST && !var->exported)				return (0);
				if (type == EXPORTED && (!var->exported || !var->value))	return (0);
				if (type == READONLY && !var->readonly)						return (0);
				if (type == INTERNAL && var->exported)						return (0);

				char var_type[6]; int j = 0;
				var_type[j++] = '-';
				if (var->integer) var_type[j++] = 'i';
				if (var->readonly) var_type[j++] = 'r';
				if (var->exported) var_type[j++] = 'x';
				if (j == 1) var_type[j++] = '-';
				while (j < 5) var_type[j++] = ' ';
				var_type[j] = '\0';
				
				if (type == INTERNAL)	array[i] = ft_strdup(var->name);
				else 					array[i] = ft_strjoin_sep("declare ", var_type, var->name, 0);

				if (array[i] && var->value) array[i] = ft_strjoin_sep(array[i], "=", format_for_shell(var->value, '\"'), 6);

				return (1);
			}

		#pragma endregion

		#pragma region Print

			int variables_print(t_var **table, int type, bool sort) {
				size_t i = 0;

				for (unsigned int index = 0; index < VARS_HASH_SIZE; index++) {
					t_var *var = table[index];
					while (var) {
						if (var->name) i++;
						var = var->next;
					}
				}

				if (i == 0) return (1);
				char **array = smalloc((i + 1) * sizeof(char *));

				i = 0;
				for (unsigned int index = 0; index < VARS_HASH_SIZE; index++) {
					t_var *var = table[index];
					while (var) {
						i += array_value(type, array, i, var);
						var = var->next;
					}
				} array[i] = NULL;

				if (sort) array_nsort(array, 13);

				if (array && array[0]) {
					print(STDOUT_FILENO, NULL, RESET);
					for (size_t i = 0; array[i]; ++i) {
						print(STDOUT_FILENO, array[i], JOIN);
						print(STDOUT_FILENO, "\n", JOIN);
					}
					print(STDOUT_FILENO, NULL, PRINT);
				}
				if (array) array_free(array);

				return (0);
			}

		#pragma endregion

	#pragma endregion

	#pragma region Length

		size_t variables_length(t_var **table, int type) {
			size_t i = 0;

			for (unsigned int index = 0; index < VARS_HASH_SIZE; index++) {
				t_var *var = table[index];
				while (var) {
					if (var->name) {
						if (type == INTERNAL && !var->exported) i++;
						if (type == READONLY && var->readonly) i++;
						if (type == EXPORTED && var->exported && var->value) i++;
						if (type == EXPORTED_LIST && var->exported) i++;
					}
					var = var->next;
				}
			}

			return (i);
		}

	#pragma endregion

#pragma endregion

#pragma region Delete

	#pragma region Variable

		int variables_delete(t_var **table, const char *key) {
			if (!key) return (1);

			unsigned int index = hash_index(key);
			t_var *var = table[index];
			t_var *prev = NULL;

			while (var) {
				if (!ft_strcmp(var->name, key)) {
					if (prev)	prev->next = var->next;
					else		table[index] = var->next;
					sfree(var->name); sfree(var->value); sfree(var);
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
			for (unsigned int index = 0; index < VARS_HASH_SIZE; index++) {
				if (table[index]) {
					t_var *var = table[index];
					while (var) {
						t_var *next = var->next;
						sfree(var->name);
						sfree(var->value);
						sfree(var);
						var = next;
					}
					table[index] = NULL;
				}
			}
		}

	#pragma endregion

#pragma endregion

#pragma region Initialize

	static void default_add(t_var **table, const char *name, char *value, int exported, int readonly, int integer, int force, int free_value) {
		if (!value) return;
		if (force || !variables_find(table, name)) variables_add(table, name, value, exported, readonly, integer, 1);
		if (value && free_value) sfree(value);
	}

	int variables_initialize(t_var **table, const char **envp) {
		variables_from_array(vars_table, envp);

		char *home = get_home();
		if (home && (home = ft_strjoin(home, "/.42sh_history", 1))) {
			history_file_set(home);
			default_add(table, "42HISTFILE", home, 0, 0, 0, 1, 1);
		}
		default_add(table, "42HISTSIZE", "1000", 0, 0, 0, 0, 0);
		default_add(table, "42HISTFILESIZE", "2000", 0, 0, 0, 0, 0);
		default_add(table, "42HISTCONTROL", "ignoreboth", 0, 0, 0, 0, 0);

		default_add(table, "42SH", "PATH OF 42SH", 0, 0, 0, 1, 0);								//	Normal var but set value on start always
		default_add(table, "42SH_SUBSHELL", "0", 0, 0, 0, 1, 0);								//	When modified, update (shell_level with value too) - Increment subshell_level in child when fork() or subshell
		default_add(table, "42SH_VERSION", VERSION, 0, 0, 0, 1, 0);								//	Normal var but set value on start always
		default_add(table, "42SHPID", ft_itoa(shell.pid), 0, 0, 0, 1, 1);						//	Can be modified, but expand dinamic value
		default_add(table, "PPID", ft_itoa(shell.parent_pid), 0, 0, 1, 1, 1);					//	Update var when expanded (parent_pid) READONLY
		default_add(table, "COLUMNS", ft_itoa(terminal.columns), 0, 0, 1, 1, 1);				//	Update var when expanded (terminal_columns)
		default_add(table, "LINES", ft_itoa(terminal.rows), 0, 0, 1, 1, 1);						//	Update var when expanded (terminal_rows)
		default_add(table, "SECONDS", "0", 0, 0, 0, 1, 0);										//	Can be modified, but expand dinamic value
		default_add(table, "EPOCHSECONDS", ft_itoa(shell.epoch_seconds), 0, 1, 1, 1, 1);		//	Update everytime (even with env)
		default_add(table, "EPOCHREALTIME", ft_itoa(shell.epoch_realtime), 0, 1, 1, 1, 1);		//	Update everytime (even with env)
		default_add(table, "UID", ft_itoa(shell.uid), 0, 0, 1, 1, 1);							//	Update var when expanded (shell_uid) READONLY
		default_add(table, "EUID", ft_itoa(shell.euid), 0, 0, 1, 1, 1);							//	Update var when expanded (shell_euid) READONLY
		default_add(table, "PS1", terminal.PS1, 0, 0, 0, 1, 0);									//	Normal var but set value on start always
		default_add(table, "PS2", terminal.PS2, 0, 0, 0, 1, 0);									//	Normal var but set value on start always
		//	BASH_COMMAND																		//	Can be modified, but expand dinamic value (dont create on startup)
	

		return (0);
	}

#pragma endregion

#pragma region Information

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

#pragma endregion