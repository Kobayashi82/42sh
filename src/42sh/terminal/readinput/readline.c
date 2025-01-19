/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 10:32:07 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/19 14:36:15 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "terminal.h"
#include "readinput.h"
#include "prompt.h"
#include "options.h"
#include "history.h"

//	Optimizar Delete y BackSpace

#pragma region Input

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
				if (tmp_line) { sfree(tmp_line); tmp_line = NULL; }
				history_set_pos_end();
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
				history_set_pos_end();
				buffer.value[buffer.length] = '\0';
				write(STDOUT_FILENO, "\r\n", 2);
				if (tmp_line) { sfree(tmp_line); tmp_line = NULL; }
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

					cursor_move(buffer.length, buffer.position);
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

			static void delete_char() {
				if (buffer.position < buffer.length) {
					size_t back_pos = 1;
					while (buffer.position + back_pos < buffer.length && (buffer.value[buffer.position + back_pos] & 0xC0) == 0x80) back_pos++;

					ft_memmove(&buffer.value[buffer.position], &buffer.value[buffer.position + back_pos], buffer.length - (buffer.position + back_pos));
					buffer.length -= back_pos;

					write(STDOUT_FILENO, &buffer.value[buffer.position], buffer.length - buffer.position);
					write(STDOUT_FILENO, "  ", 2); cursor_left(2);
					cursor_move(buffer.length, buffer.position);
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

		#pragma region Home

			static void home() {
				while (buffer.position > 0) {
					cursor_left(0);
					do { (buffer.position)--; } while (buffer.position > 0 && (buffer.value[buffer.position] & 0xC0) == 0x80);
				}
			}

		#pragma endregion

		#pragma region End

			static void end() {
				while (buffer.position < buffer.length) {
					cursor_right(0);
					do { (buffer.position)++; } while (buffer.position < buffer.length && (buffer.value[buffer.position] & 0xC0) == 0x80);
				}
			}

		#pragma endregion

		#pragma region Arrow Up

			static void arrow_up() {
				if (!options.hist_on || !history_length()) return;
				char *new_line = history_prev();

				if (!new_line) return;
				if (!tmp_line) tmp_line = ft_substr(buffer.value, 0, buffer.length);

				end(); backspace_start();
				while (ft_strlen(new_line) >= (int)buffer.size) {
					buffer.value = ft_realloc(buffer.value, buffer.size, buffer.size * 2);
					buffer.size *= 2;
				}
				ft_strcpy(buffer.value, new_line);
				buffer.length = ft_strlen(buffer.value);
				buffer.position = buffer.length;
				write(STDOUT_FILENO, buffer.value, buffer.length);
			}

		#pragma endregion

		#pragma region Arrow Down

			static void arrow_down() {
				if (!options.hist_on || !history_length()) return;

				char *new_line = history_next();
				bool free_line = false;

				if (!tmp_line && history_get_pos() == history_length() - 1) return;
				if (!new_line && history_get_pos() == history_length() - 1) {
					new_line = tmp_line;
					tmp_line = NULL;
					free_line = true;
				}

				end(); backspace_start();
				while (ft_strlen(new_line) >= (int)buffer.size) {
					buffer.value = ft_realloc(buffer.value, buffer.size, buffer.size * 2);
					buffer.size *= 2;
				}
				ft_strcpy(buffer.value, new_line);
				buffer.length = ft_strlen(buffer.value);
				buffer.position = buffer.length;
				write(STDOUT_FILENO, buffer.value, buffer.length);

				if (free_line && new_line) sfree(new_line);
			}

		#pragma endregion

		#pragma region Arrow Left

			static void arrow_left() {
				if (!buffer.ALT && !buffer.SHIFT && buffer.position > 0) {
					if (buffer.CTRL) {
						while (buffer.position > 0 && (ft_isspace(buffer.value[buffer.position - 1]) || ft_ispunct(buffer.value[buffer.position - 1]))) {
							cursor_left(0); (buffer.position)--;
						}
						while (buffer.position > 0 && !ft_isspace(buffer.value[buffer.position - 1]) && !ft_ispunct(buffer.value[buffer.position - 1])) {
							cursor_left(0);
							do { (buffer.position)--; } while (buffer.position > 0 && (buffer.value[buffer.position] & 0xC0) == 0x80);
						}
					} else {
						cursor_left(0);
						do { (buffer.position)--; } while (buffer.position > 0 && (buffer.value[buffer.position] & 0xC0) == 0x80);
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

		#pragma region Cursor

			static int cursor() {
				char seq[8];
				ft_memset(&seq, 0, sizeof(seq));
				if (buffer.c == 27) {

					fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
					int result = read(STDIN_FILENO, seq, sizeof(seq) - 1);
					fcntl(STDIN_FILENO, F_SETFL, O_SYNC);

					if (result > 0) {
						if (seq[0] == 't') { swap_word(); return (1); }				// ALT + T			- Swap word
						//if (seq[0] == '-') { redo(); return (1); }					// ALT + -			- Redo last action
						if (seq[0] == '[') { seq[1] = modifiers(seq);
							if (seq[1] == 'A') 						arrow_up();		// Up				- History next
							if (seq[1] == 'B') 						arrow_down();	// Down				- History prev
							if (seq[1] == 'D') 						arrow_left();	// Left				- Cursor left
							if (seq[1] == 'C') 						arrow_right();	// Right			- Cursor right
							if (seq[1] == 'H') 						home();			// Home				- Cursor to the start
							if (seq[1] == 'F')						end();			// End				- Cursor to the end
							if (seq[1] == '3' && seq[2] == '~')		delete_char();	// Delete			- Delete
							if (!ft_strncmp(seq + 1, "3;5~", 4))	delete_word();	// CTRL + Delete	- Delete current word
						}
					} return (1);
				} return (0);
			}

		#pragma endregion

	#pragma endregion

	#pragma region Clear Screen

		static void clear_screen() {
			write(STDOUT_FILENO, "\033[H\033[2J", 7);
			if (prompt_PS1) write(STDOUT_FILENO, prompt_PS1, ft_strlen(prompt_PS1));
			write(STDOUT_FILENO, buffer.value, buffer.length);
			buffer.position = buffer.length;
		}

	#pragma endregion

	#pragma region Specials

		static int specials() {
			if 		(buffer.c == 1)		home();					// CTRL + A - Cursor to the start
			else if (buffer.c == 2)		arrow_left();			// CTRL + B - Cursor right
			else if (buffer.c == 4)		delete_char();			// CTRL + D - Delete
			else if (buffer.c == 5)		end();					// CTRL + E - Cursor to the end
			else if (buffer.c == 6)		arrow_right();			// CTRL + F - Cursor left
			else if (buffer.c == 8)		backspace();			// CTRL + H - Backspace
			else if (buffer.c == 11)	delete_end();			// CTRL + K - Delete from cursor to end
			else if (buffer.c == 12)	clear_screen();			// CTRL + L - Clear screen
			else if (buffer.c == 14)	arrow_down();			// CTRL + N - History next
			else if (buffer.c == 16)	arrow_up();				// CTRL + P - History prev
			else if	(buffer.c == 19)	fake_segfault = true;	// CTRL + S - Fake SegFault
			else if (buffer.c == 20)	swap_char();			// CTRL + T - Swap char										(Not working right with multibytes 漢字)
			else if (buffer.c == 21)	backspace_start();		// CTRL + U - Backspace from cursor to start
			else if (buffer.c == 23)	backspace_word();		// CTRL + W	- Backspace current word 
			else if (buffer.c == 31)	undo();					// CTRL + _ - Undo last action								(todo)
			else if (buffer.c == 127)	backspace();			// BackSpace
			else if (buffer.c >= 1 && buffer.c <= 26) ;			// Ignore rest of the specials
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

#pragma region ReadLine

	int readline(int readed) {
		if		(ctrl_d(readed))	return (1);
		else if	(ctrl_c())			return (0);
		else if	(enter())			return (1);
		else if (specials())		return (0);
		else if (cursor())			return (0);
		else if (print_char())		return (0);
		return (0);
	}

#pragma endregion

#pragma region Info

	// set -o emacs

	// ✓	Up				(History Prev)
	// ✓	Down			(History Next)
	// ✓	Left			(Cursor Left)
	// ✓	Right			(Cursor Right)
	// ✓	Home			(Cursor Home)
	// ✓	End				(Cursor End)
	// ✓	CTRL + F		(Cursor Left)
	// ✓	CTRL + B		(Cursor Right)
	// ✓	CTRL + A		(Cursor to the start)
	// ✓	CTRL + E		(Cursor to the end)
	// ✓	CTRL + P		(History Prev)
	// ✓	CTRL + N		(History Next)
	// ✓	CTRL + H		(BackSpace)
	// ✓	CTRL + U		(Backspace from cursor to start)
	// ✓	CTRL + W		(Backspace current word)
	// ✓	CTRL + Delete	(Delete current word)
	// ✓	CTRL + D		(Delete)
	// ✓	CTRL + K		(Delete from cursor to end)
	// ✓	CTRL + L		(Clear Screen)
	// ✓	CTRL + _		(Undo)
	// ✓	CTRL + T		(Swap Char)
	// ✓	ALT  + T		(Swap Word)
	// ✓	CTRL + S		(Fake SegFault)

	// https://pubs.opengroup.org/onlinepubs/9699919799/utilities/sh.html#tag_20_117_13_04

#pragma endregion
