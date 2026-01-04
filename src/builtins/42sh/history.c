/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 21:02:57 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/04 21:54:55 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "terminal/readinput/history.h"
	#include "hashes/builtin.h"
	#include "builtins/options.h"
	#include "utils/libft.h"
	#include "utils/print.h"
	#include "utils/getopt2.h"

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Help"

		static int help() {
			char *msg =
			"history: history [-c] [-d offset] [n] or history -anrw [filename] or history -ps arg [arg...]\n"
			"    Display or manipulate the history list.\n\n"

			"    Display the history list with line numbers\n"
			"    An argument of [n] lists only the last [n] entries.\n\n"

			"    Options:\n"
			"      -c        clear the history list by deleting all of the entries\n"
			"      -d offset delete the history entry at position OFFSET. Negative\n"
			"                offsets count back from the end of the history list\n\n"

			"      -a        append history commands from this session to the history file\n"
			"      -n        read all history commands not already read from the history file\n"
			"                and append them to the history list before session commands\n"
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

	#pragma region "Version"

		static int version() {
			char *msg =
				"history 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by Kobayashi82 (vzurera-).\n";

			print(STDOUT_FILENO, msg, RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Internal"

	#pragma region "Print"

		static int print_history(t_parse_result *result) {
			if (!result->argc) {
				history_print(history_length(), 0);
			} else {
				if (!ft_isdigit_s(result->argv[0])) {
					print(STDERR_FILENO, result->shell_name, RESET);
					print(STDERR_FILENO, ft_strjoin_sep(": history: ", result->argv[0], ": numeric argument required\n", 0), FREE_PRINT);
					return (1);
				} else {
					history_print(atoi(result->argv[0]), 0);
				}
			}

			return (0);
		}

	#pragma endregion

	#pragma region "(-s) Append Args"

		static int append_args(t_parse_result *result) {
			if (!result->argc) return (0);

			history_remove_last_if_added(1);

			char *line = NULL;
			for (int i = 0; i < result->argc; i++) {
				line = ft_strjoin(line, result->argv[i], 1);
				if (i + 1 < result->argc) line = ft_strjoin(line, " ", 1);
			}

			if (line) {
				history_add(line, 0);
				free(line);
			}

			return (0);
		}

	#pragma endregion

	#pragma region "(-p) Expansion"

		static int expansion_history(t_parse_result *result) {
			if (!result->argc) return (0);

			history_remove_last_if_added(1);

			print(STDOUT_FILENO, NULL, RESET);

			for (int i = 0; i < result->argc; i++) {
				char *line = ft_strdup(result->argv[i]);
				expand_history(&line, 0);
				print(STDOUT_FILENO, ft_strjoin(line, "\n", 1), FREE_JOIN);
			}

			print(STDOUT_FILENO, NULL, PRINT);

			return (0);
		}

	#pragma endregion

	#pragma region "(-d) Delete"

		static int parse_history_range(const char *offset, size_t *start, size_t *end) {
			char *dash = strchr(offset + 1, '-');

			if (!dash)												return (1);
			*dash = '\0';
			if (!ft_isdigit_s(offset) || !ft_isdigit_s(dash + 1))	return (1);
			if (history_position_offset(atoi(offset), start))		return (1);
			if (history_position_offset(atoi(dash + 1), end))		return (1);

			return (0);
		}

		static int delete_history(t_parse_result *result) {
			const char *offset_str = get_option_value(result, 'd');

			if (!offset_str) {
				print(STDERR_FILENO, result->shell_name, RESET);
				print(STDERR_FILENO, ": history: -d: option requires an argument\n", PRINT);
				return (2);
			}

			size_t start = 0, end = 0;

			if (strchr(offset_str + 1, '-')) {
				if (parse_history_range(offset_str, &start, &end)) {
					print(STDERR_FILENO, result->shell_name, RESET);
					print(STDERR_FILENO, ft_strjoin_sep(": history: ", offset_str, ": history position out of range\n", 0), FREE_PRINT);
					return (1);
				}
				history_remove_offset_range(start, end);
				return (0);
			}

			if (!ft_isdigit_s(offset_str)) {
				print(STDERR_FILENO, result->shell_name, RESET);
				print(STDERR_FILENO, ft_strjoin_sep(": history: ", offset_str, ": history position out of range\n", 0), FREE_PRINT);
				return (1);
			}

			if (history_remove_offset(atoi(offset_str))) {
				print(STDERR_FILENO, result->shell_name, RESET);
				print(STDERR_FILENO, ft_strjoin_sep(": history: ", offset_str, ": history position out of range\n", 0), FREE_PRINT);
				return (1);
			}

			return (0);
		}

	#pragma endregion

	#pragma region "(-w) Write"

		static int write_history(t_parse_result *result) {
			const char *filename = (result->argc > 0) ? result->argv[0] : NULL;

			if (filename && !access(filename, F_OK) && access(filename, W_OK)) {
				print(STDERR_FILENO, result->shell_name, RESET);
				print(STDERR_FILENO, ft_strjoin_sep(": history: ", filename, ": file is not writable\n", 0), FREE_PRINT);
				return (1);
			} else {
				history_write(filename, 0);
			}

			return (0);
		}

	#pragma endregion

	#pragma region "(-r) Read"

		static int read_history(t_parse_result *result) {
			const char *filename = (result->argc > 0) ? result->argv[0] : NULL;

			if (filename && access(filename, F_OK)) {
				print(STDERR_FILENO, result->shell_name, RESET);
				print(STDERR_FILENO, ft_strjoin_sep(": history: ", filename, ": file does not exist\n", 0), FREE_PRINT);
				return (1);
			} else if (filename && access(filename, R_OK)) {
				print(STDERR_FILENO, result->shell_name, RESET);
				print(STDERR_FILENO, ft_strjoin_sep(": history: ", filename, ": file is not readable\n", 0), FREE_PRINT);
				return (1);
			} else {
				history_read(filename);
			}

			return (0);
		}

	#pragma endregion

	#pragma region "(-a) Append"

		static int write_append(t_parse_result *result) {
			const char *filename = (result->argc > 0) ? result->argv[0] : NULL;

			if (filename && !access(filename, F_OK) && access(filename, W_OK)) {
				print(STDERR_FILENO, result->shell_name, RESET);
				print(STDERR_FILENO, ft_strjoin_sep(": history: ", filename, ": file is not writable\n", 0), FREE_PRINT);
				return (1);
			} else {
				history_write(filename, 1);
			}

			return (0);
		}

	#pragma endregion

	#pragma region "(-n) Read New"

		static int read_history_new(t_parse_result *result) {
			const char *filename = (result->argc > 0) ? result->argv[0] : NULL;

			if (filename && access(filename, F_OK)) {
				print(STDERR_FILENO, result->shell_name, RESET);
				print(STDERR_FILENO, ft_strjoin_sep(": history: ", filename, ": file does not exist\n", 0), FREE_PRINT);
				return (1);
			} else if (filename && access(filename, R_OK)) {
				print(STDERR_FILENO, result->shell_name, RESET);
				print(STDERR_FILENO, ft_strjoin_sep(": history: ", filename, ": file is not readable\n", 0), FREE_PRINT);
				return (1);
			} else {
				history_read_append(filename);
			}

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "History"

	int bt_history(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",    NO_ARGUMENT, 0},
			{"version", NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, "cd:pranws", NULL, long_opts, "history [-c] [-d offset] [n] or history -anrw [filename] or history -ps arg [arg...]", 0);

		if (!result)		return (1);
		if (result->error)	return (free_options(result), 1);

		if (find_long_option(result, "help"))		return (free_options(result), help());
		if (find_long_option(result, "version"))	return (free_options(result), version());

		int local_options = 0;
		local_options += has_option(result, 'a');
		local_options += has_option(result, 'n');
		local_options += has_option(result, 'r');
		local_options += has_option(result, 'w');

		if (local_options > 1) {
			print(STDERR_FILENO, result->shell_name, RESET);
			print(STDERR_FILENO, ": history: cannot use more than one of -anrw\n", PRINT);
			return (free_options(result), 1);
		}

		int ret = 0;

		if (!result->options)			{ ret = print_history(result);		return (free_options(result), ret); }
		if (has_option(result, 'c'))	{ history_clear();					return (free_options(result), ret); }
		if (has_option(result, 's'))	{ ret = append_args(result);		return (free_options(result), ret); }
		if (has_option(result, 'p'))	{ ret = expansion_history(result);	return (free_options(result), ret); }
		if (has_option(result, 'd'))	{ ret = delete_history(result);		return (free_options(result), ret); }
		if (has_option(result, 'a'))	{ ret = write_append(result);		return (free_options(result), ret); }
		if (has_option(result, 'w'))	{ ret = write_history(result);		return (free_options(result), ret); }
		if (has_option(result, 'r'))	{ ret = read_history(result);		return (free_options(result), ret); }
		if (has_option(result, 'n'))	{ ret = read_history_new(result);	return (free_options(result), ret); }

		return (free_options(result), ret);
	}

#pragma endregion
