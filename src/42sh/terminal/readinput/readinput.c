/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readinput.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 09:44:40 by vzurera-          #+#    #+#             */
/*   Updated: 2025/02/21 22:01:19 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Auto-Complete

// History Expantion

#pragma region "Includes"

	#include "libft.h"
	#include "terminal/terminal.h"
	#include "terminal/readinput/termcaps.h"
	#include "terminal/readinput/readinput.h"
	#include "terminal/readinput/prompt.h"
	#include "terminal/readinput/history.h"
	#include "terminal/print.h"
	#include "main/options.h"
	#include "main/project.h"

#pragma endregion

#pragma region "Variables"

	t_buffer	buffer;

	bool		show_control_chars	= true;			//	Displays ^C or hides it
	bool		fake_segfault		= false;		//	Simulates a segmentation fault in the current command (does not execute or save it to history)
	int			vi_mode				= INSERT;		//	Current 'vi' mode
	char		*tmp_line			= NULL;			//	Store input while navigating through history
	bool		searching			= false;		//	Indicates whether the terminal is in searching mode

	static bool	raw_mode			= false;		//	Indicates whether the terminal is in raw mode

#pragma endregion

#pragma region "Raw Mode"

	void disable_raw_mode() {
		if (raw_mode) {
			raw_mode = false;
			cursor_show();
			terminal_release();
			tcsetattr(STDIN_FILENO, TCSAFLUSH, &terminal.term);
			prompt_clear(BOTH);
		}
	}

	static void enable_raw_mode() {
		raw_mode = true;
		tcgetattr(STDIN_FILENO, &terminal.term);
		terminal_initialize();

		struct termios raw = terminal.term;
		raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);			//	Disable echo (ECHO), canonical mode (ICANON), signals (ISIG), and extended input processing (IEXTEN)
		raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);	//	Disable break interrupt (BRKINT), carriage return to newline conversion (ICRNL), parity check (INPCK), stripping of eighth bit (ISTRIP), and software flow control (IXON)
		raw.c_oflag &= ~(OPOST);									//	Disable post-processing of output (OPOST)
		raw.c_cc[VMIN] = 1;											//	Read at least 1 character before returning
		raw.c_cc[VTIME] = 0;										//	No timeout for read

		tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
	}

#pragma endregion

	static char *expand_history_search(char *value, bool at_start) {
		for (size_t i = history_length() - 1; i >= 0; --i) {
			HIST_ENTRY *hist = history_get(i);
			if (hist && hist->line) {
				if (at_start && !ft_strncmp(hist->line, value, ft_strlen(value)))	return (hist->line);
				if (!at_start && ft_strstr(hist->line, value))						return (hist->line);
			}
			if (i == 0) break;
		}

		return (NULL);
	}

	static void expand_history() {
		if (!options.history || history_length() == 0) return;

		size_t i = 0;
		bool in_quotes = false;
		bool in_dquotes = false;

		while (i < buffer.length) {
			if (buffer.value[i] == '\\' && !in_quotes && !in_dquotes) {
				i += 2; continue;
			}
			if (buffer.value[i] == '\'' && !in_dquotes) {
				in_quotes = !in_quotes;
			}
			if (buffer.value[i] == '\"' && !in_quotes) {
				in_dquotes = !in_dquotes;
			}

			if (buffer.value[i] == '!' && !ft_isspace(buffer.value[i + 1]) && !in_quotes && !in_dquotes) {
				int start = i;
				size_t end = i + 1;
				char *replacement = NULL;

				if (buffer.value[end] == '!') { // Caso !!
					end++;
					HIST_ENTRY *hist = history_get(history_length() - 1);
					if (!hist) {
						print(STDERR_FILENO, ft_strjoin(PROYECTNAME, ": event not found\n", 0), FREE_RESET_PRINT);
						buffer.value[0] = '\0'; return;
					} else replacement = hist->line;
				} else if (ft_isdigit(buffer.value[end])) { // Caso !n
					while (ft_isdigit(buffer.value[end]) && end < buffer.length) end++;
					char *number = ft_substr(buffer.value, start + 1, end - (start + 1));
					size_t event = ft_atoi(number);
					HIST_ENTRY *hist = history_event(event);
					if (!hist || ft_strlen(number) > 9) {
						print(STDERR_FILENO, ft_strjoin_sep(PROYECTNAME, ": !", number, 3), FREE_RESET);
						print(STDERR_FILENO, ": event not found\n", PRINT);
						buffer.value[0] = '\0'; return;
					} else replacement = hist->line;
					sfree(number);
				} else if (buffer.value[end] == '-' && ft_isdigit(buffer.value[end + 1])) { // Caso !-n
					end++;
					while (ft_isdigit(buffer.value[end]) && end < buffer.length) end++;
					char *number = ft_substr(buffer.value, start + 2, end - (start + 2));
					size_t pos = ft_atoi(number);
					HIST_ENTRY *hist = history_get(history_length() - pos);
					if (!hist || ft_strlen(number) > 9) {
						print(STDERR_FILENO, ft_strjoin_sep(PROYECTNAME, ": !", number, 3), FREE_RESET);
						print(STDERR_FILENO, ": event not found\n", PRINT);
						buffer.value[0] = '\0'; return;
					} else replacement = hist->line;
					sfree(number);
				} else if (buffer.value[end] == '?' && buffer.value[end + 1] != '?' && !ft_isspace(buffer.value[end + 1])) { // Caso !?substr?
					end++;
					while (buffer.value[end] != '?' && !ft_isspace(buffer.value[end]) && end < buffer.length) end++;
					char *value = ft_substr(buffer.value, start + 2, end - (start + 2));
					replacement = expand_history_search(value, false);
					if (!replacement) {
						print(STDERR_FILENO, ft_strjoin_sep(PROYECTNAME, ": !?", value, 3), FREE_RESET);
						if (buffer.value[end] == '?') print(STDERR_FILENO, "?", JOIN);
						print(STDERR_FILENO, ": event not found\n", PRINT);
						buffer.value[0] = '\0'; return;
					}
					if (buffer.value[end] == '?') end++;
					sfree(value);
				} else { // Caso !prefijo
					while (!ft_isspace(buffer.value[end]) && end < buffer.length) end++;
					char *value = ft_substr(buffer.value, start + 1, end - (start + 1));
					replacement = expand_history_search(value, true);
					if (!replacement) {
						print(STDERR_FILENO, ft_strjoin_sep(PROYECTNAME, ": !", value, 3), FREE_RESET);
						print(STDERR_FILENO, ": event not found\n", PRINT);
						buffer.value[0] = '\0'; return;
					}
					sfree(value);
				}

				if (replacement) {
					buffer.value = replace(buffer.value, &start, end - start, replacement);
					i = start;
					buffer.length = ft_strlen(buffer.value);
				}
			}
			i++;
		}
	}

#pragma region "ReadInput"

	char *readinput(char *prompt) {
		int result = 0;
		buffer.size = 1024;
		buffer.position = 0, buffer.length = 0;
		buffer.value = ft_calloc(buffer.size, sizeof(char));
		buffer.CTRL = false; buffer.ALT = false; buffer.SHIFT = false;
		vi_mode = INSERT;

		undo_push();

		prompt_set(PS1, prompt);
		enable_raw_mode();
		if (prompt_PS1) write(STDOUT_FILENO, prompt_PS1, ft_strlen(prompt_PS1));

		cursor_get();
		while (!result) {
			cursor_show();
			int readed = read(STDIN_FILENO, &buffer.c, 1);
			cursor_hide();

			if (searching)			search_history();
			if (searching)			continue;

			if		(options.emacs)	result = readline(readed);
			else if	(options.vi)	result = vi(readed);
			else					result = dumb(readed);
		}

		undo_clear();
		expand_history();
		disable_raw_mode();

		if (fake_segfault) { fake_segfault = false;
			write(2, "Segmentation fault (core dumped)\n", 34);
			return (readinput(prompt));
		}

		if ((options.emacs || options.vi) && options.history) {
			// Expand history commands
		}

		return (buffer.value);
	}

#pragma endregion
