/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:12:03 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/18 22:36:07 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"
	#include "terminal/print.h"
	#include "parser/tokenizer/args.h"
	#include "builtins/builtins.h"
	#include "builtins/options.h"
	#include "hashes/cmdp.h"

#pragma endregion

#pragma region "Help"

	static int print_help() {
		char *msg =
			"hash: hash [-lr] [-p pathname] [-dt] [name ...]\n"
			"    Remember or display program locations.\n\n"

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

		print(STDOUT_FILENO, msg, RESET_PRINT);

		return (0);
	}

#pragma endregion

#pragma region "Print"

	static int print_hash(bool reuse) {
		bool title = false; int total = 0;
		for (unsigned int index = 0; index < CMDP_HASH_SIZE; ++index) {
			t_cmdp *cmdp = cmdp_table[index];
			while (cmdp) {
				if (cmdp->name && cmdp->path) {
					if (reuse) {
						print(STDOUT_FILENO, "builtin hash -p ", JOIN);
						print(STDOUT_FILENO, cmdp->path, JOIN);
						print(STDOUT_FILENO, " ", JOIN);
						print(STDOUT_FILENO, cmdp->name, JOIN);
						print(STDOUT_FILENO, "\n", JOIN);
					} else {
						if (!title) { title = true; print(STDOUT_FILENO, "hits    command\n", JOIN); }
						char *hits = ft_itoa(cmdp->hits);
						int spaces = 4 - ft_strlen(hits);
						while (spaces--) print(STDOUT_FILENO, " ", JOIN);
						print(STDOUT_FILENO, hits, FREE_JOIN);
						print(STDOUT_FILENO, "    ", JOIN);
						print(STDOUT_FILENO, cmdp->path, JOIN);
						print(STDOUT_FILENO, "\n", JOIN);
					}
					total++;
				}
				cmdp = cmdp->next;
			}
		}
		
		if (!total) { print(STDERR_FILENO, "hash: hash table empty\n", PRINT); return (1); }
		else print(STDOUT_FILENO, NULL, PRINT);

		return (0);
	}

#pragma endregion

#pragma region "Hash"

	int hash(t_arg *args) {
		t_opt *opts = parse_options(args, "dlprt", '-', false);

		if (*opts->invalid) {
			invalid_option("hash", opts->invalid, "[-lr] [-p pathname] [-dt] [name ...]");
			return (free(opts), 1);
		}

		if (ft_strchr(opts->valid, '?')) return (free(opts), print_help());
		if (ft_strchr(opts->valid, '#')) return (free(opts), print_version("hash", "1.0"));

		print(STDOUT_FILENO, NULL, RESET);
		print(STDERR_FILENO, NULL, RESET);
		
		if ((!*opts->valid || !strcmp(opts->valid, "l")) && !opts->args) {
			int result = print_hash((ft_strchr(opts->valid, 'l')));
			return (free(opts), result);
		}

		if ((ft_strchr(opts->valid, 't') || ft_strchr(opts->valid, 'p') || ft_strchr(opts->valid, 'd')) && !opts->args) {
			char opt[4]; int i = 0;
			if (ft_strchr(opts->valid, 't')) opt[i++] = 't';
			if (ft_strchr(opts->valid, 'p')) opt[i++] = 'p';
			if (ft_strchr(opts->valid, 'd')) opt[i++] = 'd';
			opt[i] = '\0';
			print(STDERR_FILENO, ft_strjoin_sep("hash: -", opt, ": option requires an argument\n", 0), FREE_JOIN);
			print(STDERR_FILENO, "usage: hash [-lr] [-p pathname] [-dt] [name ...]\n\n", JOIN);
			print(STDERR_FILENO, "Try 'hash --help' for more information\n", PRINT);
			return (free(opts), 1);
		}

		print(STDOUT_FILENO, NULL, PRINT);
		print(STDERR_FILENO, NULL, PRINT);

		return (free(opts), 0);
	}

#pragma endregion
