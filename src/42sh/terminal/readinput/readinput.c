/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readinput.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 09:44:40 by vzurera-          #+#    #+#             */
/*   Updated: 2025/02/22 20:22:10 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Auto-Complete

#pragma region "Includes"

	#include "libft.h"
	#include "terminal/terminal.h"
	#include "terminal/readinput/termcaps.h"
	#include "terminal/readinput/readinput.h"
	#include "terminal/readinput/prompt.h"
	#include "terminal/readinput/history.h"
	#include "main/options.h"

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

#pragma region "ReadInput"

	char *readinput(char *prompt) {
		int result = 0;
		buffer.size = 1024;
		buffer.position = 0, buffer.length = 0;
		buffer.value = ft_calloc(buffer.size, sizeof(char));
		buffer.CTRL = false; buffer.ALT = false; buffer.SHIFT = false;
		vi_mode = INSERT;

		//undo_push(f);

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
		disable_raw_mode();
		
		if (fake_segfault) { fake_segfault = false;
			write(2, "Segmentation fault (core dumped)\n", 34);
			return (readinput(prompt));
		}
		
		if ((options.emacs || options.vi) && options.history) {
			// Expand history commands
		}
		
		return (expand_history(buffer.value));
	}

#pragma endregion
