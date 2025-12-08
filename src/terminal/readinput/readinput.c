/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readinput.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 09:44:40 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/08 21:32:56 by vzurera-         ###   ########.fr       */
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
	#include "terminal/readinput/termcaps.h"
	#include "terminal/readinput/prompt.h"
	#include "terminal/readinput/readinput.h"
	#include "terminal/readinput/history.h"
	#include "main/options.h"
	#include "main/shell.h"
	#include "utils/libft.h"

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
			if (!isatty(STDIN_FILENO)) return (NULL);
			cursor_hide();		

			if (hist_searching && history_search()) continue;

			if		(options.emacs)	result = readline(readed);
			else if	(options.vi)	result = vi();
			else					result = dumb(readed);
		}
		
		history_set_pos_end();
		undo_clear();

		disable_raw_mode();

		return (buffer.value);
	}

#pragma endregion
