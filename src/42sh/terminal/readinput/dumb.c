/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dumb.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:34:33 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/17 17:12:38 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Input

	#pragma region EOF

		static int ctrl_d(const int n) {
			if (n <= 0 || (buffer.c == 4 && !buffer.length)) {
				sfree(buffer.value); buffer.value = NULL;
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

	#pragma region Cursor

		static int cursor() {
			char seq[8];
			if (buffer.c == 27) {

				fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
				read(STDIN_FILENO, seq, sizeof(seq) - 1);
				fcntl(STDIN_FILENO, F_SETFL, O_SYNC);

				return (1);
			}
			return (0);
		}

	#pragma endregion

	#pragma region Specials

		static int specials() {
			if (buffer.c >= 1 && buffer.c <= 26) ;
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
				buffer.value = ft_realloc(buffer.value, buffer.size, buffer.size * 2);
				buffer.size *= 2;
			}

			// Insert all bytes of the character into the buffer
			buffer.value[(buffer.position)++] = buffer.c;
			for (size_t i = 1; i < char_size; i++) read(STDIN_FILENO, &buffer.value[(buffer.position)++], 1);
			buffer.length += char_size;

			write(STDOUT_FILENO, &buffer.value[buffer.position - char_size], buffer.length - (buffer.position - char_size));

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
		else if (cursor())			return (0);
		else if (print_char())		return (0);
		return (0);
	}

#pragma endregion
