/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dumb.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:34:33 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/10 14:09:41 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Input

	#pragma region EOF

		static int ctrl_d(const int n) {
			if (n <= 0 || (buffer.c == 4 && !buffer.length)) {
				safe_free(buffer.value); buffer.value = NULL;
				write(STDOUT_FILENO, "\r\n", 2);
				return (1);
			}
			return (0);
		}

	#pragma endregion

	#pragma region SIGINT

		static int ctrl_c() {
			if (buffer.c == 3) {	// Ctrl+C
				buffer.position = 0; buffer.length = 0;
				if (show_control_chars)	write(STDOUT_FILENO, "^C\r\n", 4);
				else					write(STDOUT_FILENO, "\r\n", 2);
				if (prompt_PS1) write(STDOUT_FILENO, prompt_PS1, ft_strlen(prompt_PS1));
				terminal.signal = 2;
				return (1);
			}
			return (0);
		}

	#pragma endregion

	#pragma region NewLine

		static int enter() {
			if (buffer.c == '\r' || buffer.c == '\n') {
				buffer.value[buffer.length] = '\0';
				write(STDOUT_FILENO, "\r\n", 2);
				return (1);
			}
			return (0);
		}

	#pragma endregion

	#pragma region BackSpace

		static void backspace() {
			if (buffer.position > 0) {
				size_t back_pos = 1;
				while (buffer.position - back_pos > 0 && (buffer.value[buffer.position - back_pos] & 0xC0) == 0x80) back_pos++;
				if (buffer.position < buffer.length) ft_memmove(&buffer.value[buffer.position - back_pos], &buffer.value[buffer.position], buffer.length - buffer.position);
				buffer.position -= back_pos; buffer.length -= back_pos;

				cursor_left(0);
				write(STDOUT_FILENO, &buffer.value[buffer.position], buffer.length - buffer.position);
				write(STDOUT_FILENO, "  ", 2); cursor_left(2);

				cursor_move(buffer.length, buffer.position);
			}
		}

	#pragma endregion

	#pragma region Specials

		static int specials() {
			if (buffer.c == 127) backspace();
			else return (0);

			return (1);
		}

	#pragma endregion

	#pragma region Print Char

		static int print_char() {
			size_t char_size = 1;
			if		(buffer.c >= 0xF0)	char_size = 4;
			else if (buffer.c >= 0xE0)	char_size = 3;
			else if (buffer.c >= 0xC0)	char_size = 2;

			if (buffer.position >= buffer.size - 1) return (0);

			// Expand buffer if necessary
			if (buffer.position + char_size >= buffer.size) {
				buffer.value = safe_realloc(buffer.value, buffer.size, buffer.size * 2);
				buffer.size *= 2;
				if (!buffer.value) { disable_raw_mode(); exit_error(NO_MEMORY, 1, NULL, true); }
			}

			if (buffer.position < buffer.length) ft_memmove(&buffer.value[buffer.position + char_size], &buffer.value[buffer.position], buffer.length - buffer.position);

			// Insert all bytes of the character into the buffer
			buffer.value[(buffer.position)++] = buffer.c;
			for (size_t i = 1; i < char_size; i++) read(STDIN_FILENO, &buffer.value[(buffer.position)++], 1);
			buffer.length += char_size;

			write(STDOUT_FILENO, &buffer.value[buffer.position - char_size], buffer.length - (buffer.position - char_size));

			// Adjust the cursor position in the terminal
			size_t move_back = 0;
			for (size_t i = buffer.position; i < buffer.length; ) {
				if (char_width(i, buffer.value) == 2) move_back++;
				if ((unsigned char)buffer.value[i] >= 0xC0) {
					if ((unsigned char)buffer.value[i] >= 0xF0)		i += 4;	// 4 bytes
					else if ((unsigned char)buffer.value[i] >= 0xE0)	i += 3;	// 3 bytes
					else										i += 2;	// 2 bytes
				} else											i++;	// 1 byte
				move_back++;
			}
			while (move_back--) write(STDOUT_FILENO, "\033[D", 3);
			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region Dumb

	int dumb(int readed) {
		if		(ctrl_d(readed))	return (1);
		else if	(ctrl_c())			return (0);
		else if	(enter())			return (1);
		else if (specials())		return (0);
		else if (print_char())		return (0);
		return (0);
	}

#pragma endregion
