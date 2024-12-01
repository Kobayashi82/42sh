/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 10:32:07 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/01 20:55:27 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Variables

	bool show_control_chars = true;

#pragma endregion

#pragma region Raw Mode

	// Restaurar la terminal a su estado original
	static void disable_raw_mode() {
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &data.terminal.term);
	}

	// Poner la terminal en modo raw
	static void enable_raw_mode() {
		struct termios raw;

		// Guardar la configuración actual de la terminal
		tcgetattr(STDIN_FILENO, &data.terminal.term);
		atexit(disable_raw_mode); // Restaurar al salir del programa

		raw = data.terminal.term;

		// Configurar modo raw
		raw.c_lflag &= ~(ECHO | ICANON | ISIG);	// Desactivar eco y modo canónico, pero no señales (ISIG)
		raw.c_iflag &= ~(IXON | ICRNL);         // Desactivar Ctrl+S/Ctrl+Q y conversión CR a NL
		raw.c_oflag &= ~(OPOST);                // Desactivar procesamiento de salida
		raw.c_cc[VMIN] = 1;                     // Leer de a un carácter
		raw.c_cc[VTIME] = 0;                    // Sin tiempo de espera

		// Aplicar configuración
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
	}

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
				disable_raw_mode();
			}
			return (result);
		}

	#pragma endregion

	#pragma region C_SIGINT

		static int check_SIGINT(const char c, size_t *position, const char *prompt) {
			if (c == 3) {	// Manejar Ctrl+C
				*position = 0;
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

		static int check_nl(const char c, const size_t position, char *buffer) {
			if (c == '\r' || c == '\n') {
				buffer[position] = '\0';
				write(STDOUT_FILENO, "\r\n", 2);
				return (1);
			}
			return (0);
		}

	#pragma endregion

	#pragma region Cursors

		static int check_cursors(const char c, size_t *position, char *buffer) {
			if (c == 127) {	// Backspace
				if (*position > 0) {
					do { (*position)--; } while (*position > 0 && (buffer[*position] & 0xC0) == 0x80);
					write(STDOUT_FILENO, "\b \b", 3);
				}
				return (1);
			}
			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region ReadLine

	char *readline(const char* prompt) {
		size_t buffer_size = 1024;
		size_t position = 0;
		char *buffer = safe_malloc(buffer_size);
		char c;

		enable_raw_mode();
		if (prompt) write(STDOUT_FILENO, prompt, ft_strlen(prompt));

		while (1) {
			int n = read(STDIN_FILENO, &c, 1);

			int result = check_EOF(n, c, position, buffer);
			if (result == 1)								continue;
			else if (result == 2)							return (NULL);

			if (check_SIGINT(c, &position, prompt))			continue;
			if (check_nl(c, position, buffer))				break; 
			else if (check_cursors(c, &position, buffer))	;
			else if (position < buffer_size - 1) {
				buffer[position++] = c;
				write(STDOUT_FILENO, &c, 1);
			}

			if (position >= buffer_size - 1) {
				buffer = safe_realloc(buffer, buffer_size, buffer_size * 2);
				if (!buffer) { free(buffer); exit_error(NO_MEMORY, 1, NULL, true); }
				buffer_size *= 2;
			}
		}

		disable_raw_mode();
		return (buffer);
	}

#pragma endregion
