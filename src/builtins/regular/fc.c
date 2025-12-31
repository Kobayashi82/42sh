/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fc.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 21:00:36 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/31 23:10:55 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// -[n] lo trata como opcion

#pragma region "Includes"

	#include "terminal/readinput/history.h"
	#include "hashes/variable.h"
	#include "hashes/builtin.h"
	#include "builtins/options.h"
	#include "main/shell.h"
	#include "utils/libft.h"
	#include "utils/paths.h"
	#include "utils/print.h"

	#include <sys/wait.h>

	#include <stdio.h>	// Quitar dependencias
	#include "tests/args.h"
	
#pragma endregion

#pragma region "Help"

	static int print_help() {
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

#pragma region "Replace (-s)"

	static int fc_replace(t_opt *opts) {
		int result = 0;
		char *query = NULL;
		char *command = NULL;

		if (strchr(opts->valid, 'e') && opts->args) {
			if (opts->args->value && !strchr(opts->args->value, '=')) {
				if (opts->args->prev)	opts->args->prev->next = opts->args->next;
				else					opts->args = opts->args->next;
			}
		}

		t_arg *start = opts->args;
		while (opts->args) {
			if (opts->args->value && !strchr(opts->args->value, '=')) {
				query = opts->args->value;
				break;
			}
			opts->args = opts->args->next;
		}
		opts->args = start;

		char *hist_fc_command = NULL;
		HIST_ENTRY *hist_curr = history_entry_last_if_added();
		if (hist_curr) hist_fc_command = ft_strdup(hist_curr->line);
		history_remove_last_if_added(1);

		if (query) {
			if (!ft_isdigit_s(query)) {
				size_t last_pos = history_length();
				if (last_pos--) {
					for (int i = last_pos; i > 0; i--) {
						HIST_ENTRY * hist = history_entry_position(i);
						if (!hist) {
							result = 1;
							break;
						}
						if (!strncmp(hist->line, query, ft_strlen(query))) {
							command = ft_strdup(hist->line);
							break;
						}
					}
				} else result = 1;
			} else {
				int number = atoi(query);
				if (number == 0) result = 1;
				else if (*query == '-' ||  *query == '+') {
					if (ft_strlen(query) > 7 || history_length() < (size_t) abs(number)) {
						result = 1;
					} else {
						size_t pos = number < 0 ?  history_length() - (size_t) abs(number) : number - 1;
						HIST_ENTRY *hist = history_entry_position(pos);
						if (!hist) result = 1;
						else command = ft_strdup(hist->line);
					}
				} else {
					HIST_ENTRY *hist = history_entry_event(number);
					if (!hist) result = 1;
					else command = ft_strdup(hist->line);
				}
			}
		} else {
			size_t last_pos = history_length();
			if (last_pos--) {
				HIST_ENTRY *hist = history_entry_position(last_pos);
				if (!hist) result = 1;
				else command = ft_strdup(hist->line);
			} else result = 1;
		}

		if (!command) result = 1;
		if (result) print(STDOUT_FILENO, "fc: no command found\n", RESET_PRINT);
		else {
			printf("%s\n", command);	// No se si era debug
			while (opts->args) {
				if (opts->args->value && strchr(opts->args->value, '=')) {
					char *key = NULL, *value = NULL;
					get_key_value(opts->args->value, &key, &value, '=');
					if (key && value) {
						char *find = command;
						while ((find = strstr(find, key))) {
							int len = (find - command) + ft_strlen(value);
							char *new_command = replace_substring(command, find - command, ft_strlen(key), value);
							if (!new_command) break;
							free(command);
							command = new_command;
							find = command + len;
						}
						free(key); free(value);
					}
				}
				opts->args = opts->args->next;
			}

			if (command && !ft_isspace_s(command)) {
				print(STDOUT_FILENO, ft_strjoin_sep("Ejecuto (alias, sintaxis, etc...): ", command, "\n", 2), FREE_RESET_PRINT);
			}
		}

		if (result && hist_fc_command) history_add(hist_fc_command, 0);
		free(hist_fc_command);

		return (result);
	}

#pragma endregion

#pragma region "List (-lnr)"

	#pragma region "Add"

		static void fc_list_add(int i, int hide_events) {
			HIST_ENTRY *hist = history_entry_position(i);
			if (hist && hist->line) {
				if (!hide_events) {
					char *txt_event = ft_itoa(hist->event);
					int spaces = 4 - ft_strlen(txt_event);
					while (spaces--) print(STDOUT_FILENO, " ", JOIN);
					print(STDOUT_FILENO, txt_event, FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin_sep("     ", hist->line, "\n", 0), FREE_JOIN);
				} else
					print(STDOUT_FILENO, ft_strjoin_sep("         ", hist->line, "\n", 0), FREE_JOIN);
			}
		}

	#pragma endregion

	#pragma region "GetPos"

		static int fc_list_getpos(char *query, int reduce_one) {
			if (query) {
				if (!ft_isdigit_s(query)) {
					int last_pos = history_length();
					last_pos = ft_max(last_pos - 1 - reduce_one, 0);
					if (last_pos) {
						for (int i = last_pos; i > 0; i--) {
							HIST_ENTRY * hist = history_entry_position(i);
							if (hist && !strncmp(hist->line, query, ft_strlen(query))) return (i);
						} return (-2);
					} else return (-2);
				} else {
					int number = atoi(query);
					if (number == 0) return (-1);
					else if (*query == '-' ||  *query == '+') {
						if (ft_strlen(query) > 7 || history_length() < (size_t) abs(number))
							return (-1);
						else {
							size_t pos = number < 0 ?  history_length() - (size_t) abs(number) : number - 1;
							HIST_ENTRY *hist = history_entry_position(pos);
							if (hist) return (pos);
						}
					} else {
						size_t pos = 0;
						return ((history_position_event(number, &pos)) ? -1 : pos);
					}
				}
			}

			return (-1);
		}

	#pragma endregion

	#pragma region "List"

		static int fc_list(t_opt *opts) {
			int result = 0, start_pos = 0, end_pos = 0, last_pos = 0;
			int reduce_one = (history_entry_last_if_added() != NULL);

			if (strchr(opts->valid, 'e') && opts->args) {
				if (opts->args->prev)	opts->args->prev->next = opts->args->next;
				else					opts->args = opts->args->next;
			}

			history_set_pos_last();
			last_pos = history_position();
			last_pos = ft_max(last_pos - reduce_one, 0);
		
			if (!opts->args) {
				end_pos = last_pos;
				start_pos = ft_max(end_pos - 15, 0);
			}
			
			if (opts->args) {
				start_pos = fc_list_getpos(opts->args->value, reduce_one);
				opts->args = opts->args->next;
				if (start_pos == -2) result = 1;
				else if (start_pos == -1) start_pos = 0;
			}

			if (!result) {
				if (opts->args) {
					end_pos = fc_list_getpos(opts->args->value, reduce_one);
					if (end_pos == -2) result = 1;
					else if (end_pos == -1) end_pos = last_pos;
				} else end_pos = last_pos;
			}

			if (!result) {
				if (end_pos < start_pos) {
					int tmp = start_pos;
					start_pos = end_pos;
					end_pos = tmp;
				}
				
				int hide_events = strchr(opts->valid, 'n') != NULL;
				print(STDOUT_FILENO, NULL, RESET);
				if (strchr(opts->valid, 'r'))
					for (int i = end_pos; i >= start_pos; --i) fc_list_add(i, hide_events);
				else
					for (int i = start_pos; i <= end_pos; ++i) fc_list_add(i, hide_events);
				print(STDOUT_FILENO, NULL, PRINT);
			}

			if (result) print(STDOUT_FILENO, "fc: no command found\n", RESET_PRINT);

			return (result);
		}

	#pragma endregion

#pragma endregion

#pragma region "Edit (-e)"

	#pragma region "Editor"

		const char *default_editor() {
			const char	*editor = variables_find_value(vars_table, "FCEDIT");
			if (!editor || !*editor) editor = variables_find_value(vars_table, "EDITOR");
			if (!editor || !*editor) editor = variables_find_value(vars_table, "VISUAL");
			if (!editor || !*editor) editor = resolve_symlink("/usr/bin/editor");
			if (!editor || !*editor) editor = "nano"; // o como ultima instacia ed...
			return (editor);
		}

	#pragma endregion

	#pragma region "Edit"

		static int fc_edit(t_opt *opts, char *editor) {
			int result = 0, start_pos = 0, end_pos = 0, last_pos = 0;

			// Remove fc command from history
			// char *hist_fc_command = NULL;
			// HIST_ENTRY *hist_curr = history_entry_last_if_added();
			// if (hist_curr) hist_fc_command = ft_strdup(hist_curr->line);
			history_remove_last_if_added(1);

			// Set first and last
			history_set_pos_last();
			last_pos = history_position();
			last_pos = ft_max(last_pos, 0);

			if (!opts->args) {
				end_pos = last_pos;
				start_pos = last_pos;
			}

			if (opts->args) {
				start_pos = fc_list_getpos(opts->args->value, 0);
				opts->args = opts->args->next;
				if (start_pos == -2) result = 1;
				else if (start_pos == -1) start_pos = 0;
			}

			if (!result) {
				if (opts->args) {
					end_pos = fc_list_getpos(opts->args->value, 0);
					if (end_pos == -2) result = 1;
					else if (end_pos == -1) end_pos = last_pos;
				} else end_pos = last_pos;
			}

			// Create temp file
			int fd = -1;
			if (!result) {
				fd = tmp_find_fd_path(ft_mkdtemp(NULL, "fc_edit"));
				if (fd == -1) result = 2;
				else {
					if (end_pos < start_pos) {
						int tmp = start_pos;
						start_pos = end_pos;
						end_pos = tmp;
					}
					for (int i = start_pos; i <= end_pos; ++i) {
						HIST_ENTRY *hist = history_entry_position(i);
						if (hist && hist->line) {
							if (i != start_pos) write(fd, "\n", 1);
							if (write(fd, hist->line, hist->length) == -1) {
								tmp_delete_fd(fd);
								result = 2;
								break;
							}
						}
					}
					close(fd);
				}
			}
			if (!result && fd == -1) result = 2;

			// Abre el editor
			char *tmp_file = NULL;
			if (!result) {
				tmp_file = tmp_find_path_fd(fd);

				pid_t pid = fork();
				if (pid < 0) {
					// fork error
					result = 4;
				} else if (pid == 0) {
					char *const args[] = { editor, tmp_file, NULL};
					char **env = variables_to_array(vars_table, EXPORTED, 1);
					execve(editor, args, env);
					exit(1);
				} else if (pid > 0) {
					int status;
					waitpid(pid, &status, 0);
					if (WIFEXITED(status))			status = WEXITSTATUS(status);
					else if (WIFSIGNALED(status))	status = 128 + WTERMSIG(status);

					if (status || (fd = open(tmp_file, O_RDONLY)) == -1) result = 3;
				}
			}

			// Abre y lee el archivo modificado
			char *file_content = NULL;
			if (!result) {
				char	temp_buffer[1024];
				size_t	file_size = 0;
				int		readed = 0;

				while ((readed = read(fd, temp_buffer, sizeof(temp_buffer))) > 0) {
					file_content = realloc(file_content, file_size + readed + 1);
					memcpy(file_content + file_size, temp_buffer, readed);
					file_size += readed;
				}

				if (readed < 0) {
					unlink(tmp_file);
					free(file_content);
					file_content = NULL;
					result = 3;
				} else {
					if (file_content) {

						file_content[file_size] = '\0';
						
						if (file_size > 0 && file_content[file_size - 1] == '\n') {
							file_content[file_size - 1] = '\0';
							file_size--;
						}
						if (file_size > 0 && file_content[file_size - 1] == '\r') {
							file_content[file_size - 1] = '\0';
							file_size--;
						}
					}
				}
			}

			if (result == 1) print(STDOUT_FILENO, "fc: no command found\n", RESET_PRINT);
			if (result == 2) print(STDOUT_FILENO, "fc: crear\n", RESET_PRINT);
			if (result == 3) print(STDOUT_FILENO, "fc: leer\n", RESET_PRINT);

			free(editor);
			tmp_delete_path(tmp_file);

			// Add fc command to history
			if (!result && file_content) {
				history_add(file_content, 0);
				print(STDOUT_FILENO, ft_strjoin(file_content, "\n", 1), FREE_RESET_PRINT);
			}

			return (result);
		}

	#pragma endregion

#pragma endregion

#pragma region "FC (Fix Command)"

	int bt_fc(t_arg *args) {
		t_opt *opts = parse_options_old(args, "elnrs", '-', 0);
		
		if (*opts->invalid) {
			invalid_option("fc", opts->invalid, "[-e ename] [-lnr] [first] [last] or fc -s [pat=rep] [command]");
			return (free(opts), 1);
		}

		if (strchr(opts->valid, '?')) return (free(opts), print_help());
		if (strchr(opts->valid, '#')) return (free(opts), print_version("fc", "1.0"));
		
		int result = 0;
		if (!*opts->valid) {
			char *editor = get_fullpath((char *)default_editor());
			if (access(editor, X_OK) == -1) {
				print(STDERR_FILENO, ft_strjoin(editor, ": command not found\n", 1), FREE_RESET_PRINT);
				free(editor);
				result = 1;
			} else {
				result = fc_edit(opts, editor);
			}
		}

		if (*opts->valid) {
			if (strchr(opts->valid, 's')) {
				result = fc_replace(opts);
			} else if (strchr(opts->valid, 'l')) {
				result = fc_list(opts);
			} else if (strchr(opts->valid, 'e')) {
				if (!opts->args) {
					print(STDOUT_FILENO, "fc: -e: option requires an argument\n", RESET_PRINT);
					result = 1;
				} else {
					char *editor = get_fullpath(opts->args->value);
					if (access(editor, X_OK) == -1) {
						print(STDERR_FILENO, ft_strjoin(editor, ": command not found\n", 1), FREE_RESET_PRINT);
						free(editor);
						result = 1;
					} else {
						opts->args = opts->args->next;
						result = fc_edit(opts, editor);
					}
				}
			}
		}

		return (free(opts), (result != 0));
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
	//	si [+first] [+last] se aplica offset a partir del primer comando
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
	//	si [+first] [+last] se aplica offset a partir del primer comando
	//	si [-first] [-last] se aplica offset a partir del último comando
	//	si [first] solo, se aplica desde el último comando hasta [first]
	//	si [first] es un string, se muestra desde el último comando hasta la primera coincidencia (empezando por la cadena)
	//	si [first] [last] son strings, utiliza los rangos de las coincidencias. Siempre se empieza la busqueda desde el último comando independientemente de si es [first] o [last]

	// Nota:	si se juntan flags, tiene prioridad [--help], [--version], [-s], [-l], [-e]
	//			pero si está [-e], por ejemplo (fc -sle vim 500) se ignora vim si existe un argumento más, si solo está vim, se considera [command]

#pragma endregion
