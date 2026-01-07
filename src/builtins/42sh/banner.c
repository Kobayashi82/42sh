/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   banner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 14:08:19 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/07 16:32:27 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "hashes/builtin.h"
	#include "utils/libft.h"
	#include "utils/print.h"
	#include "utils/getopt.h"

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Help"

		int bt_banner_help(int format, int no_print) {
			char *name = "banner";
			char *syntax = "banner";
			char *description = "Display 42sh banner.";
			char *msg = "    Displays the 42sh banner in ASCII art\n";

			if (!no_print) print(STDOUT_FILENO, NULL, RESET);

			if (format == HELP_SYNTAX) {
				print(STDOUT_FILENO, ft_strjoin(name, ": ", 0),   FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin(syntax, "\n", 0), FREE_JOIN);
			}

			if (format == HELP_DESCRIPTION) {
				print(STDOUT_FILENO, ft_strjoin(name, " - ", 0),       FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin(description, "\n", 0), FREE_JOIN);
			}

			if (format == HELP_NORMAL) {
				print(STDOUT_FILENO, ft_strjoin(name, ": ", 0),                      FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin(syntax, "\n", 0),                    FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin_sep("    ", description, "\n\n", 0), FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin(msg, "\n", 0),                       FREE_JOIN);
			}

			if (format == HELP_MANPAGE) {
				print(STDOUT_FILENO, "NAME\n",                                       JOIN);
				print(STDOUT_FILENO, ft_strjoin_sep("    ", name, " - ", 0),         FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin(description, "\n\n", 0),             FREE_JOIN);
				print(STDOUT_FILENO, "SYNOPSYS\n",                                   JOIN);
				print(STDOUT_FILENO, ft_strjoin_sep("    ", syntax, "\n\n", 0),      FREE_JOIN);
				print(STDOUT_FILENO, "DESCRIPTION\n",                                JOIN);
				print(STDOUT_FILENO, ft_strjoin_sep("    ", description, "\n\n", 0), FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin(msg, "\n\n", 0),                     FREE_JOIN);
				print(STDOUT_FILENO, "SEE ALSO\n    42sh(1)\n\n",                    JOIN);
			}

			if (!no_print) print(STDOUT_FILENO, NULL, PRINT);

			return (0);
		}

	#pragma endregion

	#pragma region "Version"

		static int version() {
			char *msg =
				"banner 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by Kobayashi82 (vzurera-).\n";

			print(STDOUT_FILENO, msg, RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Banner"

	int bt_banner(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, NULL, NULL, long_opts, "banner", IGNORE_OFF);
		if (!result)		return (1);
		if (result->error)	return (free_options(result), 2);

		if (find_long_option(result, "help"))		return (free_options(result), bt_banner_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());

		print(STDOUT_FILENO, RED600"\n\t        ,--,                                        \n",   RESET);
		print(STDOUT_FILENO,         "\t      ,--.'|      ,----,  "Y"           ,---,     \n",     JOIN);
		print(STDOUT_FILENO,         "\t   ,--,  | :    .'   .' \\"Y"          ,--.' |     \n",    JOIN);
		print(STDOUT_FILENO,         "\t,---.'|  : '  ,----,'    |"Y"         |  |  :     \n",     JOIN);
		print(STDOUT_FILENO,         "\t;   : |  | ;  |    :  .  ;"Y".--.--.  :  :  :     \n",     JOIN);
		print(STDOUT_FILENO,         "\t|   | : _' |  ;    |.'  /"Y"/  /    ' :  |  |,--. \n",     JOIN);
		print(STDOUT_FILENO,         "\t:   : |.'  |  `----'/  ;"Y"|  :  /`./ |  :  '   | \n",     JOIN);
		print(STDOUT_FILENO,         "\t|   ' '  ; :    /  ;  / "Y"|  :  ;_   |  |   /' : \n",     JOIN);
		print(STDOUT_FILENO,         "\t\\   \\  .'. |   ;  /  /-,"Y" \\  \\    `.'  :  | | | \n", JOIN);
		print(STDOUT_FILENO,         "\t `---`:  | '  /  /  /.`|"Y"  `----.   \\  |  ' | : \n",    JOIN);
		print(STDOUT_FILENO,         "\t      '  ; |./__;      :"Y" /  /`--'  /  :  :_:,' \n",     JOIN);
		print(STDOUT_FILENO,         "\t      |  : ;|   :    .' "Y"'--'.     /|  | ,'     \n",     JOIN);
		print(STDOUT_FILENO,         "\t      '  ,/ ;   | .'     "Y" `--'---' `--''       \n",     JOIN);
		print(STDOUT_FILENO,         "\t      '--'  `---'                                \n\n"NC,  PRINT);

		return (free_options(result), 0);
	}

#pragma endregion
