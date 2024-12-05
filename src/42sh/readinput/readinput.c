/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readinput.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 09:44:40 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/05 13:29:58 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Variables

	t_buffer	buffer;

	bool		show_control_chars = true;
	bool		fake_segfault = false;
	bool		free_prompt = false;
	int			input_mode = READLINE;
	int			vi_mode = INSERT;

#pragma endregion

#pragma region Raw Mode

	void disable_raw_mode() {
		cursor_show();
		tgetent(NULL, "none");
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &data.terminal.term);
		if (free_prompt) { free_prompt = false; free(buffer.prompt); buffer.prompt = NULL; }
	}

	static void enable_raw_mode() {
		tcgetattr(STDIN_FILENO, &data.terminal.term);
		atexit(disable_raw_mode);
		terminal_initialize();

		struct termios raw = data.terminal.term;
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
		int result = 0; free_prompt = false;
		buffer.prompt = prompt ? prompt : get_prompt();
		buffer.size = 1024;
		buffer.position = 0, buffer.length = 0;
		buffer.value = safe_malloc(buffer.size);
		buffer.CTRL = false; buffer.ALT = false; buffer.SHIFT = false;
		vi_mode = INSERT;

		enable_raw_mode(); terminal_size(); terminal_start();
		if (buffer.prompt) write(STDOUT_FILENO, buffer.prompt, ft_strlen(buffer.prompt));

		while (!result) {
			cursor_show();
			int readed = read(STDIN_FILENO, &buffer.c, 1);
			cursor_hide();

			if (input_mode == READLINE)	result = readline(readed);
			if (input_mode == VI)		result = vi(readed);
		}

		disable_raw_mode();
		return (buffer.value);
	}

#pragma endregion
