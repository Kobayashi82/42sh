/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 10:32:07 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/04 23:06:50 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Variables

	t_buffer	buffer;

	bool		show_control_chars = true;
	bool		fake_segfault = false;

#pragma endregion

#pragma region Raw Mode

	static void hide_cursor() { write(STDOUT_FILENO, "\033[?25l", 6); }
	static void show_cursor() { write(STDOUT_FILENO, "\033[?25h", 6); }

	static void disable_raw_mode() {
		show_cursor();
		tgetent(NULL, "none");
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &data.terminal.term);
	}

	static void enable_raw_mode() {
		tcgetattr(STDIN_FILENO, &data.terminal.term);
		atexit(disable_raw_mode);
		init_terminal_data();

		struct termios raw = data.terminal.term;
		raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);			// Disable echo (ECHO), canonical mode (ICANON), signals (ISIG), and extended input processing (IEXTEN)
		raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);	// Disable break interrupt (BRKINT), carriage return to newline conversion (ICRNL), parity check (INPCK), stripping of eighth bit (ISTRIP), and software flow control (IXON)
		raw.c_oflag &= ~(OPOST);									// Disable post-processing of output (OPOST)
		raw.c_cc[VMIN] = 1;											// Read at least 1 character before returning
		raw.c_cc[VTIME] = 0;										// No timeout for read

		tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
	}

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

			static void swap_char() {
				if (buffer.position > 0) {
					char temp[8];
					if (buffer.position < buffer.length) {
						size_t back_pos1 = 1, back_pos2 = 1;
						while (buffer.position - back_pos1 > 0 && (buffer.value[buffer.position - back_pos1] & 0xC0) == 0x80) back_pos1++;
						while (buffer.position + back_pos2 < buffer.length && (buffer.value[buffer.position + back_pos2] & 0xC0) == 0x80) back_pos2++;

						if (back_pos1 > 8 || back_pos2 > 8) return;

						ft_memcpy(temp, &buffer.value[buffer.position - back_pos1], back_pos1);
						ft_memmove(&buffer.value[buffer.position - back_pos1], &buffer.value[buffer.position], back_pos2);
						buffer.position -= back_pos1; buffer.position += back_pos2;
						ft_memmove(&buffer.value[buffer.position], temp, back_pos1);

						cursor_left(char_width(0, temp));
						write(STDOUT_FILENO, &buffer.value[buffer.position - back_pos2], back_pos1 + back_pos2);
						buffer.position += back_pos1;
					} else {
						size_t back_pos1 = 1, back_pos2 = 1;
						while (buffer.position - back_pos1 > 0 && (buffer.value[buffer.position - back_pos1] & 0xC0) == 0x80) back_pos1++;
						if (back_pos1 > 8) return;
						buffer.position -= back_pos1;
						while (buffer.position - back_pos2 > 0 && (buffer.value[buffer.position - back_pos2] & 0xC0) == 0x80) back_pos2++;

						ft_memcpy(temp, &buffer.value[buffer.position - back_pos2], back_pos2);
						ft_memmove(&buffer.value[buffer.position - back_pos2], &buffer.value[buffer.position], back_pos1);
						buffer.position -= back_pos2; buffer.position += back_pos1;
						ft_memmove(&buffer.value[buffer.position], temp, back_pos2);

						cursor_left(char_width(0, temp));
						cursor_left(char_width(buffer.position - back_pos1, buffer.value));

						write(STDOUT_FILENO, &buffer.value[buffer.position - back_pos1], back_pos1 + back_pos2);
						buffer.position += back_pos2;
					}
				}
			}

		#pragma endregion

		#pragma region Word

			// Not perfect, and multibyte characters are not handled consistently
			static t_word get_word(size_t position, size_t len, bool prev_word) {
				t_word	word;

				if (prev_word) {
					if (position == len && position > 0) do { (position)--; } while (position > 0 && (buffer.value[position] & 0xC0) == 0x80);
					while (position > 0 && (ft_isspace(buffer.value[position]) || ft_ispunct(buffer.value[position]))) position--;
					if (position == 0) { word.len = 0; return (word); }
					while (position > 0 && !ft_isspace(buffer.value[position]) && !ft_ispunct(buffer.value[position])) position--;
					word.start = position + (ft_isspace(buffer.value[position]) || ft_ispunct(buffer.value[position]));
				} else {
					while (position < len && (ft_isspace(buffer.value[position]) || ft_ispunct(buffer.value[position]))) position++;
					if (position == len) { do { (position)--; } while (position > 0 && (buffer.value[position] & 0xC0) == 0x80);
						while (position > 0 && (ft_isspace(buffer.value[position]) || ft_ispunct(buffer.value[position]))) position--;
						while (position > 0 && !ft_isspace(buffer.value[position]) && !ft_ispunct(buffer.value[position])) position--;
					} else
						while (position > 0 && !ft_isspace(buffer.value[position]) && !ft_ispunct(buffer.value[position])) position--;

					word.start = position + (ft_isspace(buffer.value[position]) || ft_ispunct(buffer.value[position]));
				}

				size_t	temp_pos = word.start;
				while (temp_pos < len && !ft_isspace(buffer.value[temp_pos]) && !ft_ispunct(buffer.value[temp_pos]))
					do { temp_pos++; } while (temp_pos < len && (buffer.value[temp_pos] & 0xC0) == 0x80);
				word.end = temp_pos;

				word.len = word.end - word.start;

				if (word.len > 0 && word.len < sizeof(word.value))
					for (size_t i = 0, pos = word.start; i < word.len; i++, pos++)
						word.value[i] = buffer.value[pos];

				return (word);
			}

			static void swap_word() {
				t_word prev, next, sep;
				size_t temp_pos = buffer.position;

				if (buffer.position == buffer.length && buffer.position == 0) return;

				next = get_word(buffer.position, buffer.length, false);
				if (next.len > sizeof(next.value)) return;
				if (!next.len) next = get_word(buffer.position, buffer.length, true);
				if (!next.len || next.start == 0) return;
				temp_pos = next.start - 1;
				prev = get_word(temp_pos, buffer.length, true);
				if (!prev.len || prev.len > sizeof(prev.value)) return;

				//printf (" - %ld : %ld\n", prev.len, next.len); return ;
				sep.start = prev.end;
				sep.end = next.start;
				if (sep.start < sep.end) {
					sep.len = sep.end - sep.start;
					if (sep.len > 0 && sep.len < sizeof(sep.value))
						for (size_t i = 0, pos = sep.start; i < sep.len; i++, pos++) sep.value[i] = buffer.value[pos];
				}

				ft_memmove(&buffer.value[prev.start], next.value, next.len);
				ft_memmove(&buffer.value[prev.start + next.len], sep.value, sep.len);
				ft_memmove(&buffer.value[prev.start + next.len + sep.len], prev.value, prev.len);

				// printf("%ld\n", buffer.position);
				while (buffer.position > 0 && buffer.position > prev.start) {
					do { (buffer.position)--; } while (buffer.position > 0 && (buffer.value[buffer.position] & 0xC0) == 0x80);
					cursor_left(char_width(buffer.position, buffer.value));
				}

				write(STDOUT_FILENO, &buffer.value[buffer.position], prev.len + sep.len + next.len);
				buffer.position = prev.start + next.len + sep.len + prev.len;
			}

		#pragma endregion

	#pragma endregion

#pragma endregion

#pragma region Input

	#pragma region C_EOF

		static int check_EOF(const int n) {
			if (n <= 0 || (buffer.c == 4 && !buffer.length)) {
				free(buffer.value); buffer.value = NULL;
				write(STDOUT_FILENO, "\r\n", 2);
				disable_raw_mode();
				return (1);
			}
			return (0);
		}

	#pragma endregion

	#pragma region C_SIGINT

		static int check_SIGINT(const char *prompt) {
			if (buffer.c == 3) {	// Ctrl+C
				buffer.position = 0; buffer.length = 0;
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

		static int check_nl() {
			if (buffer.c == '\r' || buffer.c == '\n') {
				buffer.value[buffer.length] = '\0';
				write(STDOUT_FILENO, "\r\n", 2);
				return (1);
			}
			return (0);
		}

	#pragma endregion

	#pragma region BackSpace

		#pragma region Char

			static void backspace() {
				if (buffer.position > 0) {
					size_t back_pos = 1;
					while (buffer.position - back_pos > 0 && (buffer.value[buffer.position - back_pos] & 0xC0) == 0x80) back_pos++;
					if (buffer.position < buffer.length) ft_memmove(&buffer.value[buffer.position - back_pos], &buffer.value[buffer.position], buffer.length - buffer.position);
					buffer.position -= back_pos; buffer.length -= back_pos;

					cursor_left(0);
					write(STDOUT_FILENO, &buffer.value[buffer.position], buffer.length - buffer.position);
					write(STDOUT_FILENO, "  ", 2); cursor_left(2);

					cursor_to_pos(buffer.length, buffer.position);
				}
			}

		#pragma endregion

		#pragma region Word

			static void backspace_word() {
				size_t pos = buffer.position, back_pos;

				if (buffer.position > 0) {
					while (pos > 0) { back_pos = 1;
						while (pos - back_pos > 0 && (buffer.value[pos - back_pos] & 0xC0) == 0x80) back_pos++;
						pos -= back_pos;
						if (!ft_isspace(buffer.value[pos]) && !ft_ispunct(buffer.value[pos])) break;
					} while (buffer.position > pos) backspace();

					while (pos > 0) { back_pos = 1;
						while (pos - back_pos > 0 && (buffer.value[pos - back_pos] & 0xC0) == 0x80) back_pos++;
						pos -= back_pos;
						if (ft_isspace(buffer.value[pos]) || ft_ispunct(buffer.value[pos])) { pos += back_pos; break; }
					} while (buffer.position > pos) backspace();
				}
			}

		#pragma endregion

		#pragma region Start

			static void backspace_start() {
				size_t pos = buffer.position, back_pos;

				if (buffer.position > 0) {
					while (pos > 0) { back_pos = 1;
						while (pos - back_pos > 0 && (buffer.value[pos - back_pos] & 0xC0) == 0x80) back_pos++;
						pos -= back_pos;
					} while (buffer.position > pos) backspace();
				}
			}

		#pragma endregion

	#pragma endregion

	#pragma region Delete

		#pragma region Char

			void delete_char() {
				if (buffer.position < buffer.length) {
					size_t back_pos = 1;
					while (buffer.position + back_pos < buffer.length && (buffer.value[buffer.position + back_pos] & 0xC0) == 0x80) back_pos++;

					ft_memmove(&buffer.value[buffer.position], &buffer.value[buffer.position + back_pos], buffer.length - (buffer.position + back_pos));
					buffer.length -= back_pos;

					write(STDOUT_FILENO, &buffer.value[buffer.position], buffer.length - buffer.position);
					write(STDOUT_FILENO, "  ", 2); cursor_left(2);
					cursor_to_pos(buffer.length, buffer.position);
				}
			}

		#pragma endregion

		#pragma region Word

			static void delete_word() {
				if (buffer.position < buffer.length) {
					while (buffer.position < buffer.length && (ft_isspace(buffer.value[buffer.position]) || ft_ispunct(buffer.value[buffer.position])))
						delete_char();
					while (buffer.position < buffer.length && !ft_isspace(buffer.value[buffer.position]) && !ft_ispunct(buffer.value[buffer.position]))
						delete_char();
				}
			}

		#pragma endregion

		#pragma region End

			static void delete_end() {
				if (buffer.position < buffer.length)
					while (buffer.position < buffer.length) delete_char();
			}

		#pragma endregion

	#pragma endregion

	#pragma region Cursors

		#pragma region Modifiers

			static char modifiers(char *seq) {
				int		modifier = 0;
				char	key = seq[1];
				size_t	i = 0;

				buffer.CTRL = false; buffer.ALT = false; buffer.SHIFT = false;
				if (seq[1] == '3') return (seq[1]);
				if (seq[0] == '[') { while (seq[i] && seq[i] != ';') i++;
					if (seq[i] == ';') { i++;
						while (seq[i] >= '0' && seq[i] <= '9')
							modifier = modifier * 10 + (seq[i++] - '0');
						if (seq[i]) key = seq[i];
					}
				}
				if (modifier && key) {
					if (modifier == 2)		buffer.SHIFT = true;
					if (modifier == 3)		buffer.ALT   = true;
					if (modifier == 5)		buffer.CTRL  = true;
					if (modifier == 6) {	buffer.SHIFT = true; buffer.CTRL = true; }
					if (modifier == 7) {	buffer.ALT   = true; buffer.CTRL = true; }
					if (modifier == 8) {	buffer.SHIFT = true; buffer.ALT  = true; buffer.CTRL = true; }
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

			static void arrow_left() {
				if (!buffer.ALT && !buffer.SHIFT && buffer.position > 0) {
					if (buffer.CTRL) {
						while (buffer.position > 0 && (ft_isspace(buffer.value[buffer.position - 1]) || ft_ispunct(buffer.value[buffer.position - 1]))) {
							(buffer.position)--; cursor_left(0);
						}
						while (buffer.position > 0 && !ft_isspace(buffer.value[buffer.position - 1]) && !ft_ispunct(buffer.value[buffer.position - 1])) {
							do { (buffer.position)--; } while (buffer.position > 0 && (buffer.value[buffer.position] & 0xC0) == 0x80);
							cursor_left(0);
						}
					} else {
						do { (buffer.position)--; } while (buffer.position > 0 && (buffer.value[buffer.position] & 0xC0) == 0x80);
						cursor_left(0);
					}
				}
			}

		#pragma endregion

		#pragma region Arrow Right

			static void arrow_right() {
				if (!buffer.ALT && !buffer.SHIFT && buffer.position < buffer.length) {
					if (buffer.CTRL) {
						while (buffer.position < buffer.length && (ft_isspace(buffer.value[buffer.position]) || ft_ispunct(buffer.value[buffer.position]))) {
							cursor_right(0); (buffer.position)++;
						}
						while (buffer.position < buffer.length && !ft_isspace(buffer.value[buffer.position]) && !ft_ispunct(buffer.value[buffer.position])) {
							cursor_right(0);
							do { (buffer.position)++; } while (buffer.position < buffer.length && (buffer.value[buffer.position] & 0xC0) == 0x80);
						}
					} else {
						if (buffer.position < buffer.length) {
							cursor_right(0);
							do { (buffer.position)++; } while (buffer.position < buffer.length && (buffer.value[buffer.position] & 0xC0) == 0x80);
						}
					}
				}
			}

		#pragma endregion

		#pragma region Arrow Home

			static void arrow_home() {
				while (buffer.position > 0) {
					do { (buffer.position)--; } while (buffer.position > 0 && (buffer.value[buffer.position] & 0xC0) == 0x80);
					cursor_left(0);
				}
			}

		#pragma endregion

		#pragma region Arrow End

			static void arrow_end() {
				while (buffer.position < buffer.length) {
					cursor_right(0);
					do { (buffer.position)++; } while (buffer.position < buffer.length && (buffer.value[buffer.position] & 0xC0) == 0x80);
				}
			}

		#pragma endregion

		#pragma region Cursor

			static int cursor() {
				char seq[8];
				ft_memset(&seq, 0, sizeof(seq));
				if (buffer.c == '\033') {
					if (read(STDIN_FILENO, seq, sizeof(seq) - 1) > 0) {
						if (seq[0] == 't') { swap_word(); return (1); }				// ALT + T			- Swap word
						if (seq[0] == '-') { redo(); return (1); }					// ALT + -			- Redo last action
						if (seq[0] == '[') { seq[1] = modifiers(seq);
							if (seq[1] == 'A') 						arrow_up();		// Up				- History next
							if (seq[1] == 'B') 						arrow_down();	// Down				- History prev
							if (seq[1] == 'D') 						arrow_left();	// Left				- Cursor left
							if (seq[1] == 'C') 						arrow_right();	// Right			- Cursor right
							if (seq[1] == 'H') 						arrow_home();	// Home				- Cursor to the start of the line
							if (seq[1] == 'F')						arrow_end();	// End				- Cursor to the end of the line
							if (seq[1] == '3' && seq[2] == '~')		delete_char();	// Delete			- Delete
							if (!ft_strncmp(seq + 1, "3;5~", 4))	delete_word();	// CTRL + Delete	- Delete current word
						}
					} return (1);
				} return (0);
			}

		#pragma endregion

	#pragma endregion

	#pragma region Specials

		static int specials() {
			if		(buffer.c == 19)	fake_segfault = true;	// CTRL + S - Fake SegFault
			else if (buffer.c == 31)	undo();					// CTRL + _ - Undo last action								(todo)
			else if (buffer.c == 1)		arrow_home();			// CTRL + A - Cursor to the start of the line
			else if (buffer.c == 5)		arrow_end();			// CTRL + E - Cursor to the end of the line
			else if (buffer.c == 2)		arrow_left();			// CTRL + B - Cursor right
			else if (buffer.c == 6)		arrow_right();			// CTRL + F - Cursor left
			else if (buffer.c == 16)	history_prev();			// CTRL + P - History prev									(todo)
			else if (buffer.c == 14)	history_next();			// CTRL + N - History next									(todo)
			else if (buffer.c == 20)	swap_char();			// CTRL + T - Swap char										(Not working right with multibytes 漢字)
			else if (buffer.c == 4)		delete_char();			// CTRL + D - Delete
			else if (buffer.c == 11)	delete_end();			// CTRL + K - Delete from cursor to end of line
			else if (buffer.c == 8)		backspace();			// CTRL + H - Backspace
			else if (buffer.c == 21)	backspace_start();		// CTRL + U - Backspace from cursor to start of line
			else if (buffer.c == 23)	backspace_word();		// CTRL + W	- Backspace current word 
			else if (buffer.c == 127)	backspace();			// BackSpace
			else if (buffer.c >= 1 && buffer.c <= 26) ;			// Ignore rest of the specials
			else return (0);
			return (1);
		}

	#pragma endregion

	#pragma region Print Char

		static void print_char() {
			size_t char_size = 1;
			if		(buffer.c >= 0xF0)	char_size = 4;
			else if (buffer.c >= 0xE0)	char_size = 3;
			else if (buffer.c >= 0xC0)	char_size = 2;

			// Expand buffer if necessary
			if (buffer.position + char_size >= buffer.size) {
				buffer.value = safe_realloc(buffer.value, buffer.size, buffer.size * 2);
				buffer.size *= 2;
				if (!buffer.value) {
					disable_raw_mode();
					exit_error(NO_MEMORY, 1, NULL, true);
				}
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
		}

	#pragma endregion

#pragma endregion

#pragma region ReadLine

	char *readline(const char* prompt) {
		buffer.size = 1024;
		buffer.position = 0, buffer.length = 0;
		buffer.value = safe_malloc(buffer.size);
		buffer.CTRL = false; buffer.ALT = false; buffer.SHIFT = false;

		enable_raw_mode();
		if (prompt) write(STDOUT_FILENO, prompt, ft_strlen(prompt));
	
		while (1) {
			show_cursor();
			int readed = read(STDIN_FILENO, &buffer.c, 1);
			hide_cursor();
			if (readed && buffer.c == 0x0C) {								// CTRL + L
				write(STDOUT_FILENO, "\033[H\033[2J", 7);
				if (prompt) write(STDOUT_FILENO, prompt, ft_strlen(prompt));
				write(STDOUT_FILENO, buffer.value, buffer.length);
				buffer.position = buffer.length;			continue;
			}

			if		(check_EOF(readed))						return (NULL);	// CTRL + D
			else if	(check_SIGINT(prompt))					continue;		// CTRL + C
			else if	(check_nl())							break;
			else if (specials())							continue;
			else if (cursor())								continue;
			else if (buffer.position < buffer.size - 1)		print_char();
		}

		disable_raw_mode();
		return (buffer.value);
	}

#pragma endregion
