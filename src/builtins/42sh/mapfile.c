/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mapfile.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:08:17 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/12 12:24:54 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "main/shell.h"
	#include "utils/utils.h"
	#include "utils/getopt.h"

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Readarray"

		#pragma region "Help"

			int bt_readarray_help(int format, int no_print) {
				char *name = "readarray";
				char *syntax = "readarray [-d delim] [-n count] [-O origin] [-s count] [-t] [-u fd] [-C callback] [-c quantum] [array]";
				char *description = "Read lines from a file into an array variable.";
				char *msg = "    A synonym for `mapfile'.  See `help mapfile'.\n";

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

			static int version_readarray() {
				char *msg =
					"readarray 1.0.\n"
					"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
					"This is free software: you are free to change and redistribute it.\n"
					"There is NO WARRANTY, to the extent permitted by law.\n\n"

					"Written by "DEVELOPER" ("LOGIN42").\n";

				print(STDOUT_FILENO, msg, P_RESET_PRINT);

				return (0);
			}

		#pragma endregion

	#pragma endregion

	#pragma region "Declare"

		#pragma region "Help"

			int bt_mapfile_help(int format, int no_print) {
				char *name = "mapfile";
				char *syntax = "mapfile [-d delim] [-n count] [-O origin] [-s count] [-t] [-u fd] [-C callback] [-c quantum] [array";
				char *description = "Read lines from the standard input into an indexed array variable.";
				char *msg =
					"    Read lines from the standard input into the indexed array variable ARRAY, or\n"
					"    from file descriptor FD if the -u option is supplied.  The variable MAPFILE\n"
					"    is the default ARRAY.\n\n"

					"    Options:\n"
					"      -d delim       use DELIM to terminate lines, instead of newline\n"
					"      -n count       copy at most COUNT lines.  If COUNT is 0, all lines are copied\n"
					"      -O origin      begin assigning to ARRAY at index ORIGIN.  The default index is 0\n"
					"      -s count       discard the first COUNT lines read\n"
					"      -t             remove a trailing DELIM from each line read (default newline)\n"
					"      -u fd          read lines from file descriptor FD instead of the standard input\n"
					"      -C callback    evaluate CALLBACK each time QUANTUM lines are read\n"
					"      -c quantum     specify the number of lines read between each call to CALLBACK\n\n"

					"    Arguments:\n"
					"      ARRAY          array variable name to use for file data\n\n"

					"    If -C is supplied without -c, the default quantum is 5000.  When\n"
					"    CALLBACK is evaluated, it is supplied the index of the next array\n"
					"    element to be assigned and the line to be assigned to that element\n"
					"    as additional arguments.\n\n"

					"    If not supplied with an explicit origin, mapfile will clear ARRAY before\n"
					"    assigning to it.\n\n"

					"    Exit Status:\n"
					"      Returns success unless an invalid option is given or ARRAY is readonly or\n"
					"      not an indexed array.\n";

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

			static int version() {
				char *msg =
					"mapfile 1.0.\n"
					"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
					"This is free software: you are free to change and redistribute it.\n"
					"There is NO WARRANTY, to the extent permitted by law.\n\n"

					"Written by "DEVELOPER" ("LOGIN42").\n";

				print(STDOUT_FILENO, msg, P_RESET_PRINT);

				return (0);
			}

		#pragma endregion

	#pragma endregion

#pragma endregion

#pragma region "Mapfile"

	int bt_mapfile(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		char *syntax = "mapfile [-d delim] [-n count] [-O origin] [-s count] [-t] [-u fd] [-C callback] [-c quantum] [array]";
		if (argc && !strcmp(argv[0], "readarray")) syntax = "readarray [-d delim] [-n count] [-O origin] [-s count] [-t] [-u fd] [-C callback] [-c quantum] [array]";

		t_parse_result *result = parse_options(argc, argv, "", NULL, long_opts, syntax, IGNORE_OFF);
		if (!result)		return (1);
		if (result->error)	return (free_options(result), 2);

		if (find_long_option(result, "help") && !strcmp(argv[0], "readarray"))		return (free_options(result), bt_readarray_help(HELP_NORMAL, 0));
		if (find_long_option(result, "help"))										return (free_options(result), bt_mapfile_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version") && !strcmp(argv[0], "readarray"))	return (free_options(result), version_readarray());
		if (find_long_option(result, "version"))									return (free_options(result), version());


		int ret = 0;


		return (free_options(result), ret);
	}

#pragma endregion
