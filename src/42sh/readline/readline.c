/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 10:32:07 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/03 22:19:31 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Variables

	bool show_control_chars = true;
	bool fake_segfault = false;

	static bool SHIFT = false;
	static bool ALT = false;
	static bool CTRL = false;

	typedef struct s_word { size_t start, end, len; char value[1024]; } t_word;

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

#pragma endregion

#pragma region Utils

	#pragma region Chars

		#pragma region Char Length

			int char_length(unsigned char ch) {
				if (ch >= 0xF0) return (4);  // 4-byte
				if (ch >= 0xE0) return (3);  // 3-byte
				if (ch >= 0xC0) return (2);  // 2-byte
				if (ch < 0x80)  return (1);  // 1-byte
				return (0);                  // Invalid byte
			}

		#pragma endregion

		#pragma region Char Codepoint

			unsigned int char_codepoint(const char *buffer, size_t len) {
				unsigned char c = buffer[0];
				if (c < 0x80)					return (c);
				else if (c < 0xE0 && len >= 2)	return (((c & 0x1F) << 6)  | (buffer[1] & 0x3F));
				else if (c < 0xF0 && len >= 3)	return (((c & 0x0F) << 12) | ((buffer[1] & 0x3F) << 6)  | (buffer[2] & 0x3F));
				else if (c < 0xF8 && len >= 4)	return (((c & 0x07) << 18) | ((buffer[1] & 0x3F) << 12) | ((buffer[2] & 0x3F) << 6) | (buffer[3] & 0x3F));
				else							return (0);
			}

		#pragma endregion

		#pragma region Char Width

			int char_width(const size_t position, const char *buffer) {
				unsigned int codepoint = char_codepoint(&buffer[position], char_length(buffer[position]));
				if ((codepoint >= 0x1100  && codepoint <= 0x115F)	||			// Hangul Jamo
					(codepoint >= 0x2329  && codepoint <= 0x232A)	||			// Angle brackets
					(codepoint >= 0x2E80  && codepoint <= 0x9FFF)	||			// CJK, radicals, etc.
					(codepoint >= 0xAC00  && codepoint <= 0xD7A3)	||			// Hangul syllables
					(codepoint >= 0xF900  && codepoint <= 0xFAFF)	||			// CJK compatibility
					(codepoint >= 0xFE10  && codepoint <= 0xFE19)	||			// Vertical forms
					(codepoint >= 0x1F300 && codepoint <= 0x1F64F)	||			// Emojis
					(codepoint >= 0x1F900 && codepoint <= 0x1F9FF))	return (2);	// Supplemental Symbols
				return 1;
			}

		#pragma endregion

	#pragma endregion

	#pragma region Cursor

		static void hide_cursor() { write(STDOUT_FILENO, "\033[?25l", 6); }
		static void show_cursor() { write(STDOUT_FILENO, "\033[?25h", 6); }
	
	#pragma endregion

#pragma endregion

#pragma region Functions

	#pragma region History

		#pragma region Prev

			static void history_prev() {
				write(1, "History Prev", 13);
			}

		#pragma endregion

		#pragma region Next

			static void history_next() {
				write(1, "History Next", 13);
			}

		#pragma endregion

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

	#pragma region Swap

		#pragma region Char

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

						if (char_width(0, temp) == 2) write(STDOUT_FILENO, "\033[D", 3);
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

						if (char_width(0, temp) == 2) write(STDOUT_FILENO, "\033[D", 3);
						if (char_width(*position - back_pos1, buffer) == 2) write(STDOUT_FILENO, "\033[D", 3);
						write(STDOUT_FILENO, "\033[D\033[D", 6);
						write(STDOUT_FILENO, &buffer[*position - back_pos1], back_pos1 + back_pos2);
						*position += back_pos2;
					}
				}
			}

		#pragma endregion

		#pragma region Word

			// Not perfect, and multibyte characters are not handled consistently
			static t_word get_word(size_t position, size_t len, char *buffer, bool prev_word) {
				t_word	word;

				if (prev_word) {
					if (position == len && position > 0) do { (position)--; } while (position > 0 && (buffer[position] & 0xC0) == 0x80);
					while (position > 0 && (ft_isspace(buffer[position]) || ft_ispunct(buffer[position]))) position--;
					if (position == 0) { word.len = 0; return (word); }
					while (position > 0 && !ft_isspace(buffer[position]) && !ft_ispunct(buffer[position])) position--;
					word.start = position + (ft_isspace(buffer[position]) || ft_ispunct(buffer[position]));
				} else {
					while (position < len && (ft_isspace(buffer[position]) || ft_ispunct(buffer[position]))) position++;
					if (position == len) { do { (position)--; } while (position > 0 && (buffer[position] & 0xC0) == 0x80);
						while (position > 0 && (ft_isspace(buffer[position]) || ft_ispunct(buffer[position]))) position--;
						while (position > 0 && !ft_isspace(buffer[position]) && !ft_ispunct(buffer[position])) position--;
					} else
						while (position > 0 && !ft_isspace(buffer[position]) && !ft_ispunct(buffer[position])) position--;

					word.start = position + (ft_isspace(buffer[position]) || ft_ispunct(buffer[position]));
				}

				size_t	temp_pos = word.start;
				while (temp_pos < len && !ft_isspace(buffer[temp_pos]) && !ft_ispunct(buffer[temp_pos]))
					do { temp_pos++; } while (temp_pos < len && (buffer[temp_pos] & 0xC0) == 0x80);
				word.end = temp_pos;

				word.len = word.end - word.start;

				if (word.len > 0 && word.len < sizeof(word.value))
					for (size_t i = 0, pos = word.start; i < word.len; i++, pos++)
						word.value[i] = buffer[pos];

				return (word);
			}

			static void swap_word(size_t *position, size_t *len, char *buffer) {
				t_word prev, next, sep;
				size_t temp_pos = *position;

				if (*position == *len && *position == 0) return;

				next = get_word(*position, *len, buffer, false);
				if (next.len > sizeof(next.value)) return;
				if (!next.len) next = get_word(*position, *len, buffer, true);
				if (!next.len || next.start == 0) return;
				temp_pos = next.start - 1;
				prev = get_word(temp_pos, *len, buffer, true);
				if (!prev.len || prev.len > sizeof(prev.value)) return;

				//printf (" - %ld : %ld\n", prev.len, next.len); return ;
				sep.start = prev.end;
				sep.end = next.start;
				if (sep.start < sep.end) {
					sep.len = sep.end - sep.start;
					if (sep.len > 0 && sep.len < sizeof(sep.value))
						for (size_t i = 0, pos = sep.start; i < sep.len; i++, pos++) sep.value[i] = buffer[pos];
				}

				ft_memmove(&buffer[prev.start], next.value, next.len);
				ft_memmove(&buffer[prev.start + next.len], sep.value, sep.len);
				ft_memmove(&buffer[prev.start + next.len + sep.len], prev.value, prev.len);

				// printf("%ld\n", *position);
				while (*position > 0 && *position > prev.start) {
					do { (*position)--; } while (*position > 0 && (buffer[*position] & 0xC0) == 0x80);
					if (char_width(*position, buffer) == 2) write(STDOUT_FILENO, "\033[D", 3);
					write(STDOUT_FILENO, "\033[D", 3);
				}

				write(STDOUT_FILENO, &buffer[*position], prev.len + sep.len + next.len);
				*position = prev.start + next.len + sep.len + prev.len;
			}

		#pragma endregion

	#pragma endregion

#pragma endregion

#pragma region Input

	#pragma region C_EOF

		static int check_EOF(const int n, const unsigned char c, const size_t position, char *buffer) {
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

		static int check_SIGINT(const unsigned char c, size_t *position, size_t *len, const char *prompt) {
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

		static int check_nl(const unsigned char c, const size_t len, char *buffer) {
			if (c == '\r' || c == '\n') {
				buffer[len] = '\0';
				write(STDOUT_FILENO, "\r\n", 2);
				return (1);
			}
			return (0);
		}

	#pragma endregion

	#pragma region BackSpace

		#pragma region Char

			static void backspace(size_t *position, size_t *len, char *buffer) {
				if (*position > 0) {
					size_t back_pos = 1;
					while (*position - back_pos > 0 && (buffer[*position - back_pos] & 0xC0) == 0x80) back_pos++;
					if (char_width(*position - back_pos, buffer) == 2) write(STDOUT_FILENO, "\033[D", 3);
					if (*position < *len) ft_memmove(&buffer[*position - back_pos], &buffer[*position], *len - *position);
					*position -= back_pos; *len -= back_pos;

					write(STDOUT_FILENO, "\033[D", 3);
					write(STDOUT_FILENO, &buffer[*position], *len - *position);
					write(STDOUT_FILENO, "  ", 2);

					size_t move_back = 2;
					for (size_t i = *position; i < *len; ) {
						if (char_width(i, buffer) == 2) move_back++;
						if ((unsigned char)buffer[i] >= 0xC0) {
							if ((unsigned char)buffer[i] >= 0xF0)		i += 4;	// 4 bytes
							else if ((unsigned char)buffer[i] >= 0xE0)	i += 3;	// 3 bytes
							else										i += 2;	// 2 bytes
						} else											i += 1;	// 1 byte
						move_back++;
					}
					while (move_back--) write(STDOUT_FILENO, "\033[D", 3);
				}
			}

		#pragma endregion

		#pragma region Word

			static void backspace_word(size_t *position, size_t *len, char *buffer) {
				size_t pos = *position, back_pos;

				if (*position > 0) {
					while (pos > 0) { back_pos = 1;
						while (pos - back_pos > 0 && (buffer[pos - back_pos] & 0xC0) == 0x80) back_pos++;
						pos -= back_pos;
						if (!ft_isspace(buffer[pos]) && !ft_ispunct(buffer[pos])) break;
					} while (*position > pos) backspace(position, len, buffer);

					while (pos > 0) { back_pos = 1;
						while (pos - back_pos > 0 && (buffer[pos - back_pos] & 0xC0) == 0x80) back_pos++;
						pos -= back_pos;
						if (ft_isspace(buffer[pos]) || ft_ispunct(buffer[pos])) { pos += back_pos; break; }
					} while (*position > pos) backspace(position, len, buffer);
				}
			}

		#pragma endregion

		#pragma region Start

			static void backspace_start(size_t *position, size_t *len, char *buffer) {
				size_t pos = *position, back_pos;

				if (*position > 0) {
					while (pos > 0) { back_pos = 1;
						while (pos - back_pos > 0 && (buffer[pos - back_pos] & 0xC0) == 0x80) back_pos++;
						pos -= back_pos;
					} while (*position > pos) backspace(position, len, buffer);
				}
			}

		#pragma endregion

	#pragma endregion

	#pragma region Delete

		#pragma region Char

			static void delete_char(size_t *position, size_t *len, char *buffer) {
				if (*position < *len) {
					size_t back_pos = 1;
					while (*position + back_pos < *len && (buffer[*position + back_pos] & 0xC0) == 0x80) back_pos++;

					ft_memmove(&buffer[*position], &buffer[*position + back_pos], *len - (*position + back_pos));
					*len -= back_pos;

					write(STDOUT_FILENO, &buffer[*position], *len - *position);
					write(STDOUT_FILENO, "  ", 2);

					size_t move_back = 2;
					for (size_t i = *position; i < *len; ) {
						if (char_width(i, buffer) == 2) move_back++;
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

		#pragma region Word

			static void delete_word(size_t *position, size_t *len, char *buffer) {
				if (*position < *len) {
					while (*position < *len && (ft_isspace(buffer[*position]) || ft_ispunct(buffer[*position])))
						delete_char(position, len, buffer);
					while (*position < *len && !ft_isspace(buffer[*position]) && !ft_ispunct(buffer[*position]))
						delete_char(position, len, buffer);
				}
			}

		#pragma endregion

		#pragma region Word

			static void delete_end(size_t *position, size_t *len, char *buffer) {
				if (*position < *len)
					while (*position < *len) delete_char(position, len, buffer);
			}

		#pragma endregion

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
				history_prev();
			}

		#pragma endregion

		#pragma region Arrow Down

			static void arrow_down() {
				history_next();
			}

		#pragma endregion

		#pragma region Arrow Left

			static void arrow_left(size_t *position, char *buffer) {
				if (!ALT && !SHIFT && *position > 0) {
					if (CTRL) {
						while (*position > 0 && (ft_isspace(buffer[*position - 1]) || ft_ispunct(buffer[*position - 1]))) {
							(*position)--;
							if (char_width(*position, buffer) == 2) write(STDOUT_FILENO, "\033[D", 3);
							write(STDOUT_FILENO, "\033[D", 3);
						}
						while (*position > 0 && !ft_isspace(buffer[*position - 1]) && !ft_ispunct(buffer[*position - 1])) {
							do { (*position)--; } while (*position > 0 && (buffer[*position] & 0xC0) == 0x80);
							if (char_width(*position, buffer) == 2) write(STDOUT_FILENO, "\033[D", 3);
							write(STDOUT_FILENO, "\033[D", 3);
						}
					} else {
						do { (*position)--; } while (*position > 0 && (buffer[*position] & 0xC0) == 0x80);
						if (char_width(*position, buffer) == 2) write(STDOUT_FILENO, "\033[D", 3);
						write(STDOUT_FILENO, "\033[D", 3);
					}
				}
			}

		#pragma endregion

		#pragma region Arrow Right

			static void arrow_right(size_t *position, size_t *len, char *buffer) {
				if (!ALT && !SHIFT && *position < *len) {
					size_t prev_pos = *position;
					if (CTRL) {
						while (*position < *len && (ft_isspace(buffer[*position]) || ft_ispunct(buffer[*position]))) {
							(*position)++;
							write(STDOUT_FILENO, "\033[C", 3);
						}
						while (*position < *len && !ft_isspace(buffer[*position]) && !ft_ispunct(buffer[*position])) {
							prev_pos = *position;
							do { (*position)++; } while (*position < *len && (buffer[*position] & 0xC0) == 0x80);
							if (char_width(prev_pos, buffer) == 2) write(STDOUT_FILENO, "\033[C", 3);
							write(STDOUT_FILENO, "\033[C", 3);
						}
					} else {
						if (*position < *len) {
							prev_pos = *position;
							do { (*position)++; } while (*position < *len && (buffer[*position] & 0xC0) == 0x80);
							if (char_width(prev_pos, buffer) == 2) write(STDOUT_FILENO, "\033[C", 3);
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
					if (char_width(*position, buffer) == 2) write(STDOUT_FILENO, "\033[D", 3);
					write(STDOUT_FILENO, "\033[D", 3);
				}
			}

		#pragma endregion

		#pragma region Arrow End

			static void arrow_end(size_t *position, size_t *len, char *buffer) {
				size_t prev_pos;
				while (*position < *len) {
					prev_pos = *position;
					do { (*position)++; } while (*position < *len && (buffer[*position] & 0xC0) == 0x80);
					if (char_width(prev_pos, buffer) == 2) write(STDOUT_FILENO, "\033[C", 3);
					write(STDOUT_FILENO, "\033[C", 3);
				}
			}

		#pragma endregion

		#pragma region Cursor

			static int cursor(const unsigned char c, size_t *position, size_t *len, char *buffer) {
				char seq[8];
				ft_memset(&seq, 0, sizeof(seq));
				if (c == '\033') {
					if (read(STDIN_FILENO, seq, sizeof(seq) - 1) > 0) {
						if (seq[0] == 't') { swap_word(position, len, buffer); return (1); }	// ALT + T
						if (seq[0] == '-') { redo(); return (1); }		// ALT + -
						if (seq[0] == '[') {
							if (seq[1] == '3' && seq[2] == '~')										{ delete_char(position, len, buffer); return (1); }
							if (seq[1] == '3' && seq[2] == ';' && seq[3] == '5' && seq[4] == '~')	{ delete_word(position, len, buffer); return (1); }
							seq[1] = modifiers(seq);
							if (seq[1] == 'A') arrow_up();
							if (seq[1] == 'B') arrow_down();
							if (seq[1] == 'D') arrow_left(position, buffer);
							if (seq[1] == 'C') arrow_right(position, len, buffer);
							if (seq[1] == 'H') arrow_home(position, buffer);
							if (seq[1] == 'F') arrow_end(position, len, buffer);
						}
					} return (1);
				} return (0);
			}

		#pragma endregion

	#pragma endregion

	#pragma region Specials

		static int specials(const unsigned char c, size_t *position, size_t *len, char *buffer) {
			if		(c == 19)	fake_segfault = true;					// CTRL + S - Fake SegFault
			else if (c == 31)	undo();									// CTRL + _ - Undo last action								(todo)
			else if (c == 1)	arrow_home(position, buffer);			// CTRL + A - Cursor to the start of the line
			else if (c == 5)	arrow_end(position, len, buffer);		// CTRL + E - Cursor to the end of the line
			else if (c == 2)	arrow_left(position, buffer);			// CTRL + B - Cursor Right
			else if (c == 6)	arrow_right(position, len, buffer);		// CTRL + F - Cursor left
			else if (c == 16)	history_prev();							// CTRL + P - History prev									(todo)
			else if (c == 14)	history_next();							// CTRL + N - History next									(todo)
			else if (c == 20)	swap_char(position, len, buffer);		// CTRL + T - Swap char										(Not working right with multibytes 漢字)
			else if (c == 11)	delete_end(position, len, buffer);		// CTRL + K - Delete from cursor to end of line
			else if (c == 8)	backspace(position, len, buffer);		// CTRL + H - Backspace
			else if (c == 21)	backspace_start(position, len, buffer);	// CTRL + U - Backspace from cursor to start of line
			else if (c == 23)	backspace_word(position, len, buffer);	// CTRL + W	- Backspace current word 
			else if (c == 127)	backspace(position, len, buffer);		// BackSpace
			else if (c >= 1 && c <= 26) ;								// Ignore rest of the specials
			else return (0);
			return (1);
		}

	#pragma endregion

	#pragma region Print Char

		static void print_char(unsigned char c, size_t *position, size_t *len, char *buffer, size_t *buffer_size) {
			size_t char_size = 1;
			if (c >= 0xF0)		char_size = 4;
			else if (c >= 0xE0)	char_size = 3;
			else if (c >= 0xC0)	char_size = 2;

			// Expand buffer if necessary
			if (*position + char_size >= *buffer_size) {
				buffer = safe_realloc(buffer, *buffer_size, *buffer_size * 2);
				if (!buffer) { free(buffer); exit_error(NO_MEMORY, 1, NULL, true); }
				*buffer_size *= 2;
			}

			if (*position < *len) ft_memmove(&buffer[*position + char_size], &buffer[*position], *len - *position);

			// Insert all bytes of the character into the buffer
			buffer[(*position)++] = c;
			for (size_t i = 1; i < char_size; i++) read(STDIN_FILENO, &buffer[(*position)++], 1);
			*len += char_size;

			write(STDOUT_FILENO, &buffer[*position - char_size], *len - (*position - char_size));

			// Adjust the cursor position in the terminal
			size_t move_back = 0;
			for (size_t i = *position; i < *len; ) {
				if (char_width(i, buffer) == 2) move_back++;
				if ((unsigned char)buffer[i] >= 0xC0) {
					if ((unsigned char)buffer[i] >= 0xF0)		i += 4;	// 4 bytes
					else if ((unsigned char)buffer[i] >= 0xE0)	i += 3;	// 3 bytes
					else										i += 2;	// 2 bytes
				} else											i++;	// 1 byte
				move_back++;
			}
			while (move_back--) write(STDOUT_FILENO, "\033[D", 3);
		}

	#pragma endregion

#pragma endregion

#pragma region ReadLine

	char *readline(const char* prompt) {
		size_t buffer_size = 1024;
		size_t position = 0, len = 0;
		char *buffer = safe_malloc(buffer_size);
		unsigned char c;
	
		enable_raw_mode();
		if (prompt) write(STDOUT_FILENO, prompt, ft_strlen(prompt));
	
		while (1) {
			show_cursor();
			int n = read(STDIN_FILENO, &c, 1);
			hide_cursor();
			if (c == 0x0C) {												// CTRL + L
				write(STDOUT_FILENO, "\033[H\033[2J", 7);
				if (prompt) write(STDOUT_FILENO, prompt, ft_strlen(prompt));
				write(STDOUT_FILENO, &buffer[0], len);
				position = len;									continue;
			}

			int result = check_EOF(n, c, position, buffer);					// CTRL + D
			if		(result == 1)								continue;
			else if	(result == 2)								return (NULL);
	
			if		(check_SIGINT(c, &position, &len, prompt))	continue;	// CTRL + C
			if		(check_nl(c, len, buffer))					break;
			else if (specials(c, &position, &len, buffer))		continue;
			else if (cursor(c, &position, &len, buffer))		continue;
			else if (position < buffer_size - 1)				print_char(c, &position, &len, buffer, &buffer_size);
		}

		show_cursor();
		disable_raw_mode();
		return (buffer);
	}

#pragma endregion
