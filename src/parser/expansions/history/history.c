/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 20:58:15 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/21 14:02:00 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "utils/libft.h"
	#include "terminal/readinput/history.h"
	#include "terminal/input.h"
	#include "terminal/print.h"
	#include "parser/syntax/syntax.h"
	#include "main/options.h"
	#include "main/project.h"

#pragma endregion

#pragma region "Position"

	static char *expand_position(char *value, int start, size_t *end, size_t length, bool is_double) {
		char *number = NULL;
		size_t pos = 0;
		(*end)++;

		if (is_double) {
			number = strdup("1");
			pos = 1;
		} else {
			while (isdigit(value[*end]) && *end < length) (*end)++;
			number = ft_substr(value, start + 2, *end - (start + 2));
			pos = atoi(number);
		}

		HIST_ENTRY *hist = history_get(history_length() - pos);

		if (!hist || ft_strlen(number) > 9) {

			if (is_double)
				print(STDERR_FILENO, ft_strjoin(PROYECTNAME, " !!: event not found\n", 0), FREE_RESET_PRINT);
			else {
				print(STDERR_FILENO, ft_strjoin_sep(PROYECTNAME, ": !-", number, 0), FREE_RESET);
				print(STDERR_FILENO, ": event not found\n", PRINT);
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
		size_t event = atoi(number);

		HIST_ENTRY *hist = history_event(event);

		if (!hist || ft_strlen(number) > 9) {

			print(STDERR_FILENO, ft_strjoin_sep(PROYECTNAME, ": !", number, 0), FREE_RESET);
			print(STDERR_FILENO, ": event not found\n", PRINT);

			free(number);
			value[0] = '\0'; return (NULL);
		}
		
		free(number);
		return (hist->line);
	}

#pragma endregion

#pragma region "Value"

	static char *expand_value(char *value, int start, size_t *end, size_t length, bool at_start) {
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
			HIST_ENTRY *hist = history_get(i);
			if (hist && hist->line) {
				if (at_start && !strncmp(hist->line, sub_value, ft_strlen(sub_value)))	{ result = hist->line; break; }
				if (!at_start && strstr(hist->line, sub_value))							{ result = hist->line; break; }
			}
			if (i == 0) break;
		}

		if (!result) {
			if (at_start) {
				print(STDERR_FILENO, ft_strjoin_sep(PROYECTNAME, ": !", sub_value, 0), FREE_RESET);
				print(STDERR_FILENO, ": event not found\n", PRINT);
			} else {
				print(STDERR_FILENO, ft_strjoin_sep(PROYECTNAME, ": !?", sub_value, 0), FREE_RESET);
				if (value[*end] == '?') print(STDERR_FILENO, "?", JOIN);
				print(STDERR_FILENO, ": event not found\n", PRINT);
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

	void expand_history(char **input, t_context *context, bool show_expansion) {
		if (!options.history || !options.histexpand || !input || !*input || !context) return;

		char *value = *input;
		size_t i = 0;
		size_t length = ft_strlen(value);
		bool changes = false, in_escape = false, in_quotes = false, in_dquotes = false;
		in_quotes = is_context(context->stack, CTX_QUOTE);
		in_dquotes = is_context(context->stack, CTX_DQUOTE);
		in_escape = context->in_escape;

		while (i < length) {
			// Handle Escape
			if (in_escape)							{ in_escape = false;		i++; continue; }
			if (value[i] == '\\' && !in_quotes)		{ in_escape = true;			i++; continue; }
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
					replacement = expand_position(value, start, &end, length, true);
					if (!replacement) return;
				} else if (value[end] == '-' && isdigit(value[end + 1])) {													// !-[n]
					replacement = expand_position(value, start, &end, length, false);
					if (!replacement) return;
				} else if (isdigit(value[end])) {																			// ![n]
					replacement = expand_event(value, start, &end, length);
					if (!replacement) return;
				} else if (value[end] == '?' && value[end + 1] != '?' && end + 1 < length && !isspace(value[end + 1]) && value[i + 1] != '"') {		// !?[str], !?[str]?
					replacement = expand_value(value, start, &end, length, false);
					if (!replacement) return;
				} else {																										// ![str]
					replacement = expand_value(value, start, &end, length, true);
					if (!replacement) return;
				}

				if (replacement) {
					char* new_input = replace_substring(value, start, end - start, replacement);
					if (new_input) {
						free(value);
						value = new_input;
						i += ft_strlen(replacement);
						length = ft_strlen(value);
						changes = true;
						continue;
					}
				}
			}
			i++;
		}

		if (changes) {
			char *new_value = strdup(value);
			if (show_expansion) {
				print(STDOUT_FILENO, new_value, RESET);
				print(STDOUT_FILENO, "\n", PRINT);
			}
			free(value);
			*input = new_value;
		}
	}

#pragma endregion

#pragma region "Info"

	// !!			Substitute for the last command. Same as !-1.
	// !-[n]		Substitute for command position [n] from the end of history.
	// ![n]			Substitute for command event [n] from history.
	// ![str]		Substitute for the last command beginning with [str].
	// !?[str]?		Substitute for the last command containing [str]. Last '?' is optional.

#pragma endregion
