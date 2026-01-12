/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:12:03 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/12 12:31:08 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"
	
	#include "main/shell.h"
	#include "utils/utils.h"
	#include "utils/getopt.h"

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Help"

		int bt_hash_help(int format, int no_print) {
			char *name = "hash";
			char *syntax = "hash [-lr] [-p pathname] [-dt] [name ...]";
			char *description = "Remember or display program locations.";
			char *msg =
				"    Determine and remember the full pathname of each command NAME.\n"
				"    If no arguments are given, information about remembered commands is displayed.\n\n"

				"    Options:\n"
				"      -d               forget the remembered location of each NAME\n"
				"      -l               display in a format that may be reused as input\n"
				"      -p pathname      use PATHNAME as the full pathname of NAME\n"
				"      -r               forget all remembered locations\n"
				"      -t               print the remembered location of each NAME, preceding\n"
				"                       each location with the corresponding NAME if multiple\n"
				"                       NAMEs are given\n"
				"    Arguments:\n"
				"      NAME             Each NAME is searched for in $PATH and added to the list\n"
				"                       of remembered commands.\n\n"

				"    Exit Status:\n"
				"      Returns success unless NAME is not found or an invalid option is given.\n";

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
				"hash 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by "DEVELOPER" ("LOGIN42").\n";

			print(STDOUT_FILENO, msg, P_RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Print"

	static int print_hash(int reuse) {
		int title = 0; int total = 0;
		for (unsigned int index = 0; index < HASH_SIZE; ++index) {
			t_hash *hash = shell.hash_table[index];
			while (hash) {
				if (hash->name && hash->path) {
					if (reuse) {
						print(STDOUT_FILENO, "builtin hash -p ", P_JOIN);
						print(STDOUT_FILENO, hash->path, P_JOIN);
						print(STDOUT_FILENO, " ", P_JOIN);
						print(STDOUT_FILENO, hash->name, P_JOIN);
						print(STDOUT_FILENO, "\n", P_JOIN);
					} else {
						if (!title) { title = 1; print(STDOUT_FILENO, "hits    command\n", P_JOIN); }
						char *hits = ft_itoa(hash->hits);
						int spaces = 4 - ft_strlen(hits);
						while (spaces--) print(STDOUT_FILENO, " ", P_JOIN);
						print(STDOUT_FILENO, hits, P_FREE_JOIN);
						print(STDOUT_FILENO, "    ", P_JOIN);
						print(STDOUT_FILENO, hash->path, P_JOIN);
						print(STDOUT_FILENO, "\n", P_JOIN);
					}
					total++;
				}
				hash = hash->next;
			}
		}

		if (!total) { print(STDERR_FILENO, "hash: hash table empty\n", P_PRINT); return (1); }
		else print(STDOUT_FILENO, NULL, P_PRINT);

		return (0);
	}

#pragma endregion

#pragma region "Hash"

	int bt_hash(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, "lrp:dt", NULL, long_opts, "hash [-lr] [-p pathname] [-dt] [name ...]", IGNORE_OFF);
		if (!result)		return (1);
		if (result->error)	return (free_options(result), 2);

		if (find_long_option(result, "help"))		return (free_options(result), bt_hash_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());


		int ret = 0;

		print(STDOUT_FILENO, NULL, P_RESET);
		print(STDERR_FILENO, NULL, P_RESET);

		if (has_option(result, 'r')) {
			hash_clear();
			return (free_options(result), 0);
		}

		if (has_option(result, 't')) {
			// -t             print the remembered location of each NAME, preceding
            //                each location with the corresponding NAME if multiple
            //                NAMEs are given
			return (free_options(result), 0);
		}

		if (has_option(result, 'd') || has_option(result, 'p')) {
			if (!result->argc) {
				print(STDERR_FILENO, shell.name,                                                                    P_RESET);
				if		(has_option(result, 'd')) print(STDERR_FILENO, ": hash: -d: option requires an argument\n", P_JOIN);
				else if (has_option(result, 't')) print(STDERR_FILENO, ": hash: -t: option requires an argument\n", P_JOIN);
				print(STDERR_FILENO, "hash: usage: hash [-lr] [-p pathname] [-dt] [name ...]\n",                   P_PRINT);
				return (free_options(result), 2);
			}

			for (int i = 0; i < result->argc; ++i)  {
				// -d             forget the remembered location of each NAME
				// -p pathname    use PATHNAME as the full pathname of NAME
			}
			return (free_options(result), 0);
		}

		if (!result->options || has_option(result, 'l')) {
			ret = print_hash(has_option(result, 'l'));
			return (free_options(result), ret);
		}

		print(STDOUT_FILENO, NULL, P_PRINT);
		print(STDERR_FILENO, NULL, P_PRINT);

		return (free_options(result), 0);
	}

#pragma endregion
