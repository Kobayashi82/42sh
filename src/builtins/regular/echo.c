/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:08:56 by vzurera-          #+#    #+#             */
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

		int bt_echo_help(int format, int no_print) {
			char *name = "echo";
			char *syntax = "echo [-neE] [arg ...]";
			char *description = " Write arguments to the standard output.";
			char *msg =
				"    Display the ARGs, separated by a single space character and followed by a\n"
				"    newline, on the standard output.\n\n"

				"    Options:\n"
				"      -n        do not append a newline\n"
				"      -e        enable interpretation of the following backslash escapes\n"
				"      -E        explicitly suppress interpretation of backslash escapes\n\n"

				"    `echo' interprets the following backslash-escaped characters:\n"
				"      \\a         alert (bell)\n"
				"      \\b         backspace\n"
				"      \\c         suppress further output\n"
				"      \\e         escape character\n"
				"      \\E         escape character\n"
				"      \\f         form feed\n"
				"      \\n         new line\n"
				"      \\r         carriage return\n"
				"      \\t         horizontal tab\n"
				"      \\v         vertical tab\n"
				"      \\          backslash\n"
				"      \\0nnn      the character whose ASCII code is NNN (octal).  NNN can be\n"
				"                 0 to 3 octal digits\n"
				"      \\xHH       the eight-bit character whose value is HH (hexadecimal).  HH\n"
				"                 can be one or two hex digits\n"
				"      \\uHHHH     the Unicode character whose value is the hexadecimal value HHHH.\n"
				"                 HHHH can be one to four hex digits.\n"
				"      \\UHHHHHHHH the Unicode character whose value is the hexadecimal value\n"
				"                 HHHHHHHH. HHHHHHHH can be one to eight hex digits.\n\n"

				"    Exit Status:\n"
				"      Returns success unless a write error occurs.\n";

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
				"echo 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by "DEVELOPER" ("LOGIN42").\n";

			print(STDOUT_FILENO, msg, P_RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Echo"

	int bt_echo(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, "neE", NULL, long_opts, "echo [-neE] [arg ...]", IGNORE_STRING);
		if (!result)		return (1);

		if (find_long_option(result, "help"))		return (free_options(result), bt_echo_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());


		int ret = 0;

		int enable_ansi = 0;
		for (t_opt_value *curr = result->options; curr; curr = curr->next) {
			if (curr->opt == 'E') enable_ansi = 0;
			if (curr->opt == 'e') enable_ansi = 1;
		}

		print(STDOUT_FILENO, NULL, P_RESET);

		for (int i = 0; i < result->argc; ++i) {
			if (enable_ansi) ansi_escapes(result->argv[i]);
			print(STDOUT_FILENO, result->argv[i],               P_JOIN);
			if (i + 1 < result->argc) print(STDOUT_FILENO, " ", P_JOIN);
		}

		if (has_option(result, 'n'))	print(STDOUT_FILENO, NULL, P_PRINT);
		else							print(STDOUT_FILENO, "\n", P_PRINT);

		return (free_options(result), ret);
	}

#pragma endregion
