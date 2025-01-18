/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vi.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 09:42:13 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/18 18:46:58 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

//	Optimizar Delete y BackSpace
//	Arreglar multibytes con navegacion

#pragma region Variables

	enum e_mode { CURSOR, AFTER_CURSOR, FIRST, LAST };

	static bool	number_mode;
	static char	n[7], last_cmd, last_char[7], *clipboard;

	static void	insert_mode(int mode);
	static int	get_n();
	static void beep() { write(STDOUT_FILENO, "\a", 1); }

#pragma endregion

#pragma region Input

	#pragma region Insert

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
				if (n <= 0 || buffer.c == 4) {
					if (!buffer.length) {
						sfree(buffer.value); buffer.value = NULL;
						write(STDOUT_FILENO, "\r\n", 2);
					} else {
						insert_mode(CURSOR);
						history_set_pos_end();
						buffer.value[buffer.length] = '\0';
						write(STDOUT_FILENO, "\r\n", 2);
						if (tmp_line) { sfree(tmp_line); tmp_line = NULL; }
					}
					return (1);
				} return (0);
			}

		#pragma endregion

		#pragma region SIGINT

			static int ctrl_c() {
				if (buffer.c == 3) {	// Ctrl+C
					insert_mode(CURSOR);
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
					insert_mode(CURSOR);
					history_set_pos_end();
					buffer.value[buffer.length] = '\0';
					write(STDOUT_FILENO, "\r\n", 2);
					if (tmp_line) { sfree(tmp_line); tmp_line = NULL; }
					return (1);
				} return (0);
			}

		#pragma endregion

		#pragma region BackSpace

			#pragma region Char

				static void backspace() {
					if (buffer.position > 0) {
						size_t back_pos = 1;
						while (buffer.position - back_pos > 0 && (buffer.value[buffer.position - back_pos] & 0xC0) == 0x80) back_pos++;
						if (buffer.position < buffer.length) ft_memmove(&buffer.value[buffer.position - back_pos], &buffer.value[buffer.position], buffer.length - buffer.position);
						cursor_left(0);
						buffer.position -= back_pos; buffer.length -= back_pos;

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

						if (vi_mode == EDIT) {
							cursor_left(0);
							if (buffer.position) buffer.position--;
						}
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

						if (vi_mode == EDIT) {
							cursor_left(0);
							if (buffer.position) buffer.position--;
						}
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
					int extra = (vi_mode == EDIT);

					while (buffer.position < buffer.length - extra) {
						cursor_right(0);
						do { (buffer.position)++; } while (buffer.position < buffer.length - extra && (buffer.value[buffer.position] & 0xC0) == 0x80);
					}
				}

			#pragma endregion

			#pragma region Arrow Up

				static void arrow_up() {
					int number = get_n();
					if (!vi_mode) number = 1;

					int hist_pos = history_get_pos() - (number - 1);
					if (hist_pos < 0) hist_pos = 0;
					if (number > 1) history_set_pos(hist_pos);
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

					if (vi_mode) {
						home();
						buffer.position = 0;
					}
				}

			#pragma endregion

			#pragma region Arrow Down

				static void arrow_down() {
					int number = get_n();
					if (!vi_mode) number = 1;

					int hist_pos = history_get_pos() + (number - 1);
					if (number > 1) history_set_pos(hist_pos);
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

					if (vi_mode) {
						home();
						buffer.position = 0;
					}

					if (free_line && new_line) sfree(new_line);
				}

			#pragma endregion

			#pragma region Arrow Left

				static void arrow_left() {
					int number = get_n();
					if (!vi_mode) number = 1;

					while (number--) {
						if (!buffer.ALT && !buffer.SHIFT && buffer.position > 0) {
							if (buffer.CTRL) {
								while (buffer.position > 0 && (ft_isspace(buffer.value[buffer.position - 1]) || ft_ispunct(buffer.value[buffer.position - 1]))) {
									cursor_left(0); (buffer.position)--;
								}
								while (buffer.position > 0 && !ft_isspace(buffer.value[buffer.position - 1]) && !ft_ispunct(buffer.value[buffer.position - 1])) {
									cursor_left(0);
									do { (buffer.position)--; } while (buffer.position > 0 && (buffer.value[buffer.position] & 0xC0) == 0x80);
								}
							} else if (buffer.position) {
								cursor_left(0);
								do { (buffer.position)--; } while (buffer.position > 0 && (buffer.value[buffer.position] & 0xC0) == 0x80);
							}
						}
					}
				}

			#pragma endregion

			#pragma region Arrow Right

				static void arrow_right() {
					int extra = (vi_mode == EDIT);

					int number = get_n();
					if (!vi_mode) number = 1;

					while (number--) {
						if (!buffer.ALT && !buffer.SHIFT && buffer.position < buffer.length - extra) {
							if (buffer.CTRL) {
								while (buffer.position < buffer.length - extra && (ft_isspace(buffer.value[buffer.position]) || ft_ispunct(buffer.value[buffer.position]))) {
									cursor_right(0); (buffer.position)++;
								}
								while (buffer.position < buffer.length - extra && !ft_isspace(buffer.value[buffer.position]) && !ft_ispunct(buffer.value[buffer.position])) {
									cursor_right(0);
									do { (buffer.position)++; } while (buffer.position < buffer.length - extra && (buffer.value[buffer.position] & 0xC0) == 0x80);
								}
							} else {
								if (buffer.position < buffer.length - extra) {
									cursor_right(0);
									do { (buffer.position)++; } while (buffer.position < buffer.length - extra && (buffer.value[buffer.position] & 0xC0) == 0x80);
								}
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
						} else if (!vi_mode) {
							vi_mode = EDIT;												// Escape			- Edit mode
							cursor_left(0);
							if (buffer.position) buffer.position--;
						} return (1);
					} return (0);
				}

			#pragma endregion

		#pragma endregion

		#pragma region Print Char

			static int print_char() {
				if (vi_mode) return (0);

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

	#pragma region Edit

		#pragma region Number

			static int get_n() {
				return (ft_max(1, ft_atoi(n)));
			}

			static void set_n() {
				if (!number_mode && buffer.c == 48) { home(); return; }					// Move to the start of the line
				if (!number_mode) {
					number_mode = true;
					ft_memset(n, 0, 7);
				}
				int pos = ft_max(ft_strlen(n), 0);
				if (pos > 6) {
					number_mode = false;
					ft_memset(n, 0, 7);
					return;
				}
				n[pos] = buffer.c;
			}

		#pragma endregion

		#pragma region Insert Mode

			static void insert_mode(int mode) {
				vi_mode = INSERT;
				last_cmd = 0;
				ft_memset(last_char, 0, 7);

				switch (mode) {
					case CURSOR: break;
					case AFTER_CURSOR: {
						cursor_right(0);
						if (buffer.position < buffer.length) buffer.position++;
						break;
					}
					case FIRST:	{ home();	break; }
					case LAST:	{ end();	break; }
				}
			}

		#pragma endregion

		#pragma region Delete

			static void n_backspace() {
				int number = get_n();

				while (number--) backspace();
			}

			static void n_delete_char() {
				int number = get_n();

				while (number--) delete_char();
			}

			static void n_delete_to() {

			}

		#pragma endregion

		#pragma region Copy

			static void copy(bool to_end) {
				if (to_end) {
					if (clipboard) sfree(clipboard);
					clipboard = ft_strdup(&buffer.value[buffer.position]);
				}
			}

		#pragma endregion

		#pragma region Paste

			static void paste(bool reverse) {
				if (!clipboard || !*clipboard) return;

				int number = get_n();

				while (number--) {
					if (reverse)	arrow_left();
					else			{ insert_mode(CURSOR); arrow_right(); }

					// Expand buffer if necessary
					while (buffer.length + ft_strlen(clipboard) >= buffer.size) {
						buffer.value = ft_realloc(buffer.value, buffer.size, buffer.size * 2);
						buffer.size *= 2;
					}

					if (buffer.position < buffer.length) ft_memmove(&buffer.value[buffer.position + ft_strlen(clipboard)], &buffer.value[buffer.position], buffer.length - buffer.position);

					// Insert all bytes of the character into the buffer
					ft_memcpy(&buffer.value[buffer.position], clipboard, ft_strlen(clipboard));

					buffer.length += ft_strlen(clipboard);

					write(STDOUT_FILENO, &buffer.value[buffer.position], buffer.length - buffer.position);
					buffer.position += ft_strlen(clipboard);

					// Adjust the cursor position in the terminal
					size_t move_back = 0;
					for (size_t i = buffer.position; i < buffer.length; ) {
						if (char_width(i, buffer.value) == 2) move_back++;
						if ((unsigned char)buffer.value[i] >= 0xC0) {
							if ((unsigned char)buffer.value[i] >= 0xF0)			i += 4;	// 4 bytes
							else if ((unsigned char)buffer.value[i] >= 0xE0)	i += 3;	// 3 bytes
							else												i += 2;	// 2 bytes
						} else													i++;	// 1 byte
						move_back++;
					}
					while (move_back--) cursor_left(1);

					if (!reverse) {
						vi_mode = EDIT;
						cursor_left(0);
						if (buffer.position) buffer.position--;
					}
				}
			}

		#pragma endregion

		#pragma region Navigation

			#pragma region Word

				static void left_start_word() {

				}

				static void right_start_word() {

				}

				static void right_end_word() {
					
				}

			#pragma endregion

			#pragma region BigWord

				static void left_start_bigword() {

				}

				static void right_start_bigword() {

				}

				static void right_end_bigword() {
					
				}

			#pragma endregion

			#pragma region GoTo

				#pragma region Char

					static void goto_char(char cmd) {
						char c[7];
						ft_memset(c, 0, 7);

						if (!cmd) {
							if (read(STDIN_FILENO, c, 7) < 1) return;
							ft_memcpy(last_char, c, 7);
							last_cmd = cmd = buffer.c;
						} else ft_memcpy(c, last_char, 7);

						int number = get_n();
						size_t last_match = buffer.position;

						if (cmd == 'F' || cmd == 'T') {
							for (size_t i = buffer.position - 1; i != (size_t)-1; --i) {
								if (!ft_strncmp(&buffer.value[i], c, ft_strlen(c))) {
									while (buffer.position > i) arrow_left(0);
									if (cmd == 'T') arrow_right();
									last_match = buffer.position;
									if (!--number) return;
								}
							}
						} else {
							for (size_t i = buffer.position + 1; i < buffer.length; ++i) {
								if (!ft_strncmp(&buffer.value[i], c, ft_strlen(c))) {
									while (buffer.position < i) arrow_right(0);
									if (cmd == 't') arrow_left();
									last_match = buffer.position;
									if (--number == 0) return;
								}
							}
						}

						if (buffer.position == last_match) beep();
						while (buffer.position > last_match) arrow_left(0);
						while (buffer.position < last_match) arrow_right(0);
					}

				#pragma endregion

				#pragma region Position

					static void goto_position() {
						int number = get_n();
						
						home();
						while (--number) arrow_right();
					}

				#pragma endregion

				#pragma region No IsSpace

					static void goto_no_isspace() {
						
					}

				#pragma endregion

	#pragma endregion

			#pragma region Repeat CMD

				static void repeat_cmd(bool reverse) {
					if (!last_cmd || !last_char[0]) return;
					
					if (reverse) {
						if (last_cmd == 'f')	goto_char('F');
						if (last_cmd == 'F')	goto_char('f');
						if (last_cmd == 't')	goto_char('T');
						if (last_cmd == 'T')	goto_char('t');					
					} else						goto_char(last_cmd);
				}

			#pragma endregion

		#pragma endregion

		#pragma region Undo

			static void n_undo() {

			}

			static void n_undo_all() {
				
			}

		#pragma endregion

		#pragma region Edit Input

			static int edit_input() {
				char *raw_editor = "nano";//default_editor();
				char tmp_file[] = "/tmp/input_XXXXXX";
				int fd = create_temp_file(tmp_file);
				if (fd == -1) { beep(); return (1); }

				if (write(fd, buffer.value, buffer.length) == -1) {
					sclose(fd);
					unlink(tmp_file);
					return (1);
				} sclose(fd);

				char *editor = get_fullpath(raw_editor);
				if (access(editor, X_OK) == -1) {
					unlink(tmp_file);
					beep();
					return (1);
				}

				pid_t pid = fork();
				if (pid < 0) {
					unlink(tmp_file);
					beep();
					return (1);
				} else if (pid == 0) {
					char *const args[] = { (char *)editor, tmp_file, NULL };
					char **env = variables_to_array(vars_table, EXPORTED, true);
					sclose_all();
					execve(editor, args, env);
					beep(); sexit(EXIT_FAILURE);
				} else if (pid > 0) {
					int status;
					waitpid(pid, &status, 0);

					fd = sopen(tmp_file, O_RDONLY, -1);
					if (fd == -1) {
						unlink(tmp_file);
						beep();
						return (1);
					}

					char	*file_content = NULL;
					char	temp_buffer[1024];
					size_t	file_size = 0;
					int		readed = 0;

					while ((readed = read(fd, temp_buffer, sizeof(temp_buffer))) > 0) {
						file_content = ft_realloc(file_content, ft_strlen(file_content), file_size + readed + 1);
						ft_memcpy(file_content + file_size, temp_buffer, readed);
						file_size += readed;
					}

					if (readed < 0) {
						beep();
						unlink(tmp_file);
					} else {
						file_content[file_size] = '\0';

						if (file_size > 0 && file_content[file_size - 1] == '\n') {
							file_content[file_size - 1] = '\0';
							file_size--;
						}
						if (file_size > 0 && file_content[file_size - 1] == '\r') {
							file_content[file_size - 1] = '\0';
							file_size--;
						}

						sfree(buffer.value);
						buffer.value = file_content;
						print(STDOUT_FILENO, "\n", RESET);
						print(STDOUT_FILENO, buffer.value, PRINT);
					}

					sclose(fd);
					unlink(tmp_file);

					insert_mode(CURSOR);
					history_set_pos_end();
					write(STDOUT_FILENO, "\r\n", 2);
					if (tmp_line) { sfree(tmp_line); tmp_line = NULL; }
					return (2);
				} return (1);
			}

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

	#pragma endregion

	#pragma region Specials

		static int specials() {
			if (buffer.c == 127 && !vi_mode)			backspace();					//	[BackSpace]	Delete the previous character						(Only in insertion mode)
			else if (buffer.c == 8 && !vi_mode)			backspace();					//	[CTRL + H]	Delete the previous character						(Only in insertion mode)
			else if	(buffer.c == 19 && !vi_mode)		fake_segfault = true;			//	[CTRL + S]	Fake SegFault										(Only in insertion mode)
			else if (buffer.c == 20)					swap_char();					//	[CTRL + T]	Swap the current character with the previous one	(Not working right with multibytes 漢字)
			else if (buffer.c == 21)					backspace_start();				//	[CTRL + U]	Backspace from cursor to the start of the line
			else if (buffer.c == 23)					backspace_word();				//	[CTRL + W]	Backspace the current word
			else if (buffer.c == 31)					undo();							//	[CTRL + _]	Undo the last change
			else if (buffer.c >= 1 && buffer.c <= 26)	;								//	Ignore other CTRL + X commands
			else if (vi_mode) {
				if (buffer.c >= '0' && buffer.c <= '9')	set_n();						//	Set the repetition number for commands

				else if (buffer.c == 'i')	insert_mode(CURSOR);						//	Enter insert mode at the cursor position
				else if (buffer.c == 'I')	insert_mode(FIRST);							//	Enter insert mode at the beginning of the line
				else if (buffer.c == 'a')	insert_mode(AFTER_CURSOR);					//	Enter insert mode after the cursor position
				else if (buffer.c == 'A')	insert_mode(LAST);							//	Enter insert mode at the end of the line
				else if (buffer.c == 'c')	{ insert_mode(CURSOR); n_delete_to();   }	//- [n]	Delete up to the specified position and enter insert mode (0, ^, $, c)
				else if (buffer.c == 'C')	{ insert_mode(CURSOR); delete_end();    }	//	Delete from cursor to the end of the line and enter insert mode
				else if (buffer.c == 's')	{ insert_mode(CURSOR); n_delete_char(); }	//	[n] Delete the current character and enter insert mode
				else if (buffer.c == 'S')	{ insert_mode(FIRST);  delete_end();    }	//	Delete the entire line and enter insert mode
				else if (buffer.c == 'd')	n_delete_to();								//- [n]	Delete up to the specified position (0, ^, $, d)
				else if (buffer.c == 'D')	delete_end();								//	Delete from cursor to the end of the line
				else if (buffer.c == 'x')	n_delete_char();							//	[n] Delete the current character
				else if (buffer.c == 'X')	n_backspace();								//	[n] Delete the previous character (backspace)

				else if (buffer.c == 'r')	;											//-	[n]	Replace the current character with the specified one
				else if (buffer.c == 'R')	;											//-	[n]	Enter replace mode: allows replacing characters one by one
				else if (buffer.c == 'y')	copy(false);								//-	Copy up to the specified position (0, ^, $, c)
				else if (buffer.c == 'Y')	copy(true);									//	Copy from the current position to the end of the line
				else if (buffer.c == 'p')	paste(false);								//	[n]	Paste copied text after the cursor
				else if (buffer.c == 'P')	paste(true);								//	[n]	Paste copied text before the cursor

				else if (buffer.c == 'b')	left_start_word();							//-	[n] Move the cursor to the beginning of the previous word
				else if (buffer.c == 'B')	left_start_bigword();						//-	[n] Move the cursor to the beginning of the previous big word
				else if (buffer.c == 'w')	right_start_word();							//-	[n] Move the cursor to the beginning of the next word
				else if (buffer.c == 'W')	right_start_bigword();						//-	[n] Move the cursor to the beginning of the next big word
				else if (buffer.c == 'e')	right_end_word();							//-	[n] Move the cursor to the end of the current word
				else if (buffer.c == 'E')	right_end_bigword();						//-	[n] Move the cursor to the end of the current big word

				else if (buffer.c == '^')	goto_no_isspace();							//-	Move the cursor to the first non-whitespace character in the line
				else if (buffer.c == 'f')	goto_char(0);								//	[n] Move the cursor forward to the character specified
				else if (buffer.c == 'F')	goto_char(0);								//	[n] Move the cursor backward to the character specified
				else if (buffer.c == 't')	goto_char(0);								//	[n] Move the cursor forward one character before the character specified
				else if (buffer.c == 'T')	goto_char(0);								//	[n] Move the cursor backward one character after the character specified
				else if (buffer.c == ';')	repeat_cmd(false);							//	[n] Repeat the last character search command (f, F, t, T)
				else if (buffer.c == ',')	repeat_cmd(true);							//	[n] Repeat the last character search command (f, F, t, T) in reverse direction
				else if (buffer.c == '|')	goto_position();							//	[n] Move the cursor to a specific character position (default is 1)

				else if (buffer.c == 'k')	arrow_up();									//	[n] Move the cursor up
				else if (buffer.c == 'j')	arrow_down();								//	[n] Move the cursor down
				else if (buffer.c == 'h')	arrow_left();								//	[n] Move the cursor left
				else if (buffer.c == 'l')	arrow_right();								//	[n] Move the cursor right
				else if (buffer.c == ' ')	arrow_right();								//	[n] Move the cursor right
				else if (buffer.c == '$')	end();										//	Move the cursor to the end of the line
				else if (buffer.c == 'u')	n_undo();									//-	[n] Undo the last change
				else if (buffer.c == 'U')	n_undo_all();								//-	Undo all changes
				else if (buffer.c == 'v')	return (edit_input());						//	Edit the input using the default editor, and terminate the input upon exiting the editor
				else if	(buffer.c == '#')	return (comment());							//	Comment and terminate the input

				else return (0);
			} else return (0);

			return (1);
		}

	#pragma endregion

#pragma endregion

#pragma region Vi

	int vi(int readed) {
		int result = 0;

		if (vi_mode && (buffer.c < '0' || buffer.c > '9') && !number_mode)	ft_memset(n, 0, 7);
		if (vi_mode && (buffer.c < '0' || buffer.c > '9') && number_mode)	number_mode = false;

		if		(ctrl_d(readed))		result = 1;
		else if	(ctrl_c())				result = 0;
		else if	(enter())				result = 1;
		else if ((result = specials()))	result = (result == 2);
		else if (cursor())				result = 0;
		else if (print_char())			result = 0;
		
		if (result && clipboard) sfree(clipboard);
		return (result);
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

	//	Insertion Mode
	//	--------------
	// ✓	Up				(History Prev)
	// ✓	Down			(History Next)
	// ✓	Left			(Cursor Left)
	// ✓	Right			(Cursor Right)
	// ✓	Home			(Cursor Home)
	// ✓	End				(Cursor End)
	// ✓	CTRL + H		(BackSpace)
	// ✓	CTRL + U		(Backspace from cursor to start)
	// ✓	CTRL + W		(Backspace current word)
	// ✓	CTRL + Delete	(Delete current word)
	// ✓	CTRL + _		(Undo)
	// ✓	CTRL + T		(Swap Char)
	// ✓	CTRL + S		(Fake SegFault)

	// https://pubs.opengroup.org/onlinepubs/9699919799/utilities/sh.html#tag_20_117_13_04

	// space	32
	// #		35
	// $		36
	// ,		44
	// 0-9		48-57
	// ;		59
	// A		65
	// B		66
	// C		67
	// D		68
	// E		69
	// F		70
	// I		73
	// P		80
	// R		82
	// S		83
	// T		84
	// U		85
	// W		87
	// X		88
	// Y		89
	// ^		94
	// a		97
	// b		98
	// c		99
	// d		100
	// e		101
	// f		102
	// h		104
	// i		105
	// j		106
	// k		107
	// l		108
	// p		112
	// r		114
	// t		116
	// u		117
	// v		118
	// w		119
	// x		120
	// y		121
	// |		124

#pragma endregion
