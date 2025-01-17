/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vi.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 09:42:13 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/17 13:40:19 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

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
					do { (buffer.position)--; } while (buffer.position > 0 && (buffer.value[buffer.position] & 0xC0) == 0x80);
					cursor_left(0);
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

		#pragma region Cursor

			static int cursor() {
				char seq[8];
				ft_memset(&seq, 0, sizeof(seq));
				if (buffer.c == '\033') {
					if (read(STDIN_FILENO, seq, sizeof(seq) - 1) > 0) {
						if (seq[0] == '-') { redo(); return (1); }					// ALT + -			- Redo last action
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

	#pragma region Comment

		static int comment() {
			if (buffer.length + 1 >= buffer.size) {
				buffer.value = ft_realloc(buffer.value, buffer.size, buffer.size * 2);
				buffer.size *= 2;
			}

			home();
			ft_memmove(&buffer.value[1], &buffer.value[0], buffer.length);
			buffer.value[0] = '#';
			buffer.length++;

			history_set_pos_end();
			buffer.value[buffer.length] = '\0';
			write(STDOUT_FILENO, buffer.value, buffer.length);
			write(STDOUT_FILENO, "\r\n", 2);
			if (tmp_line) { sfree(tmp_line); tmp_line = NULL; }

			return (2);
		}

	#pragma endregion

	#pragma region Specials

		static int specials() {
			if (buffer.c == 8 && !vi_mode)	backspace();					// ✓	CTRL + H	- Backspace										(Only in insertion mode)
			else if	(buffer.c == 19 && !vi_mode) fake_segfault = true;		// ✓	CTRL + S	- Fake SegFault									(Only in insertion mode)
			else if (buffer.c == 20)		swap_char();					// ✓	CTRL + T	- Swap char										(Not working right with multibytes 漢字)
			else if (buffer.c == 21)		backspace_start();				// ✓	CTRL + U	- Backspace from cursor to start of line
			else if (buffer.c == 23)		backspace_word();				// ✓	CTRL + W	- Backspace current word 
			else if (buffer.c == 31)		undo();							// ✗	CTRL + _	- Undo last action
			else if (buffer.c == 127)		backspace();					// ✓	BackSpace	- Backspace char
			else if (buffer.c >= 1 && buffer.c <= 26) ;						// ✓	Ignore rest of the specials
			else if (vi_mode) {
				if (buffer.c == 32)			arrow_right();					// ✓	SPACE		- Cursor right
				else if	(buffer.c == 35)	return (comment());				// ✓	#			- Comment and done
				else if (buffer.c == 36)	arrow_left();					// ✗	$			- mueve al final de la línea
				else if (buffer.c == 44)	arrow_left();					// ✗	,			- repite el comando mas reciente (f, F, t T) en modo reverse
				else if (buffer.c == 48)	arrow_left();					// ✗	0			- mueve al principio de la línea
				else if (buffer.c >= 49 && buffer.c <= 57)	arrow_left();	// ✗	1-9			- introduce el numero [n]
				else if (buffer.c == 59)	arrow_left();					// ✗	;			- repite el comando mas reciente (f, F, t T)

				else if (buffer.c == 65)	arrow_left();					// ✗	A			- modo inserción al final de la linea
				else if (buffer.c == 66)	arrow_left();					// ✗	B			- Cursor left start bigword
				else if (buffer.c == 67)	arrow_left();					// ✗	C			- delete from cursor to end of line
				else if (buffer.c == 68)	arrow_left();					// ✗	D			- elimina desde el carácter actual hasta el final de la linea
				else if (buffer.c == 69)	arrow_left();					// ✗	E			- Cursor right end bigword
				else if (buffer.c == 70)	arrow_left();					// ✗	F			- mueve al primer carácter antes del cursor
				else if (buffer.c == 73)	arrow_left();					// ✗	I			- modo inserción al principio de la linea
				else if (buffer.c == 80)	arrow_left();					// ✗	P			- pega lo copiado antes del cursor
				else if (buffer.c == 82)	arrow_left();					// ✗	R			- modo inserción pero hace algo raro
				else if (buffer.c == 83)	arrow_left();					// ✗	S			- elimina la linea y entra en modo insercion
				else if (buffer.c == 84)	arrow_left();					// ✗	T			- mueve al carácter posterior al primer carácter antes del cursor
				else if (buffer.c == 85)	arrow_left();					// ✗	U			- undo all changes
				else if (buffer.c == 87)	arrow_left();					// ✗	W			- Cursor right start bigword
				else if (buffer.c == 88)	arrow_left();					// ✗	X			- Delete previous char
				else if (buffer.c == 89)	arrow_left();					// ✗	Y			- copia hasta el final de la linea

				else if (buffer.c == 94)	arrow_left();					// ✗	^			- mueve al primer carácter de la linea que no sea un isspace

				else if (buffer.c == 97)	arrow_left();					// ✗	a			- modo inserción después del cursor
				else if (buffer.c == 98)	arrow_left();					// ✗	b			- Cursor left start word
				else if (buffer.c == 99)	arrow_left();					// ✗	c			- elimina hasta donde se indique (0, ^, $, c)	(Mirar manual para mas info)
				else if (buffer.c == 100)	arrow_left();					// ✗	d			- elimina hasta donde se indique (0, ^, $, d)	(Mirar manual para mas info)
				else if (buffer.c == 101)	arrow_left();					// ✗	e			- Cursor right end word
				else if (buffer.c == 102)	arrow_left();					// ✗	f			- mueve al primer carácter después del cursor
				else if (buffer.c == 104)	arrow_left();					// ✓	h			- Cursor left
				else if (buffer.c == 105)	arrow_left();					// ✗	i			- modo inserción donde el cursor
				else if (buffer.c == 106)	arrow_left();					// ✗	j			- algo que ver con comandos y el historial
				else if (buffer.c == 107)	arrow_left();					// ✗	k			- establece la linea por el comando del historial indicado
				else if (buffer.c == 108)	arrow_right();					// ✓	l			- Cursor right
				else if (buffer.c == 112)	arrow_left();					// ✗	p			- pega lo copiado después del cursor
				else if (buffer.c == 114)	arrow_left();					// ✗	r			- reemplaza el carácter actual por el indicado
				else if (buffer.c == 116)	arrow_left();					// ✗	t			- mueve al carácter anterior al primer carácter después del cursor
				else if (buffer.c == 117)	arrow_left();					// ✗	u			- undo last change
				else if (buffer.c == 118)	arrow_left();					// ✗	v			- abre vi... y mas polleces
				else if (buffer.c == 119)	arrow_left();					// ✗	w			- Cursor right start word
				else if (buffer.c == 120)	arrow_left();					// ✗	x			- delete char
				else if (buffer.c == 121)	arrow_left();					// ✗	y			- copia hasta donde se indique (0, ^, $, c)		(Mirar manual para mas info)

				else if (buffer.c == 124)	arrow_left();					// ✗	|			- mueve al carácter (1 por defecto)

				else return (0);
			} else return (0);

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

#pragma region Vi

	int vi(int readed) {
		int result = 0;

		vi_mode = EDIT;	// BORRAR

		if		(ctrl_d(readed))		return (1);
		else if	(ctrl_c())				return (0);
		else if	(enter())				return (1);
		else if ((result = specials()))	return (result == 2);
		else if (cursor())				return (0);
		else if (print_char())			return (0);
		return (0);
	}

#pragma endregion

#pragma region Info

	// set -o vi

	//	General Mode
	//	---------------
	// ✓	Up				(History Prev)
	// ✓	Down			(History Next)
	// ✓	Left			(Cursor Left)
	// ✓	Right			(Cursor Right)
	// ✓	Home			(Cursor Home)
	// ✓	End				(Cursor End)
	// ✓	CTRL + H		(BackSpace)							- only in insertion mode
	// ✓	CTRL + U		(Backspace from cursor to start)
	// ✓	CTRL + W		(Backspace current word)
	// ✓	CTRL + Delete	(Delete current word)
	// ✓	CTRL + _		(Undo)
	// ✓	CTRL + T		(Swap Char)
	// ✓	CTRL + S		(Fake SegFault)						- only in insertion mode

	// ✓	#				(Comment and end input)
	// ✓	h				cursor left
	// ✓	l, space		cursor right
	// ✗	w				cursor right start word
	// ✗	W				cursor right start bigword
	// ✗	e				cursor right end word
	// ✗	E				cursor right end bigword
	// ✗	b				cursor left  start word
	// ✗	B				cursor left  start bigword
	// ✗	x				delete char																	(cut line really)
	// ✗	X				delete previous char														(cut line really)
	// ✗	d				elimina hasta donde se indique (0, ^, $, d) mirar manual para mas info		(cut line really)
	// ✗	D				elimina desde el carácter actual hasta el final de la linea					(cut line really)
	// ✗	a				modo inserción después del cursor
	// ✗	i				modo inserción donde el cursor
	// ✗	A				modo inserción al final de la linea
	// ✗	I				modo inserción al principio de la linea
	// ✗	R				modo inserción pero hace algo raro
	// ✗	^				mueve al primer carácter de la linea que no sea un isspace
	// ✗	$				mueve al final de la línea
	// ✗	0				mueve al principio de la línea
	// ✗	[n] |			mueve al carácter (1 por defecto)
	// ✗	f[c]			mueve al primer carácter después del cursor
	// ✗	F[c]			mueve al primer carácter antes del cursor
	// ✗	t				mueve al carácter anterior al primer carácter después del cursor
	// ✗	T				mueve al carácter posterior al primer carácter antes del cursor
	// ✗	;				repite el comando mas reciente (f, F, t T)
	// ✗	,				repite el comando mas reciente (f, F, t T) en modo reverse
	// ✗	C				delete from cursor to end of line
	// ✗	S				elimina la linea y entra en modo insercion
	// ✗	c				elimina hasta donde se indique (0, ^, $, c) mirar manual para mas info
	// ✗	r[c]			reemplaza el carácter actual por el indicado
	// ✗	y				copia hasta donde se indique (0, ^, $, c)
	// ✗	Y				copia hasta el final de la linea
	// ✗	p				pega lo copiado después del cursor
	// ✗	P				pega lo copiado antes del cursor
	// ✗	u				undo last change
	// ✗	U				undo all changes
	// ✗	[n] k			establece la linea por el comando del historial indicado
	// ✗	j				algo que ver con comandos y el historial
	// ✗	v				abre vi... y mas polleces

	// https://pubs.opengroup.org/onlinepubs/9699919799/utilities/sh.html#tag_20_117_13_04

#pragma endregion
