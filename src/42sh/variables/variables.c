/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 17:39:40 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/11 13:43:41 by vzurera-         ###   ########.fr       */
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

		int variables_add(t_var **table, const char *name, const char *value, int exported, int readonly, int integer, int force) {
			if (!name) return (0);

			t_var *new_var = variables_find(table, name);
			if (new_var) {
				if (!readonly || force) {
					free(new_var->value);
					new_var->value = ft_strdup(value);
					new_var->readonly = readonly;
					new_var->exported = exported;
					new_var->integer = integer;
					return (0);
				} else return (1);
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

			return (0);
		}

	#pragma endregion

	#pragma region Array

		void variables_from_array(t_var **table, char **array) {
			if (!array) return;

			for (size_t i = 0; array[i]; i++) {
				char *key = NULL, *value = NULL;
				get_key_value(array[i], &key, &value, '=');
				if (!key) continue;
				variables_add(table, key, value, 1, 0, 0, 0);
				free(key); free(value);
			}
		}

#pragma endregion

	#pragma region Join

		void variables_join(t_var **dst_table, t_var **src_table) {
			for (int index = 0; index < HASH_SIZE; index++) {
				t_var *var = src_table[index];
				while (var) {
					variables_add(dst_table, var->name, var->value, var->exported, var->readonly, var->integer, 0);
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

#pragma region Initialize

	static void default_add(t_var **table, const char *name, char *value, int exported, int readonly, int integer, int force, int free_value) {
		if (!value) return;
		if (force || !variables_find(table, name)) variables_add(table, name, value, exported, readonly, integer, 1);
		if (value && free_value) free(value);
	}

	void variables_initialize(t_var **table) {
		default_add(table, "42HISTFILE", ft_strjoin(home_path(), "/.42sh_history", 0), 0, 0, 0, 0, 1);
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
	}

#pragma endregion

#pragma region Information

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