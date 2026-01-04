/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fc.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 21:00:36 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/04 22:00:21 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "terminal/terminal.h"
	#include "terminal/readinput/history.h"
	#include "hashes/builtin.h"
	#include "hashes/variable.h"
	#include "builtins/options.h"
	#include "main/shell.h"
	#include "utils/libft.h"
	#include "utils/print.h"
	#include "utils/paths.h"
	#include "utils/getopt2.h"
	
	#include <sys/wait.h>

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Help"

		static int help() {
			char *msg =
			"fc: fc [-e ename] [-lnr] [first] [last] or fc -s [pat=rep] [command]\n"
			"    Display or execute commands from the history list.\n\n"

			"    fc is used to list or edit and re-execute commands from the history list.\n"
			"    FIRST and LAST can be numbers specifying the range, or FIRST can be a\n"
			"    string, which means the most recent command beginning with that\n"
			"    string.\n\n"

			"    Options:\n"
			"      -e ENAME  select which editor to use.  Default is FCEDIT, then EDITOR,\n"
			"                then vi\n"
			"      -l        list lines instead of editing\n"
			"      -n        omit line numbers when listing\n"
			"      -r        reverse the order of the lines (newest listed first)\n\n"

			"    With the `fc -s [pat=rep ...] [command]' format, COMMAND is\n"
			"    re-executed after the substitution OLD=NEW is performed.\n\n"

			"    A useful alias to use with this is r='fc -s', so that typing `r cc'\n"
			"    runs the last command beginning with `cc' and typing `r' re-executes\n"
			"    the last command.\n\n"

			"    Exit Status:\n"
			"      Returns success or status of executed command; non-zero if an error occurs.\n";

			print(STDOUT_FILENO, msg, RESET_PRINT);

			return (0);
		}

	#pragma endregion

	#pragma region "Version"

		static int version() {
			char *msg =
				"fc 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by Kobayashi82 (vzurera-).\n";

			print(STDOUT_FILENO, msg, RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Get Position"

	static int fc_get_position(t_parse_result *result, int offset, char *query, size_t *out) {
		if (!ft_isdigit_s(query)) {
			if (history_position_query(offset , query, out)) {
				print(STDERR_FILENO, ft_strjoin(result->shell_name, ": fc: no command found\n", 0), FREE_RESET_PRINT);
				return (1);
			}
		} else {
			int number = atoi(query);
			if (!strcmp(query, "-0")) {
				print(STDERR_FILENO, ft_strjoin(result->shell_name, ": fc: history specification out of range\n", 0), FREE_RESET_PRINT);
				return (1);
			}
			if (!strcmp(query, "0")) number = -1;
			if (history_position_offset(number, out)) history_position_offset(-1, out);
		}

		return (0);
	}

#pragma endregion

#pragma region "Replace (-s)"

	static int fc_replace(t_parse_result *result) {
		int no_command = 1;

		// Check if there are more than one command
		for (int i = 0; i < result->argc; ++i) {
			if (!strchr(result->argv[i], '=')) {
				if (i < result->argc - 1) {
					print(STDERR_FILENO, ft_strjoin(result->shell_name, ": fc: too many arguments\n", 0), FREE_RESET_PRINT);
					return (1);
				}
				no_command = 0;
				break;
			}
		}

		// Remove fc command from the history
		history_remove_last_if_added(1);

		// Set start position
		size_t start = history_position();
		if (!no_command && result->argc && fc_get_position(result, -1, result->argv[result->argc - 1], &start)) return (1);

		HIST_ENTRY *entry = history_entry_position(start);
		char *command = ft_strdup(entry->line);

		for (int i = 0; i < result->argc; ++i) {
			if (strchr(result->argv[i], '=')) {
				char *key = NULL, *value = NULL;
				get_key_value(result->argv[i], &key, &value, '=');

				if (key && *key) {
					char *find = command;
					while ((find = strstr(find, key))) {
						int len = (find - command) + ft_strlen(value);
						char *new_command = replace_substring(command, find - command, ft_strlen(key), value);
						if (!new_command) break;
						free(command);
						command = new_command;
						find = command + len;
					}
					free(key);
					free(value);
				} else {
					print(STDERR_FILENO, ft_strjoin(result->shell_name, ": fc: invalid substitution format\n", 0), FREE_RESET_PRINT);
					free(command);
					free(key);
					free(value);
					return (1);
				}
			}
		}

		// Execute command
		int ret = 0;

		// set -v  # Activa modo verbose
		// contexto del padre
		// modo no interactivo
		// shell.source = SRC_ARGUMENT;
		// ret = read_input((char *)argv[2]);
		// ret = execute_script(command);	// como con -c

		free(command);
		if (!ret) ret = shell.exit_code;
		
		return (ret);
	}

#pragma endregion

#pragma region "List (-lnr)"

	static int fc_list(t_parse_result *result) {
		// Set start and end position
		history_set_pos_last();
		size_t	start = 0, end = 0;
		start = end = history_position();
		if (result->argc > 0 && fc_get_position(result, -1, result->argv[0], &start))	return (1);
		if (result->argc > 1 && fc_get_position(result, -1, result->argv[1], &end))		return (1);
		if (end < start) {
			int tmp = start;
			start = end;
			end = tmp;
		}

		history_print_range(start, end, has_option(result, 'r'), has_option(result, 'n'));

		return (0);
	}

#pragma endregion

#pragma region "Edit (-e)"

	#pragma region "Editor"

		static int default_editor(t_parse_result *result, char **editor, const char *cmd) {
			if (!result || !editor) return (1);
			char *name = NULL;

			if (cmd) {
				name	= get_fullpath_command(cmd, 0);
				*editor = get_fullpath_command(name, 1);
			}

			if (!cmd && !*editor) {
				free(name);
				name	= get_fullpath_command(variables_find_value(vars_table, "FCEDIT"), 0);
				*editor = get_fullpath_command(name, 1);
			}
			if (!cmd && !*editor) {
				free(name);
				name	= get_fullpath_command(variables_find_value(vars_table, "EDITOR"), 0);
				*editor = get_fullpath_command(name, 1);
			}
			if (!cmd && !*editor) {
				free(name);
				name	= get_fullpath_command(variables_find_value(vars_table, "VISUAL"), 0);
				*editor = get_fullpath_command(name, 1);
			}
			if (!cmd && !*editor) {
				free(name);
				name	= get_fullpath_command("/usr/bin/editor", 0);
				*editor = get_fullpath_command(resolve_symlink(name), 1);
			}
			if (!cmd && !*editor) {
				free(name);
				name	= get_fullpath_command("nano", 0);
				*editor = get_fullpath_command(name, 1);
			}
			if (!cmd && !*editor) {
				free(name);
				name	= get_fullpath_command("ed", 0);
				*editor = get_fullpath_command(name, 1);
			}

			if (!*editor) {
				print(STDERR_FILENO, result->shell_name, RESET);
				if (cmd && name)	print(STDERR_FILENO, ft_strjoin_sep(": ", name, ": command not found\n", 0), FREE_PRINT);
				else				print(STDERR_FILENO, ": fc: editor not found\n", PRINT);
				free(name);
				return (1);
			}
		
			if (is_directory((char *)(*editor))) {
				print(STDERR_FILENO, result->shell_name, RESET);
				if (cmd && name)	print(STDERR_FILENO, ft_strjoin_sep(": ", name, ": Is a directory\n", 0), FREE_PRINT);
				else				print(STDERR_FILENO, ": editor: Is a directory\n", PRINT);
				free(name);
				free(*editor);
				*editor = NULL;
				return (1);
			}

			if (access(*editor, X_OK) == -1) {
				print(STDERR_FILENO, result->shell_name, RESET);
				if (cmd && name)	print(STDERR_FILENO, ft_strjoin_sep(": ", name, ": Permission denied\n", 0), FREE_PRINT);
				else				print(STDERR_FILENO, ": editor: Permission denied\n", PRINT);
				free(name);
				free(*editor);
				*editor = NULL;
				return (1);
			}

			free(name);
			return (0);
		}

	#pragma endregion

	#pragma region "Edit"

		static int fc_edit(t_parse_result *result) {
			// Find editor command
			char *editor = NULL;
			if (default_editor(result, &editor, get_option_value(result, 'e')))				return (1);

			// Remove fc command from the history
			history_remove_last_if_added(1);

			// Set start and end position
			history_set_pos_last();
			size_t	start = 0, end = 0;
			start = end = history_position();
			if (result->argc > 0 && fc_get_position(result, -1, result->argv[0], &start))	return (free(editor), 1);
			if (result->argc > 1 && fc_get_position(result, -1, result->argv[1], &end))		return (free(editor), 1);
			if (result->argc == 1) end = start;
			if (end < start) {
				int tmp = start;
				start = end;
				end = tmp;
			}

			// Create temp file
			int fd = tmp_find_fd_path(ft_mkdtemp(NULL, "fc_edit"));
			if (fd == -1) {
				print(STDERR_FILENO, result->shell_name, RESET);
				print(STDERR_FILENO, ft_strjoin_sep(": fc: cannot create temp file: ", strerror(errno), "\n", 0), FREE_PRINT);
				free(editor);
				return (1);
			}

			for (size_t i = start; i <= end; ++i) {
				HIST_ENTRY *entry = history_entry_position(i);
				if (entry && entry->line) {
					if (i != start) write(fd, "\n", 1);
					if (write(fd, entry->line, entry->length) == -1) {
						free(editor);
						tmp_delete_fd(fd);
						print(STDERR_FILENO, result->shell_name, RESET);
						print(STDERR_FILENO, ft_strjoin_sep(": fc: cannot write to temp file: ", strerror(errno), "\n", 0), FREE_PRINT);
						return (1);
					}
				}
			}
			close(fd);

			// Fork and execute the editor
			char *tmp_file = tmp_find_path_fd(fd);

			pid_t pid = fork();
			if (pid < 0) {	// Error
				free(editor);
				tmp_delete_path(tmp_file);
				print(STDERR_FILENO, result->shell_name, RESET);
				print(STDERR_FILENO, ft_strjoin_sep(": fc: cannot fork: ", strerror(errno), "\n", 0), FREE_PRINT);
				return (1);
			}
			if (pid == 0) {	// Child
				close(terminal.bk_stdin);
				close(terminal.bk_stdout);
				close(terminal.bk_stderr);
				char *const args[] = { editor, tmp_file, NULL};
				char **env = variables_to_array(vars_table, EXPORTED, 1);
				execve(editor, args, env);
				free(editor);
				tmp_delete_path(tmp_file);
				array_free(env);
				ast_free(&shell.ast);
				for (int i = 0; result->argv_original && result->argv_original[i]; ++i) free(result->argv_original[i]);
				free(result->argv_original);
				free_options(result);
				exit_error(NOTHING, 1, NULL, 1);
			}
			if (pid > 0) {	// Parent
				int status;
				waitpid(pid, &status, 0);
				if		(WIFEXITED(status))		status = WEXITSTATUS(status);
				else if	(WIFSIGNALED(status))	status = 128 + WTERMSIG(status);

				if (status) {
					free(editor);
					tmp_delete_path(tmp_file);
					return (1);
				}
			}

			// Execute tmp_file
			int ret = 0;
			free(editor);

			// set -v  # Activa modo verbose
			// contexto del padre
			// modo no interactivo
			// ret = execute_script(tmp_file);

			tmp_delete_path(tmp_file);
			if (!ret) ret = shell.exit_code;

			return (ret);
		}

	#pragma endregion

#pragma endregion

#pragma region "FC (Fix Command)"

	int bt_fc(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",    NO_ARGUMENT, 0},
			{"version", NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, "se:lnr", NULL, long_opts, "fc [-e ename] [-lnr] [first] [last] or fc -s [pat=rep] [command]", 1);

		if (!result)		return (1);
		if (result->error)	return (free_options(result), 1);

		if (find_long_option(result, "help"))		return (free_options(result), help());
		if (find_long_option(result, "version"))	return (free_options(result), version());

		int ret = 0;

		if (!result->options)			{ ret = fc_edit(result);			return (free_options(result), ret); }
		if (has_option(result, 's'))	{ ret = fc_replace(result);			return (free_options(result), ret); }
		if (has_option(result, 'l'))	{ ret = fc_list(result);			return (free_options(result), ret); }
		if (has_option(result, 'e'))	{ ret = fc_edit(result);			return (free_options(result), ret); }

		return (free_options(result), ret);
	}

#pragma endregion

#pragma region "Info"

	// fc [-e ename] [first] [last]
	//
	// -e ENAME  select which editor to use.  Default is FCEDIT, then EDITOR, then vi
	//
	//	el comando propio (fc ...) no se añade al historial (o se elimina el comando y el evento se reduce en 1)
	//	[ENAME] tiene que ser un programa válido (absoluto o relativo) (ojo, porque parece que no muestra programas interactivos, por ejemplo fc -e ./42sh echo)
	//	si no se indica [ENAME] se usará la variable EDITOR, si no, VISUAL, si no, se buscará en "/usr/bin/editor" y por ultimo se usará "nano". Si todo eso falla, muestra un error
	//	[first] [last] son números de eventos del comando
	//	si [first] [last] no se indican, se utilizará el último comando
	//	si [-first] [-last] se aplica offset a partir del último comando
	//	si [first] solo, se aplica desde el último comando hasta [first]
	//	si [first] es un string, se muestra desde el último comando hasta la primera coincidencia (empezando por la cadena)
	//	si [first] [last] son strings, utiliza los rangos de las coincidencias. Siempre se empieza la busqueda desde el último comando independientemente de si es [first] o [last]

	//	si hay fallo de sintaxis en un comando de la lista, se muestra el error y no se siguen ejecutando comandos, pero hasta llegar ahí si se ejecutan

	// fc -s [pat=rep] [command]
	//
	//	reemplaza y ejecuta (añade el comando al historial)
	//	pueden haber varios reemplazos (fc -s a=b b=c c=d)
	//	[command] es el número de evento del comando
	//	si [command] no se indica, se utilizará el último comando
	//	si [+command] se aplica offset a partir del primer comando
	//	si [-command] se aplica offset a partir del último comando
	//	si [command] es un string, se aplica la primera coincidencia empezando desde el último comando
	//	

	// fc [-lnr] [first] [last]
	//
	// -l        list lines instead of editing
	// -n        omit line numbers when listing
	// -r        reverse the order of the lines (newest listed first)
	//
	//	[-nr] no tienen efecto (se ignoran) si [-l] no está.
	//	[first] [last] son números de eventos del comando
	//	si [first] [last] no se indican, se utilizarán los últimos 16 comandos
	//	si [-first] [-last] se aplica offset a partir del último comando
	//	si [first] solo, se aplica desde el último comando hasta [first]
	//	si [first] es un string, se muestra desde el último comando hasta la primera coincidencia (empezando por la cadena)
	//	si [first] [last] son strings, utiliza los rangos de las coincidencias. Siempre se empieza la busqueda desde el último comando independientemente de si es [first] o [last]

	// Nota:	si se juntan flags, tiene prioridad [--help], [--version], [-s], [-l], [-e]
	//			pero si está [-e], por ejemplo (fc -sle vim 500) se ignora vim si existe un argumento más, si solo está vim, se considera [command]

#pragma endregion
