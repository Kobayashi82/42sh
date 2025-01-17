/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readinput.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 09:44:40 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/17 11:54:44 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Variables

	t_buffer	buffer;

	bool		show_control_chars	= true;
	bool		fake_segfault		= false;
	int			vi_mode				= INSERT;
	char		*tmp_line			= NULL;

	static bool	raw_mode			= false;

#pragma endregion

#pragma region Raw Mode

	void disable_raw_mode() {
		if (raw_mode) {
			raw_mode = false;
			cursor_show();
			tgetent(NULL, "none");
			tcsetattr(STDIN_FILENO, TCSAFLUSH, &terminal.term);
		}
	}

	static void enable_raw_mode() {
		raw_mode = true;
		tcgetattr(STDIN_FILENO, &terminal.term);
		terminal_initialize();

		struct termios raw = terminal.term;
		raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);			// Disable echo (ECHO), canonical mode (ICANON), signals (ISIG), and extended input processing (IEXTEN)
		raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);	// Disable break interrupt (BRKINT), carriage return to newline conversion (ICRNL), parity check (INPCK), stripping of eighth bit (ISTRIP), and software flow control (IXON)
		raw.c_oflag &= ~(OPOST);									// Disable post-processing of output (OPOST)
		raw.c_cc[VMIN] = 1;											// Read at least 1 character before returning
		raw.c_cc[VTIME] = 0;										// No timeout for read

		tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
	}

#pragma endregion

#pragma region ReadInput

	char *readinput(char *prompt) {
		int result = 0;
		buffer.size = 1024;
		buffer.position = 0, buffer.length = 0;
		buffer.value = ft_calloc(buffer.size, sizeof(char));
		buffer.CTRL = false; buffer.ALT = false; buffer.SHIFT = false;
		vi_mode = INSERT;

		set_prompt(PS1, prompt);
		enable_raw_mode();
		if (prompt_PS1) write(STDOUT_FILENO, prompt_PS1, ft_strlen(prompt_PS1));

		cursor_get();
		while (!result) {
			cursor_show();
			int readed = read(STDIN_FILENO, &buffer.c, 1);
			cursor_hide();

			if 		(options.emacs)	result = readline(readed);
			else if	(options.vi)	result = vi(readed);
			else					result = dumb(readed);
		}

		disable_raw_mode();
		return (buffer.value);
	}

#pragma endregion
