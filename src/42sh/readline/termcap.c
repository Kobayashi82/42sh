/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 14:07:05 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/04 23:07:21 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Utils

	#pragma region Chars

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

		#pragma region Char Width

			int char_width(size_t position, char *value) {
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

#pragma endregion

void init_terminal_data() {
	char *termtype = getenv("TERM");
	if (!termtype) termtype = "dumb";

	int success = tgetent(NULL, termtype);
	if (success < 0)	write(2, "Could not access the termcap data base.\n", 41);
	if (success == 0)	write(2, "Terminal type is not defined.\n", 31);
}

void move_cursor(int x, int y) {
    char *move_cmd;
    char buffer[2048]; // Buffer para almacenar el comando de movimiento

    // Obtén el código para mover el cursor a una posición específica (x, y)
    move_cmd = tgetstr("cm", NULL); // "cm" es el código de comando para "cursor motion"

    // Si `move_cmd` no es NULL, mueve el cursor a la posición solicitada
    if (move_cmd != NULL) {
        snprintf(buffer, sizeof(buffer), move_cmd, y, x); // Formateamos el código con las coordenadas (y, x)
        fputs(buffer, stdout); // Enviar el comando a la terminal
    } else {
        fprintf(stderr, "No se pudo obtener el comando para mover el cursor.\n");
    }
}

void cursor_up() {
	char *action = tgetstr("up", NULL);

	if (action) write(1, action, ft_strlen(action));
}

void cursor_down() {
	char *action = tgetstr("do", NULL);

	if (action) write(1, action, ft_strlen(action));
}

void cursor_left(int moves) {
    char *action = tgetstr("le", NULL);
	if (!moves) moves = char_width(buffer.position, buffer.value);

	while (action && moves--) {
		get_cursor_position();
		if (buffer.col == 0) {
			buffer.col = 30;
			cursor_up();
		}
		write(1, action, ft_strlen(action));
	}
}

void cursor_right(int moves) {
    char *action = tgetstr("nd", NULL);
	if (!moves) moves = char_width(buffer.position, buffer.value);

	while (action && moves--) write(1, action, ft_strlen(action));
}

void cursor_to_pos(size_t from_pos, size_t to_pos) {
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

void get_cursor_position() {
    char	buf[32];
    int		a = 0, i = 0;

	ft_memset(buf, 0, sizeof(buf));
    write(1, "\033[6n", 4);
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

    // printf("row: %d - col: %d", buffer.row, buffer.col); fflush(stdout);
}

// void get_cursor_position() {
//     char buf[4];
//     int a = 0, i = 0;

//     write(1, "\033[6n", 4);
//     read(1, buf, 4);

//     while (buf[i]) {
//         if (buf[i] >= 48 && buf[i] <= 57) {
//             if (a == 0)	buffer.row = atoi(&buf[i]) - 1;
//             else		buffer.col = atoi(&buf[i]) - 1;
//             a++;
//         } i++;
//     }
// 	printf("row: %d - col: %d", buffer.row, buffer.col); fflush(stdout);
// }
