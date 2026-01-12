/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_expansion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 20:58:15 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/12 12:31:08 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "terminal/readinput/history.h"
	#include "main/shell.h"
	#include "utils/utils.h"

#pragma endregion

#pragma region "Position"

	static char *expand_position(char *value, int start, size_t *end, size_t length, int is_double) {
		char *number = NULL;
		size_t pos = 0;
		(*end)++;

		if (is_double) {
			number = ft_strdup("1");
			pos = 1;
		} else {
			while (isdigit(value[*end]) && *end < length) (*end)++;
			number = ft_substr(value, start + 2, *end - (start + 2));
			pos = atoi(number);
		}

		HIST_ENTRY *hist = history_entry_position(history_length() - pos);

		if (!hist || ft_strlen(number) > 9) {

			if (is_double)
				print(STDERR_FILENO, ft_strjoin(PROYECTNAME, " !!: event not found\n", 0), P_FREE_RESET_PRINT);
			else {
				print(STDERR_FILENO, ft_strjoin_sep(PROYECTNAME, ": !-", number, 0), P_FREE_RESET);
				print(STDERR_FILENO, ": event not found\n", P_PRINT);
			}
		
			free(number);
			value[0] = '\0'; return (NULL);
		}
		
		free(number);
		return (hist->line);
	}

#pragma endregion

#pragma region "Event"

	static char *expand_event(char *value, int start, size_t *end, size_t length) {
		while (isdigit(value[*end]) && *end < length) (*end)++;
		char *number = ft_substr(value, start + 1, *end - (start + 1));
		size_t offset = atoi(number);

		HIST_ENTRY *hist = history_entry_offset(offset);

		if (!hist || ft_strlen(number) > 9) {

			print(STDERR_FILENO, ft_strjoin_sep(PROYECTNAME, ": !", number, 0), P_FREE_RESET);
			print(STDERR_FILENO, ": event not found\n", P_PRINT);

			free(number);
			value[0] = '\0'; return (NULL);
		}
		
		free(number);
		return (hist->line);
	}

#pragma endregion

#pragma region "Value"

	static char *expand_value(char *value, int start, size_t *end, size_t length, int at_start) {
		char *sub_value = NULL;

		if (at_start) {
			while (!isspace(value[*end]) && value[*end] != '"' && *end < length) (*end)++;
			sub_value = ft_substr(value, start + 1, *end - (start + 1));
		} else {
			(*end)++;
			while (value[*end] != '?' && !isspace(value[*end]) && value[*end] != '"' && *end < length) (*end)++;
			sub_value = ft_substr(value, start + 2, *end - (start + 2));
		}

		char *result = NULL;
		for (size_t i = history_length() - 1; i >= 0; --i) {
			HIST_ENTRY *hist = history_entry_position(i);
			if (hist && hist->line) {
				if (at_start && !strncmp(hist->line, sub_value, ft_strlen(sub_value)))	{ result = hist->line; break; }
				if (!at_start && strstr(hist->line, sub_value))							{ result = hist->line; break; }
			}
			if (i == 0) break;
		}

		if (!result) {
			if (at_start) {
				print(STDERR_FILENO, ft_strjoin_sep(PROYECTNAME, ": !", sub_value, 0), P_FREE_RESET);
				print(STDERR_FILENO, ": event not found\n", P_PRINT);
			} else {
				print(STDERR_FILENO, ft_strjoin_sep(PROYECTNAME, ": !?", sub_value, 0), P_FREE_RESET);
				if (value[*end] == '?') print(STDERR_FILENO, "?", P_JOIN);
				print(STDERR_FILENO, ": event not found\n", P_PRINT);
			}

			free(sub_value);
			value[0] = '\0'; return (NULL);
		}

		if (value[*end] == '?') (*end)++;
		free(sub_value);
		return (result);
	}

#pragma endregion

#pragma region "Expand"

	int expand_history(char **input, int show_expansion) {
		if (!shell.options.history || !shell.options.histexpand || !input || !*input) return (0);

		char *value = *input;
		size_t i = 0;
		size_t length = ft_strlen(value);
		int changes = 0, in_escape = 0, in_quotes = 0, in_dquotes = 0;

		int ret = 0;

		while (i < length) {
			// Handle Escape
			if (in_escape)							{ in_escape = 0;			i++; continue; }
			if (value[i] == '\\' && !in_quotes)		{ in_escape = 1;			i++; continue; }
			// Handle Quotes
			if (value[i] == '\'' && !in_dquotes)	{ in_quotes  = !in_quotes;	i++; continue; }
			if (value[i] == '"'  && !in_quotes)		{ in_dquotes = !in_dquotes;	i++; continue; }
			// Handle Spaces
			if (isspace(value[i])) {											i++; continue; }

			if (!in_quotes && value[i] == '!' && i + 1 < length && !isspace(value[i + 1]) && value[i + 1] != '"') {

				size_t start = i;
				size_t end = i + 1;
				char *replacement = NULL;

				if (value[end] == '!') {																						// !!
					replacement = expand_position(value, start, &end, length, 1);
					if (!replacement) return (2);
					ret = 1;
				} else if (value[end] == '-' && isdigit(value[end + 1])) {														// !-[n]
					replacement = expand_position(value, start, &end, length, 0);
					if (!replacement) return (2);
					ret = 1;
				} else if (isdigit(value[end])) {																				// ![n]
					replacement = expand_event(value, start, &end, length);
					if (!replacement) return (2);
					ret = 1;
				} else if (value[end] == '?' && value[end + 1] != '?' && end + 1 < length && !isspace(value[end + 1]) && value[i + 1] != '"') {		// !?[str], !?[str]?
					replacement = expand_value(value, start, &end, length, 0);
					if (!replacement) return (2);
					ret = 1;
				} else {																										// ![str]
					replacement = expand_value(value, start, &end, length, 1);
					if (!replacement) return (2);
					ret = 1;
				}

				if (replacement) {
					char* new_input = replace_substring(value, start, end - start, replacement);
					if (new_input) {
						free(value);
						value = new_input;
						i += ft_strlen(replacement);
						length = ft_strlen(value);
						changes = 1;
						continue;
					}
				}
			}
			i++;
		}

		if (changes) {
			char *new_value = ft_strdup(value);
			if (show_expansion) {
				print(STDOUT_FILENO, new_value, P_RESET);
				print(STDOUT_FILENO, "\n", P_PRINT);
			}
			free(value);
			*input = new_value;
		}

		return (ret);
	}

#pragma endregion

#pragma region "Info"

	// !!			Substitute for the last command. Same as !-1.
	// !-[n]		Substitute for command position [n] from the end of history.
	// ![n]			Substitute for command event [n] from history.
	// ![str]		Substitute for the last command beginning with [str].
	// !?[str]?		Substitute for the last command containing [str]. Last '?' is optional.

#pragma endregion
