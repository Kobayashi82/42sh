/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:08:17 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/21 21:55:08 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "internal/shell.h"
	#include "utils/utils.h"
	#include "utils/getopt.h"

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Help"

		int bt_read_help(int format, int no_print) {
			char *name = "read";
			char *syntax = "read [-ers] [-a array] [-d delim] [-i text] [-n nchars] [-N nchars] [-p prompt] [-t timeout] [-u fd] [name ...]";
			char *description = "Read a line from the standard input and split it into fields.";
			char *msg =
				"    Reads a single line from the standard input, or from file descriptor FD\n"
				"    if the -u option is supplied.  The line is split into fields as with word\n"
				"    splitting, and the first word is assigned to the first NAME, the second\n"
				"    word to the second NAME, and so on, with any leftover words assigned to\n"
				"    the last NAME.  Only the characters found in $IFS are recognized as word\n"
				"    delimiters. By default, the backslash character escapes delimiter characters\n"
				"    and newline.\n\n"

				"    If no NAMEs are supplied, the line read is stored in the REPLY variable.\n\n"

				"    Options:\n"
				"      -a array   assign the words read to sequential indices of the array\n"
				"                 variable ARRAY, starting at zero\n"
				"      -d delim   continue until the first character of DELIM is read, rather\n"
				"                 than newline\n"
				"      -e         use Readline to obtain the line\n"
				"      -i text    use TEXT as the initial text for Readline\n"
				"      -n nchars  return after reading NCHARS characters rather than waiting\n"
				"                 for a newline, but honor a delimiter if fewer than\n"
				"                 NCHARS characters are read before the delimiter\n"
				"      -N nchars  return only after reading exactly NCHARS characters, unless\n"
				"                 EOF is encountered or read times out, ignoring any\n"
				"                 delimiter\n"
				"      -p prompt  output the string PROMPT without a trailing newline before\n"
				"                 attempting to read\n"
				"      -r         do not allow backslashes to escape any characters\n"
				"      -s         do not echo input coming from a terminal\n"
				"      -t timeout time out and return failure if a complete line of\n"
				"                 input is not read within TIMEOUT seconds.  The value of the\n"
				"                 TMOUT variable is the default timeout.  TIMEOUT may be a\n"
				"                 fractional number.  If TIMEOUT is 0, read returns\n"
				"                 immediately, without trying to read any data, returning\n"
				"                 success only if input is available on the specified\n"
				"                 file descriptor.  The exit status is greater than 128\n"
				"                 if the timeout is exceeded\n"
				"      -u fd      read from file descriptor FD instead of the standard input\n\n"

				"    Exit Status:\n"
				"      The return code is zero, unless end-of-file is encountered, read times out\n"
				"      (in which case it's greater than 128), a variable assignment error occurs,\n"
				"      or an invalid file descriptor is supplied as the argument to -u.\n";

			if (!no_print) print(STDOUT_FILENO, NULL, P_RESET);

				if (format == HELP_SYNTAX) {
					print(STDOUT_FILENO, ft_strjoin(name, ": ",   J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin(syntax, "\n", J_FREE_NONE), P_FREE_JOIN);
				}

				if (format == HELP_DESCRIPTION) {
					print(STDOUT_FILENO, ft_strjoin(name, " - ",       J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin(description, "\n", J_FREE_NONE), P_FREE_JOIN);
				}

				if (format == HELP_NORMAL) {
					print(STDOUT_FILENO, ft_strjoin(name, ": ",                      J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin(syntax, "\n",                    J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin_sep("    ", description, "\n\n", J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin(msg, "\n",                       J_FREE_NONE), P_FREE_JOIN);
				}

				if (format == HELP_MANPAGE) {
					print(STDOUT_FILENO, "NAME\n",                                                 P_JOIN);
					print(STDOUT_FILENO, ft_strjoin_sep("    ", name, " - ",         J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin(description, "\n\n",             J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, "SYNOPSYS\n",                                             P_JOIN);
					print(STDOUT_FILENO, ft_strjoin_sep("    ", syntax, "\n\n",      J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, "DESCRIPTION\n",                                          P_JOIN);
					print(STDOUT_FILENO, ft_strjoin_sep("    ", description, "\n\n", J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin(msg, "\n\n",                     J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, "SEE ALSO\n    42sh(1)\n\n",                              P_JOIN);
				}

			if (!no_print) print(STDOUT_FILENO, NULL, P_PRINT);

			return (0);
		}

	#pragma endregion

	#pragma region "Version"

		static int version() {
			char *msg =
				"read 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by "DEVELOPER" ("LOGIN42").\n";

			print(STDOUT_FILENO, msg, P_RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Read"

	int bt_read(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, "", NULL, long_opts, "read [-ers] [-a array] [-d delim] [-i text] [-n nchars] [-N nchars] [-p prompt] [-t timeout] [-u fd] [name ...]", IGNORE_OFF);
		if (!result)		return (1);

		if (find_long_option(result, "help"))		return (free_options(result), bt_read_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());


		int ret = 0;


		return (free_options(result), ret);
	}

#pragma endregion

// if (delimiter != '\n') {
//     // Cambia a modo raw/non-canonical
//     struct termios raw;
//     tcgetattr(STDIN_FILENO, &raw);
//     raw.c_lflag &= ~ICANON;  // Desactiva buffering de línea
//     tcsetattr(STDIN_FILENO, TCSANOW, &raw);
// }

// while (read(STDIN_FILENO, &c, 1) > 0) {
//     if (c == delimiter) break;
    
//     if (c == 127 || c == 8) {  // Backspace/Delete
//         // TÚ tienes que:
//         // 1. Borrar del buffer
//         if (buffer_len > 0) {
//             buffer_len--;
//             // 2. Borrar visualmente en pantalla
//             write(STDOUT_FILENO, "\b \b", 3);  // retrocede, espacio, retrocede
//         }
//     } else {
//         buffer[buffer_len++] = c;
//         // Echo manual si ECHO está desactivado
//         // write(STDOUT_FILENO, &c, 1);
//     }
// }
