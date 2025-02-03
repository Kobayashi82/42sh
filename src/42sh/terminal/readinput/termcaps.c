/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcaps.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 14:07:05 by vzurera-          #+#    #+#             */
/*   Updated: 2025/02/02 17:00:38 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//	Si hay un emoticono por ejemplo en la columna primera o ultima, se puede desajustar la posicion
//	Actualizar la posicion del cursor en todo momento

#pragma region "Includes"

	#include "libft.h"
	#include "terminal/terminal.h"
	#include "terminal/readinput/termcaps.h"
	#include "terminal/readinput/readinput.h"
	#include "main/options.h"

	#include <termcap.h>

#pragma endregion

#pragma region "Variables"

	t_terminal		terminal;

	static size_t	row, col;
	//static size_t	row_bk, col_bk;

#pragma endregion

#pragma region "Utils"

	size_t char_size(unsigned char c) {
		if (c >= 0xF0) return(4);
		if (c >= 0xE0) return(3);
		if (c >= 0xC0) return(2);

		return (1);
	}

	#pragma region "Char Width"

		#pragma region "Length"

			static int char_length(unsigned char c) {
				if (c >= 0xF0) return (4);  // 4-byte
				if (c >= 0xE0) return (3);  // 3-byte
				if (c >= 0xC0) return (2);  // 2-byte
				if (c < 0x80)  return (1);  // 1-byte
				return (0);                  // Invalid byte
			}

		#pragma endregion

		#pragma region "Codepoint"

			static unsigned int char_codepoint(const char *value, size_t length) {
				unsigned char c = value[0];
				if (c < 0x80)						return (c);
				else if (c < 0xE0 && length >= 2)	return (((c & 0x1F) << 6)  | (value[1] & 0x3F));
				else if (c < 0xF0 && length >= 3)	return (((c & 0x0F) << 12) | ((value[1] & 0x3F) << 6)  | (value[2] & 0x3F));
				else if (c < 0xF8 && length >= 4)	return (((c & 0x07) << 18) | ((value[1] & 0x3F) << 12) | ((value[2] & 0x3F) << 6) | (value[3] & 0x3F));
				else								return (0);
			}

		#pragma endregion

		#pragma region "Width"

			size_t char_width(size_t position, const char *value) {
				unsigned char c = value[position];

				if (c == '\0' || c == '\a' || c == '\b' || c == '\f' ||	c == '\r' || c == '\v') return (0);
				else if (c == '\t') return (8);

				if (c == '\033') { size_t i = position + 1;
					if (value[i] == '[') {
						while (value[++i]) {
							if ((value[i] >= 'A' && value[i] <= 'Z') || (value[i] >= 'a' && value[i] <= 'z'))	return (0);
							if (!(value[i] >= '0' && value[i] <= '9') && value[i] != ';' && value[i] != '[')	break;
						}
					} return (0);
				}

				unsigned int codepoint = char_codepoint(&value[position], char_length(value[position]));
				if ((codepoint >= 0x1100  && codepoint <= 0x115F)	||			// Hangul Jamo
					(codepoint >= 0x2329  && codepoint <= 0x232A)	||			// Angle brackets
					(codepoint >= 0x2E80  && codepoint <= 0x9FFF)	||			// CJK, radicals, etc.
					(codepoint >= 0xAC00  && codepoint <= 0xD7A3)	||			// Hangul syllables
					(codepoint >= 0xF900  && codepoint <= 0xFAFF)	||			// CJK compatibility
					(codepoint >= 0xFE10  && codepoint <= 0xFE19)	||			// Vertical forms
					(codepoint >= 0x1F300 && codepoint <= 0x1F64F)	||			// Emojis
					(codepoint >= 0x1F900 && codepoint <= 0x1F9FF))	return (2);	// Supplemental Symbols
				return (1);
			}

			size_t chars_width(size_t from, size_t to, const char *value) {
				size_t total_chars = 0;

				if (from < to) {
					while (from < to) {
						total_chars += char_width(from, value);
						do { from++; } while (from < to && (value[from] & 0xC0) == 0x80);
					}
				} else if (to < from) {
					while (to < from) {
						total_chars += char_width(to, value);
						do { to++; } while (to < from && (value[to] & 0xC0) == 0x80);
					}
				}

				return (total_chars);
			}

		#pragma endregion

	#pragma endregion

	#pragma region "Char Prev"

		size_t char_prev(size_t position, const char *value) {
			if (position > 0)
				do { position--; } while (position > 0 && (value[position] & 0xC0) == 0x80);
			return (position);
		}

	#pragma endregion

#pragma endregion

#pragma region "Cursor"

	#pragma region "Navigation"

		#pragma region "Up"

			void cursor_up() {
				cursor_set(row - 1, col);
			}

		#pragma endregion

		#pragma region "Down"

			void cursor_down() {
				cursor_set(row + 1, col);
			}

		#pragma endregion

		#pragma region "Left"

			void cursor_left(int moves) {
				if (!moves) moves = char_width(buffer.position, buffer.value);

				while (moves--) {
					if (!col)	cursor_set(row - 1, terminal.columns - 1);
					else		cursor_set(row, col - 1);
				}
			}

		#pragma endregion

		#pragma region "Right"

			void cursor_right(int moves) {
				if (!moves && buffer.position == buffer.length) return;
				if (!moves) moves = char_width(buffer.position, buffer.value);

				while (moves--) {
					if (col >= terminal.columns - 1)	cursor_set(row + 1, 0);
					else								cursor_set(row, col + 1);
				}
			}

		#pragma endregion

		#pragma region "Move"

			// void cursor_move(size_t from_pos, size_t to_pos) {
			// 	if (from_pos > to_pos) {
			// 		for (size_t i = to_pos; i < from_pos; ) {
			// 			if (char_width(i, buffer.value) == 2) cursor_left(1);
			// 			if ((unsigned char)buffer.value[i] >= 0xC0) {
			// 				if ((unsigned char)buffer.value[i] >= 0xF0)			i += 4;	// 4 bytes
			// 				else if ((unsigned char)buffer.value[i] >= 0xE0)	i += 3;	// 3 bytes
			// 				else												i += 2;	// 2 bytes
			// 			} else													i++;	// 1 byte
			// 			cursor_left(1);
			// 		}
			// 	} else if (from_pos < to_pos) {
			// 		for (size_t i = from_pos; i < to_pos; ) {
			// 			if (char_width(i, buffer.value) == 2) cursor_right(1);
			// 			if ((unsigned char)buffer.value[i] >= 0xC0) {
			// 				if ((unsigned char)buffer.value[i] >= 0xF0)			i += 4;	// 4 bytes
			// 				else if ((unsigned char)buffer.value[i] >= 0xE0)	i += 3;	// 3 bytes
			// 				else												i += 2;	// 2 bytes
			// 			} else													i++;	// 1 byte
			// 			cursor_right(1);
			// 		}	
			// 	}
			// }

		#pragma endregion

	#pragma endregion

	#pragma region "Get"

		void cursor_get() {
			char	buf[32]; ft_memset(buf, 0, sizeof(buf));
			int		a = 0, i = 0;
			char *action = tgetstr("u7", NULL);

			if (action) {
				write(STDIN_FILENO, action, ft_strlen(action));
				read(STDIN_FILENO, buf, sizeof(buf) - 1);

				while (buf[i]) {
					if (buf[i] >= '0' && buf[i] <= '9') {
						int start = i;
						while (buf[i] >= '0' && buf[i] <= '9') i++;
						int value = ft_atoi(&buf[start]);
						if (a++ == 0)	row = value - 1;
						else			col = value - 1;
					} i++;
				}
			}
		}

	#pragma endregion

	#pragma region "Set"

		// void cursor_set(size_t new_row, size_t new_col) {
		// 	char *action = tgetstr("cm", NULL);

		// 	if (new_row == terminal.rows) new_row--;
		// 	if (action) action = tgoto(action, new_col, new_row);
		// 	if (action) {
		// 		write(STDOUT_FILENO, action, ft_strlen(action));
		// 		row = new_row; col = new_col;
		// 	}
		// }

		// void popo() {
		// 	row_bk = row;
		// 	col_bk = col;
		// }


		// void papa() {
		// 	if (col_bk == terminal.columns - 1) {
		// 		col_bk = 0; row_bk++;
		// 	} else {
		// 		col_bk++;
		// 	}
		// 	cursor_set(row_bk, col_bk);
		// }


	#pragma endregion

	#pragma region "Hide"

		void cursor_hide() {
			char *action = tgetstr("vi", NULL);

			if (action) write(STDIN_FILENO, action, ft_strlen(action));
		}

	#pragma endregion

	#pragma region "Show"

		void cursor_show() {
			char *action = tgetstr("ve", NULL);

			if (action) write(STDIN_FILENO, action, ft_strlen(action));
		}

	#pragma endregion

#pragma endregion

#pragma region "Initialize"

	int terminal_initialize() {
		char *termtype = getenv("TERM");
		if (!termtype) { termtype = "dumb"; options.emacs = false; options.vi = false; }

		int success = tgetent(NULL, termtype);
		if (success < 0)	{ write(2, "Could not access the termcap data base.\n", 41);	return (1); }
		if (success == 0)	{ write(2, "Terminal type is not defined.\n", 31);				return (1); }

		terminal.rows = tgetnum("li");
		terminal.columns = tgetnum("co");

		return (0);
	}

#pragma endregion



#pragma region "Mierdas"

	#pragma region "Move"

		void cursor_move(size_t from, size_t to) {
			// ft_printf(1, "%u\n", col);
			if (from < to) {
				int total = chars_width(from, to, buffer.value);
				if (total) cursor_right(total);
			} else if (from > to) {
				int total = chars_width(to, from, buffer.value);
				if (total) cursor_left(total);
			}
			// ft_printf(1, "%u\n", col);
		}

	#pragma endregion

	#pragma region "Set"

		// void cursor_set(size_t new_row, size_t new_col) {
		// 	while (row != new_row) {
		// 		if (new_row > row) {
		// 			char *action = tgetstr("do", NULL);
		// 			if (action) { write(STDIN_FILENO, action, ft_strlen(action)); row++; }
		// 		} else if (new_row < row) {
		// 			if (row <= 0) break;
		// 			char *action = tgetstr("up", NULL);
		// 			if (action) { write(STDIN_FILENO, action, ft_strlen(action)); row--; }
		// 		}
		// 	}
		// 	while (col != new_col) {
		// 		if (new_col > col) {
		// 			if (col > terminal.columns - 1) break;
		// 			char *action = tgetstr("nd", NULL);
		// 			if (action) { write(STDIN_FILENO, action, ft_strlen(action)); col++; }
		// 			else break;
		// 		} else if (new_col < col) {
		// 			if (col <= 0) break;
		// 			char *action = tgetstr("le", NULL);
		// 			if (action) { write(STDIN_FILENO, action, ft_strlen(action)); col--; }
		// 			else break;
		// 		}
		// 	}
		// }

		void cursor_set(size_t new_row, size_t new_col) {
			if (new_row >= terminal.rows)
				new_row = terminal.rows - 1;
			char *action = tgetstr("cm", NULL);
			if (action) {
				action = tgoto(action, new_col, new_row);
				write(STDIN_FILENO, action, ft_strlen(action));
				row = new_row; col = new_col;
			}
		}

	#pragma endregion

	#pragma region "Update"

		void cursor_update(size_t length) {
			while (length--) {
				if (++col > terminal.columns - 1)	{
					if (row < terminal.rows) row++;
					col = 0;
				}
			}
		}

	#pragma endregion

	#pragma region "Write Value"

		int write_value(int fd, const char *value, size_t length) {
			if (fd < 0 || !value || length <= 0) return (1);
			int total = 0;

			for (int i = 0; value[i]; i++) {
				if (write(fd, &value[i], 1) == -1) break;
				total++;
			}
			telemetry();
			cursor_update(chars_width(0, total, value));
			telemetry();

			return (total);
		}

	#pragma endregion

	#pragma region "Telemetry"

		int telemetry() {
			int fd = open("putamierda", O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (!fd) return (-1);

			ft_printf(fd, "row: %u - col: %u - pos:%u - rows: %u\n", row, col, buffer.position, terminal.rows);
			close(fd);
			return (0);
		}

	#pragma endregion

#pragma endregion
