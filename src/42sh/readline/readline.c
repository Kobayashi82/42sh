/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 10:32:07 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/02 23:01:38 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

// Caracteres de emoticonos
// ALT + T
// CTRL + _ (CTRL + SHIFT + _)
// Historial

#pragma region Variables

	bool show_control_chars = true;
	bool SHIFT = false;
	bool ALT = false;
	bool CTRL = false;

#pragma endregion

#pragma region Raw Mode

	static void disable_raw_mode() { tcsetattr(STDIN_FILENO, TCSAFLUSH, &data.terminal.term); }

	static void enable_raw_mode() {
		tcgetattr(STDIN_FILENO, &data.terminal.term);
		atexit(disable_raw_mode);

		struct termios raw = data.terminal.term;
		raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);			// Disable echo (ECHO), canonical mode (ICANON), signals (ISIG), and extended input processing (IEXTEN)
		raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);	// Disable break interrupt (BRKINT), carriage return to newline conversion (ICRNL), parity check (INPCK), stripping of eighth bit (ISTRIP), and software flow control (IXON)
		raw.c_oflag &= ~(OPOST);									// Disable post-processing of output (OPOST)
		raw.c_cc[VMIN] = 1;											// Read at least 1 character before returning
		raw.c_cc[VTIME] = 0;										// No timeout for read

		tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
	}

	static void hide_cursor() { write(STDOUT_FILENO, "\033[?25l", 6); }
	static void show_cursor() { write(STDOUT_FILENO, "\033[?25h", 6); }

#pragma endregion

#pragma region Functions

	#pragma region Swap Char

		static void swap_char(size_t *position, size_t *len, char *buffer) {
			if (*position > 0) {
				char temp[8];
				if (*position < *len) {
					size_t back_pos1 = 1, back_pos2 = 1;
					while (*position - back_pos1 > 0 && (buffer[*position - back_pos1] & 0xC0) == 0x80) back_pos1++;
					while (*position + back_pos2 < *len && (buffer[*position + back_pos2] & 0xC0) == 0x80) back_pos2++;

					if (back_pos1 > 8 || back_pos2 > 8) return;

					ft_memcpy(temp, &buffer[*position - back_pos1], back_pos1);
					ft_memmove(&buffer[*position - back_pos1], &buffer[*position], back_pos2);
					*position -= back_pos1; *position += back_pos2;
					ft_memmove(&buffer[*position], temp, back_pos1);

					write(STDOUT_FILENO, "\033[D", 3);
					write(STDOUT_FILENO, &buffer[*position - back_pos2], back_pos1 + back_pos2);
					*position += back_pos1;
				} else {
					size_t back_pos1 = 1, back_pos2 = 1;
					while (*position - back_pos1 > 0 && (buffer[*position - back_pos1] & 0xC0) == 0x80) back_pos1++;
					if (back_pos1 > 8) return;
					*position -= back_pos1;
					while (*position - back_pos2 > 0 && (buffer[*position - back_pos2] & 0xC0) == 0x80) back_pos2++;

					ft_memcpy(temp, &buffer[*position - back_pos2], back_pos2);
					ft_memmove(&buffer[*position - back_pos2], &buffer[*position], back_pos1);
					*position -= back_pos2; *position += back_pos1;
					ft_memmove(&buffer[*position], temp, back_pos2);

					write(STDOUT_FILENO, "\033[D\033[D", 6);
					write(STDOUT_FILENO, &buffer[*position - back_pos1], back_pos1 + back_pos2);
					*position += back_pos2;
				}
			}
		}

	#pragma endregion

	#pragma region Swap Word

		static void swap_word(size_t *position, size_t *len, char *buffer) {
			(void) position;
			(void) len;
			(void) buffer;
			write(1, "swap word", 10);
		}

	#pragma endregion

	#pragma region Undo

		static void undo() {
			write(1, "undo", 4);
		}

	#pragma endregion

	#pragma region Redo

		static void redo() {
			write(1, "redo", 4);
		}

	#pragma endregion

#pragma endregion

#pragma region Input

	#pragma region C_EOF

		static int check_EOF(const int n, const char c, const size_t position, char *buffer) {
			int result = 0;

			if (n <= 0) result = 2;
			else if (c == 4) result = (position == 0) + 1;

			if (result == 2) {
				free(buffer);
				write(STDOUT_FILENO, "\r\n", 2);
				show_cursor();
				disable_raw_mode();
			}
			return (result);
		}

	#pragma endregion

	#pragma region C_SIGINT

		static int check_SIGINT(const char c, size_t *position, size_t *len, const char *prompt) {
			if (c == 3) {	// Ctrl+C
				*position = 0; *len = 0;
				if (show_control_chars)	write(STDOUT_FILENO, "^C\r\n", 4);
				else					write(STDOUT_FILENO, "\r\n", 2);
				if (prompt) write(STDOUT_FILENO, prompt, ft_strlen(prompt));
				data.terminal.signal = 2;
				return (1);
			}
			return (0);
		}

	#pragma endregion

	#pragma region NewLine

		static int check_nl(const char c, const size_t len, char *buffer) {
			if (c == '\r' || c == '\n') {
				buffer[len] = '\0';
				write(STDOUT_FILENO, "\r\n", 2);
				return (1);
			}
			return (0);
		}

	#pragma endregion

	#pragma region Delete

		static void delete(size_t *position, size_t *len, char *buffer) {
			if (*position < *len) {
				size_t back_pos = 1;
				while (*position + back_pos < *len && (buffer[*position + back_pos] & 0xC0) == 0x80) back_pos++;

				ft_memmove(&buffer[*position], &buffer[*position + back_pos], *len - (*position + back_pos));
				*len -= back_pos;

				write(STDOUT_FILENO, &buffer[*position], *len - *position);
				write(STDOUT_FILENO, " ", 1);

				size_t move_back = 1;
				for (size_t i = *position; i < *len; ) {
					if ((unsigned char)buffer[i] >= 0xC0) {
						if ((unsigned char)buffer[i] >= 0xF0)		i += 4;	// 4 bytes
						else if ((unsigned char)buffer[i] >= 0xE0)	i += 3;	// 3 bytes
						else										i += 2;	// 2 bytes
					} else											i++;	// 1 byte
					move_back++;
				}
				while (move_back--) write(STDOUT_FILENO, "\033[D", 3);
			}
		}

	#pragma endregion

	#pragma region Cursors

		#pragma region Modifiers

			static char modifiers(char *seq) {
				int modifier = 0;
				char key = seq[1];
				size_t i = 0;

				CTRL = false; ALT = false; SHIFT = false;
				if (seq[0] == '[') { while (seq[i] && seq[i] != ';') i++;
					if (seq[i] == ';') { i++;
						while (seq[i] >= '0' && seq[i] <= '9')
							modifier = modifier * 10 + (seq[i++] - '0');
						if (seq[i]) key = seq[i];
					}
				}
				if (modifier && key) {
					if (modifier == 2)		SHIFT = true;
					if (modifier == 3)		ALT = true;
					if (modifier == 5)		CTRL = true;
					if (modifier == 6) {	SHIFT = true; CTRL = true; }
					if (modifier == 7) {	ALT = true; CTRL = true; }
					if (modifier == 8) {	SHIFT = true; ALT = true; CTRL = true; }
				}
				return (key);
			}

		#pragma endregion

		#pragma region Arrow Up

			static void arrow_up() {
				write(1, "history prev", 13);
			}

		#pragma endregion

		#pragma region Arrow Down

			static void arrow_down() {
				write(1, "history next", 13);
			}

		#pragma endregion

		#pragma region Arrow Left

			static void arrow_left(size_t *position, char *buffer) {
				if (!ALT && !SHIFT && *position > 0) {
					if (CTRL) {
						while (*position > 0 && (ft_isspace(buffer[*position - 1]) || ft_ispunct(buffer[*position - 1]))) {
							(*position)--;
							write(STDOUT_FILENO, "\033[D", 3);
						}
						while (*position > 0 && !ft_isspace(buffer[*position - 1]) && !ft_ispunct(buffer[*position - 1])) {
							do { (*position)--; } while (*position > 0 && (buffer[*position] & 0xC0) == 0x80);
							write(STDOUT_FILENO, "\033[D", 3);
						}
					} else {
						do { (*position)--; } while (*position > 0 && (buffer[*position] & 0xC0) == 0x80);
						write(STDOUT_FILENO, "\033[D", 3);
					}
				}
			}

		#pragma endregion

		#pragma region Arrow Right

			static void arrow_right(size_t *position, size_t *len, char *buffer) {
				if (!ALT && !SHIFT && *position < *len) {
					if (CTRL) {
						while (*position < *len && (ft_isspace(buffer[*position]) || ft_ispunct(buffer[*position]))) {
							(*position)++;
							write(STDOUT_FILENO, "\033[C", 3);
						}
						while (*position < *len && !ft_isspace(buffer[*position]) && !ft_ispunct(buffer[*position])) {
							do { (*position)++; } while (*position < *len && (buffer[*position] & 0xC0) == 0x80);
							write(STDOUT_FILENO, "\033[C", 3);
						}
					} else {
						if (*position < *len) {
							do { (*position)++; } while (*position < *len && (buffer[*position] & 0xC0) == 0x80);
							write(STDOUT_FILENO, "\033[C", 3);
						}
					}
				}
			}

		#pragma endregion

		#pragma region Arrow Home

			static void arrow_home(size_t *position, char *buffer) {
				while (*position > 0) {
					do { (*position)--; } while (*position > 0 && (buffer[*position] & 0xC0) == 0x80);
					write(STDOUT_FILENO, "\033[D", 3);
				}
			}

		#pragma endregion

		#pragma region Arrow End

			static void arrow_end(size_t *position, size_t *len, char *buffer) {
				while (*position < *len) {
					do { (*position)++; } while (*position < *len && (buffer[*position] & 0xC0) == 0x80);
					write(STDOUT_FILENO, "\033[C", 3);
				}
			}

		#pragma endregion

		static int cursor(const char c, size_t *position, size_t *len, char *buffer) {
			char seq[8];
			ft_memset(&seq, 0, sizeof(seq));
			if (c == '\033') {
				if (read(STDIN_FILENO, seq, sizeof(seq) - 1) > 0) {
					if (seq[0] == 't') { swap_word(position, len, buffer); return (1); }	// ALT + T
					if (seq[0] == '-') { redo(); return (1); }		// ALT + -
					if (seq[0] == '[') {
						seq[1] = modifiers(seq);
						if (seq[1] == 'A') arrow_up();
						if (seq[1] == 'B') arrow_down();
						if (seq[1] == 'D') arrow_left(position, buffer);
						if (seq[1] == 'C') arrow_right(position, len, buffer);
						if (seq[1] == 'H') arrow_home(position, buffer);
						if (seq[1] == 'F') arrow_end(position, len, buffer);
						if (seq[1] == '3' && seq[2] == '~') delete(position, len, buffer);
					}
				} return (1);
			} return (0);
		}

	#pragma endregion

	#pragma region BackSpace

		static int backspace(const char c, size_t *position, size_t *len, char *buffer) {
			if (c == 127) {
				if (*position > 0) {
					size_t back_pos = 1;
					while (*position - back_pos > 0 && (buffer[*position - back_pos] & 0xC0) == 0x80) back_pos++;
					if (*position < *len) ft_memmove(&buffer[*position - back_pos], &buffer[*position], *len - *position);
					*position -= back_pos; *len -= back_pos;

					write(STDOUT_FILENO, "\033[D", 3);							// Move the cursor to the left
					write(STDOUT_FILENO, &buffer[*position], *len - *position);
					write(STDOUT_FILENO, " ", 1);

					size_t move_back = 1;
					for (size_t i = *position; i < *len; ) {					// Move the cursor to the correct position
						if ((unsigned char)buffer[i] >= 0xC0) {
							if ((unsigned char)buffer[i] >= 0xF0)		i += 4;	// 4 bytes
							else if ((unsigned char)buffer[i] >= 0xE0)	i += 3;	// 3 bytes
							else										i += 2;	// 2 bytes
						} else											i += 1;	// 1 byte
						move_back++;
					}
					while (move_back--) write(STDOUT_FILENO, "\033[D", 3);		// Move the cursor to the left
				} return (1);
			} else if (c == 2)	{ arrow_left(position, buffer);			return (1);
			} else if (c == 6)	{ arrow_right(position, len, buffer);	return (1);
			} else if (c == 16)	{ arrow_up();							return (1);
			} else if (c == 14)	{ arrow_down();							return (1);
			} else if (c == 20)	{ swap_char(position, len, buffer);		return (1);		// CTRL + T
			} else if (c == 31)	{ undo();								return (1); }	// CTRL + _
			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region ReadLine

	char *readline(const char* prompt) {
		size_t buffer_size = 1024;
		size_t position = 0, len = 0;
		char *buffer = safe_malloc(buffer_size);
		char c;
	
		enable_raw_mode();
		if (prompt) write(STDOUT_FILENO, prompt, ft_strlen(prompt));
	
		while (1) {
			show_cursor();
			int n = read(STDIN_FILENO, &c, 1);
			hide_cursor();
			int result = check_EOF(n, c, position, buffer);
			if (result == 1) continue;
			else if (result == 2) return (NULL);
	
			if (check_SIGINT(c, &position, &len, prompt)) continue;
			if (check_nl(c, len, buffer)) break; 
			else if (backspace(c, &position, &len, buffer)) ;
			else if (cursor(c, &position, &len, buffer)) ;
			else if (position < buffer_size - 1) {
				size_t char_size = 1;
				if ((unsigned char)c >= 0xC0) {
					char_size = 2;
					if ((unsigned char)c >= 0xE0) char_size = 3;
					if ((unsigned char)c >= 0xF0) char_size = 4;
				}

				// Expand buffer if necessary
				if (position + char_size >= buffer_size) {
					buffer = safe_realloc(buffer, buffer_size, buffer_size * 2);
					if (!buffer) { free(buffer); exit_error(NO_MEMORY, 1, NULL, true); }
					buffer_size *= 2;
				}

				if (position < len) ft_memmove(&buffer[position + char_size], &buffer[position], len - position);

				// Insert all bytes of the character into the buffer
				buffer[position++] = c;
				for (size_t i = 1; i < char_size; i++) read(STDIN_FILENO, &buffer[position++], 1);
				len += char_size;

				// Reprint from the current position to the end
				write(STDOUT_FILENO, &buffer[position - char_size], len - (position - char_size));

				// Adjust the cursor position in the terminal
				size_t move_back = 0;
				for (size_t i = position; i < len; ) {
					if ((unsigned char)buffer[i] >= 0xC0) {
						if ((unsigned char)buffer[i] >= 0xF0)		i += 4;	// 4 bytes
						else if ((unsigned char)buffer[i] >= 0xE0)	i += 3;	// 3 bytes
						else										i += 2;	// 2 bytes
					} else											i++;	// 1 byte
					move_back++;
				}
				while (move_back--) write(STDOUT_FILENO, "\033[D", 3);
			}
		}

		show_cursor();
		disable_raw_mode();
		return (buffer);
	}

#pragma endregion
