/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 14:07:05 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/16 18:38:43 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

//	Si hay un emoticono por ejemplo en la columna primera o ultima, se puede desajustar la posicion

#pragma region Variables

	t_terminal	terminal;

#pragma endregion

#pragma region Utils

	#pragma region Start Position (BORRAR)

		// void terminal_start() {
		// 	cursor_get();
		// 	buffer.start_row = buffer.row;
		// 	buffer.start_col = buffer.col;
		// }

	#pragma endregion

	#pragma region Char Width

		#pragma region Char Length

			static int char_length(unsigned char c) {
				if (c >= 0xF0) return (4);  // 4-byte
				if (c >= 0xE0) return (3);  // 3-byte
				if (c >= 0xC0) return (2);  // 2-byte
				if (c < 0x80)  return (1);  // 1-byte
				return (0);                  // Invalid byte
			}

		#pragma endregion

		#pragma region Char Codepoint

			static unsigned int char_codepoint(const char *value, size_t length) {
				unsigned char c = value[0];
				if (c < 0x80)						return (c);
				else if (c < 0xE0 && length >= 2)	return (((c & 0x1F) << 6)  | (value[1] & 0x3F));
				else if (c < 0xF0 && length >= 3)	return (((c & 0x0F) << 12) | ((value[1] & 0x3F) << 6)  | (value[2] & 0x3F));
				else if (c < 0xF8 && length >= 4)	return (((c & 0x07) << 18) | ((value[1] & 0x3F) << 12) | ((value[2] & 0x3F) << 6) | (value[3] & 0x3F));
				else								return (0);
			}

		#pragma endregion

		int char_width(size_t position, char *value) {
			unsigned char c = value[position];
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

	#pragma endregion

#pragma endregion

#pragma region Cursor

	#pragma region Logical (BORRAR)

		// int cursor_logical() {
		// 	size_t position = buffer.start_col;
		// 	size_t length;

		// 	if (prompt_PS1) {
		// 		length = ft_strlen(prompt_PS1);
		// 		for (size_t i = 0; i < length;) {
		// 			int width = char_width(i, prompt_PS1);
		// 			if (width == 0) {
		// 				while (i < length && prompt_PS1[i] != 'm') ++i;
		// 				++i;
		// 			} else { position += width;
		// 				if ((unsigned char)prompt_PS1[i] >= 0xC0) {
		// 					if ((unsigned char)prompt_PS1[i] >= 0xF0)		i += 4; // 4 bytes
		// 					else if ((unsigned char)prompt_PS1[i] >= 0xE0)	i += 3; // 3 bytes
		// 					else											i += 2; // 2 bytes
		// 				} else 												i += 1;	// 1 byte
		// 			}
		// 		}
		// 	}

		// 	length = buffer.position;
		// 	for (size_t i = 0; i < length;) {
		// 		int width = char_width(i, buffer.value);
		// 		if (width == 0) {
		// 			while (i < length && buffer.value[i] != 'm') ++i;
		// 			++i;
		// 		} else { position += width;
		// 			if ((unsigned char)buffer.value[i] >= 0xC0) {
		// 				if ((unsigned char)buffer.value[i] >= 0xF0)			i += 4; // 4 bytes
		// 				else if ((unsigned char)buffer.value[i] >= 0xE0)	i += 3; // 3 bytes
		// 				else												i += 2; // 2 bytes
		// 			} else 													i += 1;	// 1 byte
		// 		}
		// 	}
		// 	return (position);
		// }

	#pragma endregion

	#pragma region Up

		void cursor_up() {
			char *action = tgetstr("up", NULL);

			if (action) write(STDIN_FILENO, action, ft_strlen(action));
		}

	#pragma endregion

	#pragma region Down

		void cursor_down() {
			char *action = tgetstr("do", NULL);

			if (action) write(STDIN_FILENO, action, ft_strlen(action));
		}

	#pragma endregion

	#pragma region Left

		void cursor_left(int moves) {
			char *action = tgetstr("le", NULL);
			if (!moves) moves = char_width(buffer.position, buffer.value);

			cursor_get();
			while (action && moves--) {
				if (!buffer.col)	{ cursor_set(--buffer.row, terminal.columns);		buffer.col = terminal.columns - 1; }
				else				{ write(STDIN_FILENO, action, ft_strlen(action));	buffer.col--; }
			}
		}

	#pragma endregion

	#pragma region Right

		void cursor_right(int moves) {
			char *action = tgetstr("nd", NULL);
			if (!moves) moves = char_width(buffer.position, buffer.value);

			cursor_get();
			while (action && moves--) {
				if (buffer.col == (int)terminal.columns - 1)	{ cursor_set(++buffer.row, 0);						buffer.col = 0; }
				else											{ write(STDIN_FILENO, action, ft_strlen(action));	buffer.col++; }
			}
		}

	#pragma endregion

	#pragma region Move

		void cursor_move(size_t from_pos, size_t to_pos) {
			if (from_pos > to_pos) {
				for (size_t i = to_pos; i < from_pos; ) {
					if (char_width(i, buffer.value) == 2) cursor_left(1);
					if ((unsigned char)buffer.value[i] >= 0xC0) {
						if ((unsigned char)buffer.value[i] >= 0xF0)			i += 4;	// 4 bytes
						else if ((unsigned char)buffer.value[i] >= 0xE0)	i += 3;	// 3 bytes
						else												i += 2;	// 2 bytes
					} else													i++;	// 1 byte
					cursor_left(1);
				}
			} else if (from_pos < to_pos) {
				for (size_t i = from_pos; i < to_pos; ) {
					if (char_width(i, buffer.value) == 2) cursor_right(1);
					if ((unsigned char)buffer.value[i] >= 0xC0) {
						if ((unsigned char)buffer.value[i] >= 0xF0)			i += 4;	// 4 bytes
						else if ((unsigned char)buffer.value[i] >= 0xE0)	i += 3;	// 3 bytes
						else												i += 2;	// 2 bytes
					} else													i++;	// 1 byte
					cursor_right(1);
				}	
			}
		}

	#pragma endregion

	#pragma region Get

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
						if (a++ == 0)	buffer.row = value - 1;
						else			buffer.col = value - 1;
					} i++;
				}
			}
		}

	#pragma endregion

	#pragma region Set

		void cursor_set(int row, int col) {
			char *action = tgetstr("cm", NULL);

			if (action) action = tgoto(action, col, row);
			if (action) write(STDOUT_FILENO, action, ft_strlen(action));
		}

	#pragma endregion

	#pragma region Hide

		void cursor_hide() {
			char *action = tgetstr("vi", NULL);

			if (action) write(STDIN_FILENO, action, ft_strlen(action));
		}

	#pragma endregion

	#pragma region Show

		void cursor_show() {
			char *action = tgetstr("ve", NULL);

			if (action) write(STDIN_FILENO, action, ft_strlen(action));
		}

	#pragma endregion

#pragma endregion

#pragma region Initialize

	int terminal_initialize() {
		char *termtype = getenv("TERM");
		if (!termtype) termtype = "dumb";

		int success = tgetent(NULL, termtype);
		if (success < 0)	{ write(2, "Could not access the termcap data base.\n", 41);	return (1); }
		if (success == 0)	{ write(2, "Terminal type is not defined.\n", 31);				return (1); }

		terminal.rows = tgetnum("li");
		terminal.columns = tgetnum("co");
		return (0);
	}

#pragma endregion
