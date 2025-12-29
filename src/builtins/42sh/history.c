/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 21:02:57 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/29 18:54:30 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// -[n] lo trata como opcion

#pragma region "Includes"

	#include "terminal/readinput/history.h"
	#include "hashes/builtin.h"
	#include "builtins/options.h"
	#include "utils/libft.h"
	#include "utils/print.h"
	
	#include "tests/args.h"

#pragma endregion

#pragma region "Help"

	static int print_help() {
		char *msg =
		"history: history [-c] [-d offset] [n] or history -rw [filename]\n"
		"    Display or manipulate the history list.\n\n"

		"    Display the history list with line numbers\n"
		"    An argument of [n] lists only the last [n] entries.\n\n"

		"    Options:\n"
		"      -c        clear the history list by deleting all of the entries\n"
		"      -d offset delete the history entry at position OFFSET. Negative\n"
		"                offsets count back from the end of the history list\n\n"

		"      -r        read the history file and overwrite the current history\n"
		"      -w        write the current history to the history file\n\n"

		"      -p        perform history expansion on each ARG and display the result\n"
		"                without storing it in the history list\n"
		"      -s        append the ARGs to the history list as a single entry\n\n"

		"    If FILENAME is given, it is used as the history file. Otherwise,\n"
		"    if HISTFILE has a value, that is used, else ~/.42sh_history.\n\n"

		"    Exit Status:\n"
		"      Returns success unless an invalid option is given or an error occurs.\n";

		print(STDOUT_FILENO, msg, RESET_PRINT);

		return (0);
	}

#pragma endregion

#pragma region "History"

	int bt_history(t_arg *args) {
		t_opt *opts = parse_options_old(args, "cdsprw", '-', 0);

		if (*opts->invalid) {
			invalid_option("history", opts->invalid, "[-c] [-d offset] [n] or history -rw [filename]");
			return (free(opts), 1);
		}

		if (strchr(opts->valid, '?')) return (free(opts), print_help());
		if (strchr(opts->valid, '#')) return (free(opts), print_version("history", "1.0"));
		
		int result = 0;
		if (!*opts->valid) {
			if (!opts->args) {
				history_print(history_length(), 0);
			} else {
				if (opts->args->value && (*opts->args->value == '-' ||  *opts->args->value == '+' ||  !ft_isdigit_s(opts->args->value))) {
					print(STDOUT_FILENO, ft_strjoin_sep("history: ", opts->args->value, ": numeric argument required\n", 0), FREE_RESET_PRINT);
					result = 1;
				} else {
					history_print(atoi(opts->args->value), 0);
				}
			}
		}

		if (*opts->valid) {
			if (strchr(opts->valid, 'c')) {
				history_clear();
			} else if (strchr(opts->valid, 'd')) {
				if (!opts->args) {
					print(STDOUT_FILENO, "history: -d: option requires an argument\n", RESET_PRINT);
					result = 2;
				} else {
					if (!opts->args->value || !ft_isdigit_s(opts->args->value)) {
						print(STDOUT_FILENO, ft_strjoin_sep("history: ", opts->args->value, ": numeric argument required\n", 0), FREE_RESET_PRINT);
						result = 1;
					} else {
						int number = atoi(opts->args->value);
						if (*opts->args->value == '-' ||  *opts->args->value == '+') {
							if (ft_strlen(opts->args->value) > 7 || history_length() < (size_t) abs(number)) {
								print(STDOUT_FILENO, ft_strjoin_sep("history: ", opts->args->value, ": position out of range\n", 0), FREE_RESET_PRINT);
								result = 1;
							} else
								history_remove_offset(number);
						} else {
							HIST_ENTRY *hist = history_event(number);
							if (!hist) {
								print(STDOUT_FILENO, ft_strjoin_sep("history: ", opts->args->value, ": event number not found\n", 0), FREE_RESET_PRINT);
								result = 1;
							} else
								history_remove_event(number);
						}
					}
				}
			} else if (strchr(opts->valid, 's')) {
				history_remove_last_if_added(1);
				char *line = NULL;
				while (opts->args) {
					line = ft_strjoin(line, opts->args->value, 1);
					if (opts->args->next) line = ft_strjoin(line, " ", 1);
					opts->args = opts->args->next;
				}
				if (line) history_add(line, 0);
				free(line);
			} else if (strchr(opts->valid, 'p')) {
				history_remove_last_if_added(1);
				print(STDOUT_FILENO, NULL, RESET);
				while (opts->args) {
					char *line = ft_strdup(opts->args->value);
					expand_history(&line, 0);
					print(STDOUT_FILENO, line, FREE_JOIN);
					print(STDOUT_FILENO, "\n", JOIN);
					opts->args = opts->args->next;
				}
				print(STDOUT_FILENO, NULL, PRINT);
			} else if (strchr(opts->valid, 'w')) {
				if (!opts->args) history_write(NULL);
				else {
					if (!access(opts->args->value, F_OK) && access(opts->args->value, W_OK)) {
						print(STDOUT_FILENO, ft_strjoin_sep("history: ", opts->args->value, ": file is not writable\n", 0), FREE_RESET_PRINT);
						result = 1;
					} else history_write(opts->args->value);
				}
			} else if (strchr(opts->valid, 'r')) {
				if (!opts->args) history_read(NULL);
				else {
					if (access(opts->args->value, F_OK)) {
						print(STDOUT_FILENO, ft_strjoin_sep("history: ", opts->args->value, ": file does not exist\n", 0), FREE_RESET_PRINT);
						result = 1;
					} else if (access(opts->args->value, W_OK)) {
						print(STDOUT_FILENO, ft_strjoin_sep("history: ", opts->args->value, ": file is not readable\n", 0), FREE_RESET_PRINT);
						result = 1;
					} else history_read(opts->args->value);
				}
			}
		}

		return (free(opts), result);
	}

#pragma endregion

// -d[+-n]			Elimina el evento desde el inicio/fin o con el numero de evento
// -r [filename]	Carga todo el archivo en el historial actual	([filename], HISTFILE or ~/.bash_history)
// -w [filename]	Guarda el historial actual en el archivo		([filename], HISTFILE or ~/.bash_history)

// Nota:	si se juntan flags, tiene prioridad [--help], [--version], [-c], [-d], [-s], [-p], [-w], [-r]
