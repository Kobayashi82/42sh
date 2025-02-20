/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_search.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 15:20:34 by vzurera-          #+#    #+#             */
/*   Updated: 2025/02/20 17:39:09 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"
	#include "terminal/readinput/termcaps.h"
	#include "terminal/readinput/readinput.h"
	#include "terminal/readinput/prompt.h"
	#include "terminal/readinput/history.h"

#pragma endregion

#pragma region "Variables"

	t_buffer	search_buffer;

	//static char *white = "\033[30;47m";
	//static char *reset = "\033[0m";

	static void beep() { write(STDOUT_FILENO, "\a", 1); }
	
	static void search_history_exit();

#pragma endregion

#pragma region "Input"

	#pragma region "Insert"

		#pragma region "SIG_INT"						("CTRL + C")

			static int ctrl_c() {
				if (buffer.c == 3) {
					search_history_exit();
					return (1);
				} return (0);
			}

		#pragma endregion

		#pragma region "NewLine"						("Enter")

			static int enter() {
				if (buffer.c == '\r' || buffer.c == '\n') {
					search_history_exit();
					buffer.value[buffer.length] = '\0';
					write(STDOUT_FILENO, "\r\n", 2);
					history_set_pos_end();
					if (tmp_line) { sfree(tmp_line); tmp_line = NULL; }
					return (1);
				} return (0);
			}

		#pragma endregion

		#pragma region "BackSpace"						("BackSpace")

			static void backspace() {
				if (!search_buffer.length || !search_buffer.position || search_buffer.position > search_buffer.length) { beep(); return; }

				if (search_buffer.position > 0) {
					size_t back_pos = 1;

					int len = chars_width(0, search_buffer.position, search_buffer.value);
					if (len > 0) cursor_left(len);
					len += chars_width(search_buffer.position, search_buffer.length, search_buffer.value) + 3;
					len += chars_width(0, buffer.length, buffer.value);
					for (int i = len; i > 0; --i) write_value(STDOUT_FILENO, " ", 1);
					if (len > 0) cursor_left(len);

					while (search_buffer.position - back_pos > 0 && (search_buffer.value[search_buffer.position - back_pos] & 0xC0) == 0x80) back_pos++;
					search_buffer.position -= back_pos; search_buffer.length -= back_pos;

					write_value(STDOUT_FILENO, search_buffer.value, search_buffer.length);
					write_value(STDOUT_FILENO, "': ", 3);
					write_value(STDOUT_FILENO, buffer.value, buffer.length);
					len = 3 + chars_width(0, buffer.length, buffer.value) + chars_width(search_buffer.position, search_buffer.length, search_buffer.value);
					if (len > 0) cursor_left(len);
				}
			}

		#pragma endregion

		#pragma region "Char"

			static int print_char() {			
				size_t c_size = char_size(buffer.c);
				
				char new_char[c_size + 1];
				new_char[0] = buffer.c;
				for (size_t i = 1; i < c_size; i++) read(STDIN_FILENO, &new_char[i], 1);
				new_char[c_size] = '\0';
				
				//	Ignore multi-space chars
				if (char_width(0, new_char) > 1) return (1);
				
				int len = chars_width(0, search_buffer.position, search_buffer.value);
				if (len > 0) cursor_left(len);
				len += chars_width(search_buffer.position, search_buffer.length, search_buffer.value) + 3;
				len += chars_width(0, buffer.length, buffer.value);
				for (int i = len; i > 0; --i) write_value(STDOUT_FILENO, " ", 1);
				if (len > 0) cursor_left(len);

				// Expand buffer if necessary
				if (search_buffer.position + c_size >= search_buffer.size) {
					search_buffer.value = ft_realloc(search_buffer.value, search_buffer.size, search_buffer.size * 2);
					search_buffer.size *= 2;
				}
				
				// Insert all bytes of the character into the buffer
				for (size_t i = 0; i < c_size; i++) search_buffer.value[search_buffer.position++] = new_char[i];
				search_buffer.length += c_size;
				
				write_value(STDOUT_FILENO, search_buffer.value, search_buffer.length);
				write_value(STDOUT_FILENO, "': ", 3);
				write_value(STDOUT_FILENO, buffer.value, buffer.length);
				len = 3 + chars_width(0, buffer.length, buffer.value) + chars_width(search_buffer.position, search_buffer.length, search_buffer.value);
				if (len > 0) cursor_left(len);

				return (1);
			}

		#pragma endregion

	#pragma endregion

	#pragma region "Handle"

		#pragma region "Cursor"

			static int cursor() {
				char seq[8];
				ft_memset(&seq, 0, sizeof(seq));
				if (buffer.c == 27) {

					fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
					read(STDIN_FILENO, seq, sizeof(seq) - 1);
					fcntl(STDIN_FILENO, F_SETFL, O_SYNC);

					search_history_exit();
					return (1);
				} return (0);
			}

		#pragma endregion

		#pragma region "Specials"

			static int specials() {
				if (buffer.c == 127)						{ backspace(); 				return (1); }	//	[BackSpace]	Delete the previous character
				else if (buffer.c >= 1 && buffer.c <= 26)	{ search_history_exit(); 	return (1); }	//	Ignore other CTRL + X commands
				return (0);
			}

		#pragma endregion

	#pragma endregion

#pragma endregion

#pragma region "Search History"					("CTRL + R")

	#pragma region "Init"

		void search_history_init() {
			searching = true;
			search_buffer.size = 1024;
			search_buffer.position = 0, search_buffer.length = 0;
			search_buffer.value = ft_calloc(search_buffer.size, sizeof(char));
			char *prompt = remove_colors(prompt_PS1);
			int len = chars_width(0, ft_strlen(prompt), prompt);
			sfree(prompt);
			len += chars_width(0, buffer.position, buffer.value);
			if (len > 0) cursor_left(len);
			len += chars_width(buffer.position, buffer.length, buffer.value);
			for (int i = len; i > 0; --i) write_value(STDOUT_FILENO, " ", 1);
			if (len > 0) cursor_left(len);
			write_value(STDOUT_FILENO, "history search:`", 16);
			write_value(STDOUT_FILENO, "': ", 3); cursor_left(3);
		}

	#pragma endregion

	#pragma region "Exit"

		static void search_history_exit() {
			searching = false;
			int len = 16 + chars_width(0, search_buffer.position, search_buffer.value);
			if (len > 0) cursor_left(len);
			len = 19 + chars_width(0, search_buffer.length, search_buffer.value) + chars_width(0, buffer.length, buffer.value);
			for (int i = len; i > 0; --i) write_value(STDOUT_FILENO, " ", 1);
			if (len > 0) cursor_left(len);
			if (prompt_PS1) {
				write(STDOUT_FILENO, prompt_PS1, ft_strlen(prompt_PS1));
				cursor_update(nocolor_length(prompt_PS1));
			}
			write_value(STDOUT_FILENO, buffer.value, buffer.length);
			len = chars_width(buffer.position, buffer.length, buffer.value);
			if (len > 0) cursor_left(len);
			sfree(search_buffer.value);
		}

	#pragma endregion

	#pragma region "Search"

		int search_history() {
			if		(ctrl_c())			return (0);
			else if	(enter())			return (1);
			else if (specials())		return (0);
			else if (cursor())			return (0);
			else if (print_char())		return (0);

			search_history_exit();
			return (0);
		}

	#pragma endregion

#pragma endregion
