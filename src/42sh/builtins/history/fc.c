/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fc.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 21:00:36 by vzurera-          #+#    #+#             */
/*   Updated: 2025/03/15 23:49:39 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"
	#include "terminal/print.h"
	#include "terminal/readinput/history.h"
	#include "parser/tokenizer/args.h"
	#include "parser/expansions/history.h"
	#include "parser/syntax/syntax.h"
	#include "hashes/key_value.h"
	#include "builtins/builtins.h"
	#include "builtins/options.h"

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

	int fc_replace(t_opt *opts) {
		int result = 0;
		char *query = NULL;
		char *command = NULL;

		if (ft_strchr(opts->valid, 'e') && opts->args) {
			if (opts->args->value && !ft_strchr(opts->args->value, '=')) {
				if (opts->args->prev)	opts->args->prev->next = opts->args->next;
				else					opts->args = opts->args->next;
			}
		}

		t_arg *start = opts->args;
		while (opts->args) {
			if (opts->args->value && !ft_strchr(opts->args->value, '=')) {
				query = opts->args->value;
				break;
			}
			opts->args = opts->args->next;
		}
		opts->args = start;

		if (query) {
			if (!ft_isdigit_s(query)) {
				size_t last_pos = history_length();
				if (last_pos--) {
					for (int i = last_pos; i > 0; i--) {
						HIST_ENTRY * hist = history_get(i);
						if (!hist) {
							result = 1;
							break;
						}
						if (!ft_strncmp(hist->line, query, ft_strlen(query))) {
							command = ft_strdup(hist->line);
							break;
						}
					}
				} else result = 1;
			} else {
				int number = ft_atoi(query);
				if (number == 0) result = 1;
				else if (*query == '-' ||  *query == '+') {
					if (ft_strlen(query) > 7 || history_length() < (size_t) ft_abs(number)) {
						result = 1;
					} else {
						size_t pos = number < 0 ?  history_length() < (size_t) ft_abs(number) : number - 1;
						HIST_ENTRY *hist = history_get(pos);
						if (!hist) result = 1;
						else command = ft_strdup(hist->line);
					}
				} else {
					HIST_ENTRY *hist = history_event(number);
					if (!hist) result = 1;
					else command = ft_strdup(hist->line);
				}
			}
		} else {
			size_t last_pos = history_length();
			if (last_pos--) {
				HIST_ENTRY *hist = history_get(last_pos);
				if (!hist) result = 1;
				else command = ft_strdup(hist->line);
			} else result = 1;
		}

		if (!command) result = 1;
		if (result) print(STDOUT_FILENO, "fc: no command found\n", RESET_PRINT);
		else {
			while (opts->args) {
				if (opts->args->value && ft_strchr(opts->args->value, '=')) {
					char *key = NULL, *value = NULL;
					get_key_value(opts->args->value, &key, &value, '=');
					if (key && value) {
						char *find = command;
						while ((find = ft_strstr(find, key))) {
							int len = (find - command) + ft_strlen(value);
							char *new_command = replace_substring(command, find - command, ft_strlen(key), value);
							if (!new_command) break;
							sfree(command);
							command = new_command;
							find = command + len;
						}
						sfree(key); sfree(value);
					}
				}
				opts->args = opts->args->next;
			}

			if (command && !ft_isspace_s(command)) {
				history_add(command, false);
				print(STDOUT_FILENO, ft_strjoin_sep("Ejecuto: ", command, "\n", 2), FREE_RESET_PRINT);
			}
		}

		return (result);
	}

#pragma endregion

#pragma region "FC (Fix Command)"

	int fc(t_arg *args) {
		t_opt *opts = parse_options(args, "elnrs", '-', false);
		
		if (*opts->invalid) {
			invalid_option("fc", opts->invalid, "[-e ename] [-lnr] [first] [last] or fc -s [pat=rep] [command]");
			return (sfree(opts), 1);
		}
		

		if (ft_strchr(opts->valid, '?')) return (sfree(opts), print_help());
		if (ft_strchr(opts->valid, '#')) return (sfree(opts), print_version("fc", "1.0"));
		
		history_remove_last_if_added(true);
		int result = 0;
		if (!*opts->valid) {

		}

		if (*opts->valid) {
			if (ft_strchr(opts->valid, 's')) {
				result = fc_replace(opts);
			}
		}

		return (sfree(opts), result);
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
