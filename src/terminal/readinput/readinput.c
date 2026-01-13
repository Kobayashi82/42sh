/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readinput.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 09:44:40 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/13 13:39:23 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//	Auto-Complete
//	Prompt
//	Command with char position (los comandos de vi que dependen de una posicion... duh)
//	Revistar check_tty

//	Current issues:
//
//	Incomplete support for multi-width characters (missing logic to handle them at column edges).		SOLUTION: Multi-width characters are ignored.
//	When the input has more lines than the terminal, the cursor movement logic breaks.					SOLUTION: Do not write long lines.
//	Historial de varias lineas no se pega a la columna 0 (no se a que me referia exactamente).

#pragma region "Includes"

	#include "terminal/terminal.h"
	#include "terminal/readinput/history.h"
	#include "terminal/readinput/prompt.h"
	#include "terminal/readinput/readinput.h"
	#include "terminal/readinput/termcaps.h"
	#include "main/shell.h"
	#include "utils/utils.h"

#pragma endregion

#pragma region "Variables"

	t_buffer	buffer;
	char		*term_prompt;
	int			raw_mode;
	int			vi_mode;

#pragma endregion

#pragma region "Raw Mode"

	#pragma region "Enable"

		static void enable_raw_mode() {
			raw_mode = 1;
			tcgetattr(STDIN_FILENO, &terminal.term);
			terminal_initialize();
			terminal_update_limits();

			struct termios raw = terminal.term;
			raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);			//	Disable echo (ECHO), canonical mode (ICANON), signals (ISIG), and extended input processing (IEXTEN)
			raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);	//	Disable break interrupt (BRKINT), carriage return to newline conversion (ICRNL), parity check (INPCK), stripping of eighth bit (ISTRIP), and software flow control (IXON)
			raw.c_oflag &= ~(OPOST);									//	Disable post-processing of output (OPOST)
			raw.c_cc[VMIN] = 1;											//	Read at least 1 character before returning
			raw.c_cc[VTIME] = 0;										//	No timeout for read

			tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
		}

	#pragma endregion

	#pragma region "Disable"

		void disable_raw_mode() {
			if (raw_mode) {
				raw_mode = 0;
				cursor_show();
				tcsetattr(STDIN_FILENO, TCSAFLUSH, &terminal.term);
				free(term_prompt); term_prompt = NULL;
			}
		}

	#pragma endregion

#pragma endregion

#pragma region "ReadInput"
	void home() {
		if (!buffer.length || buffer.position > buffer.length) return;

		while (buffer.position > 0) {
			do { (buffer.position)--; } while (buffer.position > 0 && (buffer.value[buffer.position] & 0xC0) == 0x80);
			cursor_left(0);
		}
	}

	char *readinput(char *prompt) {
		if (!isatty(STDIN_FILENO)) return (NULL);
		int result = 0;
		buffer.size = 1024;
		buffer.position = buffer.length = vi_mode = 0;
		buffer.value = calloc(buffer.size, sizeof(char));
		buffer.CTRL = buffer.ALT = buffer.SHIFT = 0;

		enable_raw_mode();

		free(term_prompt);
		term_prompt = ft_strdup(prompt);
		if (term_prompt) write(STDOUT_FILENO, term_prompt, strlen(term_prompt));

		cursor_get();
		while (!result) {
			cursor_show();
			int readed = read(STDIN_FILENO, &buffer.c, 1);

			if (readed < 0) {
				if (errno == EINTR) {
					if (shell.exit) {
						disable_raw_mode();
						write(STDOUT_FILENO, "\n", 1);
						if (buffer.value) free(buffer.value);
						buffer.value = NULL;
						return (NULL);
					}
					continue;
				}
				return (NULL);
			}

			if (!isatty(STDIN_FILENO)) return (NULL);
			cursor_hide();		

			if (hist_searching && history_search()) continue;

			if		(shell.options.emacs)	result = readline(readed);
			else if	(shell.options.vi)	result = vi();
			else					result = dumb(readed);

			if (result == 2) {
				result = 0;
				if (term_prompt) write(STDOUT_FILENO, term_prompt, strlen(term_prompt));
				continue;
			}

			if (result && buffer.value && buffer.value[0]) {
				int ret;
				char *buffer_cpy = ft_strdup(buffer.value);
				if (shell.options.histexpand) {
					if (shell.options.histverify) {
						if ((ret = expand_history(&buffer.value, 0)) == 1) {
							if (term_prompt) write(STDOUT_FILENO, term_prompt, strlen(term_prompt));
							buffer.position = buffer.size = buffer.length = ft_strlen(buffer.value);
							write(STDOUT_FILENO, buffer.value, buffer.length);
							cursor_get();
							result = 0;
							free(buffer_cpy);
							continue;
						}
					} else {
						ret = expand_history(&buffer.value, 1);
					}

					if (ret == 2) {
						if (shell.options.histreedit) {
							cursor_start_column();
							if (term_prompt) write(STDOUT_FILENO, term_prompt, strlen(term_prompt));
							free(buffer.value);
							buffer.value = buffer_cpy;
							buffer.position = buffer.size = buffer.length = ft_strlen(buffer.value);
							write(STDOUT_FILENO, buffer.value, buffer.length);
							cursor_get();
							result = 0;
							continue;;
						} else {
							cursor_start_column();
							if (term_prompt) write(STDOUT_FILENO, term_prompt, strlen(term_prompt));
							free(buffer.value);
							free(buffer_cpy);
							buffer.size = 1024;
							buffer.position = buffer.length = 0;
							buffer.value = calloc(buffer.size, sizeof(char));
							cursor_get();
							result = 0;
							continue;;
						}
					}
					free(buffer_cpy);
				}
			}

		}

		history_set_pos_last();
		undo_clear();

		disable_raw_mode();

		return (buffer.value);
	}

#pragma endregion
