/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   times.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:08:17 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/24 17:08:34 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "internal/shell.h"
	#include "utils/utils.h"
	#include "utils/getopt.h"

	#include <sys/times.h>

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Help"

		int bt_times_help(int format, int no_print) {
			char *name = "times";
			char *syntax = "times";
			char *description = "Display process times.";
			char *msg =
				"    Prints the accumulated user and system times for the shell and all of its\n"
				"    child processes.\n\n"

				"    Exit Status:\n"
				"      Always succeeds.\n";

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
				"times 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by "DEVELOPER" ("LOGIN42").\n";

			print(STDOUT_FILENO, msg, P_RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Times"

	int bt_times(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, "", NULL, long_opts, "times", IGNORE_OFF);
		if (!result) return (free_options(result), (shell.error == E_OPT_MAX || shell.error == E_OPT_INVALID) ? 2 : 1);

		if (find_long_option(result, "help"))		return (free_options(result), bt_times_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());


		int ret = 0;
		struct tms	buf;
		long		clk_tck;
		char		output[256];
		
		clk_tck = sysconf(_SC_CLK_TCK);
		if (clk_tck == -1 || times(&buf) == (clock_t)-1) ret = 1;
		else {
			snprintf(output, sizeof(output), "%ldm%.3fs %ldm%.3fs\n%ldm%.3fs %ldm%.3fs\n",
				buf.tms_utime / clk_tck / 60,
				(double)(buf.tms_utime % (clk_tck * 60)) / clk_tck,
				buf.tms_stime / clk_tck / 60,
				(double)(buf.tms_stime % (clk_tck * 60)) / clk_tck,
				buf.tms_cutime / clk_tck / 60,
				(double)(buf.tms_cutime % (clk_tck * 60)) / clk_tck,
				buf.tms_cstime / clk_tck / 60,
				(double)(buf.tms_cstime % (clk_tck * 60)) / clk_tck);
			
			print(STDOUT_FILENO, output, P_RESET_PRINT);
		}

		return (free_options(result), ret);
	}

#pragma endregion

#pragma region "Info"

	// 0m0.010s 0m0.005s
	// 0m0.100s 0m0.050s

	// linea 1, tiempo 1: tms_utime  (user time del proceso actual)
	// linea 1, tiempo 2: tms_stime  (system time del proceso actual)
	// linea 2, tiempo 1: tms_cutime (user time acumulado de hijos)
	// linea 2, tiempo 2: tms_cstime (system time acumulado de hijos)

#pragma endregion
