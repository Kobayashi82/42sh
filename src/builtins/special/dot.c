/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dot.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:08:17 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/12 12:52:45 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "main/shell.h"
	#include "utils/utils.h"
	#include "utils/getopt.h"

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Source"

		#pragma region "Help"

			int bt_source_help(int format, int no_print) {
				char *name = "source";
				char *syntax = "source filename [arguments]";
				char *description = "Execute commands from a file in the current shell.";
				char *msg = 
				"    Read and execute commands from FILENAME in the current shell.  The\n"
				"    entries in $PATH are used to find the directory containing FILENAME.\n"
				"    If any ARGUMENTS are supplied, they become the positional parameters\n"
				"    when FILENAME is executed.\n\n"

				"    Exit Status:\n"
				"      Returns the status of the last command executed in FILENAME; fails if\n"
				"      FILENAME cannot be read.\n";

				if (!no_print) print(STDOUT_FILENO, NULL, P_RESET);

				if (format == HELP_SYNTAX) {
					print(STDOUT_FILENO, ft_strjoin(name, ": ", 0),   P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin(syntax, "\n", 0), P_FREE_JOIN);
				}

				if (format == HELP_DESCRIPTION) {
					print(STDOUT_FILENO, ft_strjoin(name, " - ", 0),       P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin(description, "\n", 0), P_FREE_JOIN);
				}

				if (format == HELP_NORMAL) {
					print(STDOUT_FILENO, ft_strjoin(name, ": ", 0),                      P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin(syntax, "\n", 0),                    P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin_sep("    ", description, "\n\n", 0), P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin(msg, "\n", 0),                       P_FREE_JOIN);
				}

				if (format == HELP_MANPAGE) {
					print(STDOUT_FILENO, "NAME\n",                                       P_JOIN);
					print(STDOUT_FILENO, ft_strjoin_sep("    ", name, " - ", 0),         P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin(description, "\n\n", 0),             P_FREE_JOIN);
					print(STDOUT_FILENO, "SYNOPSYS\n",                                   P_JOIN);
					print(STDOUT_FILENO, ft_strjoin_sep("    ", syntax, "\n\n", 0),      P_FREE_JOIN);
					print(STDOUT_FILENO, "DESCRIPTION\n",                                P_JOIN);
					print(STDOUT_FILENO, ft_strjoin_sep("    ", description, "\n\n", 0), P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin(msg, "\n\n", 0),                     P_FREE_JOIN);
					print(STDOUT_FILENO, "SEE ALSO\n    42sh(1)\n\n",                    P_JOIN);
				}

				if (!no_print) print(STDOUT_FILENO, NULL, P_PRINT);

				return (0);
			}

		#pragma endregion

		#pragma region "Version"

			static int version_source() {
				char *msg =
					"source 1.0.\n"
					"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
					"This is free software: you are free to change and redistribute it.\n"
					"There is NO WARRANTY, to the extent permitted by law.\n\n"

					"Written by "DEVELOPER" ("LOGIN42").\n";

				print(STDOUT_FILENO, msg, P_RESET_PRINT);

				return (0);
			}

		#pragma endregion

	#pragma endregion

	#pragma region "Help"

		int bt_dot_help(int format, int no_print) {
			char *name = ".";
			char *syntax = ". filename [arguments]";
			char *description = "Execute commands from a file in the current shell.";
			char *msg =
				"    Read and execute commands from FILENAME in the current shell.  The\n"
				"    entries in $PATH are used to find the directory containing FILENAME.\n"
				"    If any ARGUMENTS are supplied, they become the positional parameters\n"
				"    when FILENAME is executed.\n\n"

				"    Exit Status:\n"
				"      Returns the status of the last command executed in FILENAME; fails if\n"
				"      FILENAME cannot be read.\n";

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
				". 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by "DEVELOPER" ("LOGIN42").\n";

			print(STDOUT_FILENO, msg, P_RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Dot"

	int bt_dot(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		char *syntax = ". filename [arguments]";
		if (argc && !strcmp(argv[0], "source")) syntax = "source filename [arguments]";

		t_parse_result *result = parse_options(argc, argv, "", NULL, long_opts, syntax, IGNORE_OFF);
		if (!result)		return (1);
		if (result->error)	return (free_options(result), 2);

		if (find_long_option(result, "help") && !strcmp(argv[0], "source"))			return (free_options(result), bt_source_help(HELP_NORMAL, 0));
		if (find_long_option(result, "help"))										return (free_options(result), bt_dot_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version") && !strcmp(argv[0], "source"))		return (free_options(result), version_source());
		if (find_long_option(result, "version"))									return (free_options(result), version());


		int ret = 0;


		return (free_options(result), ret);
	}

#pragma endregion
