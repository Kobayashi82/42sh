/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enable.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 17:46:30 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/18 23:41:53 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

    //   -a        print a list of builtins showing whether or not each is enabled
    //   -n        disable each NAME or display a list of disabled builtins
    //   -p        print the list of builtins in a reusable format
    //   -s        print only the names of Posix `special' builtins

#pragma region "Includes"

	#include "utils/libft.h"
	#include "terminal/print.h"
	#include "parser/tokenizer/args.h"
	#include "builtins/builtins.h"
	#include "builtins/options.h"
	#include "hashes/builtin.h"

#pragma endregion

#pragma region "Help"

	static int print_help() {
		char *msg =
			"enable: enable [-a] [-nps] [name ...]\n"
			"    Enable and disable shell builtins.\n\n"

			"    Enables and disables builtin shell commands.\n"
			"    Disabling allows you to execute a disk command which has the same name as a\n"
			"    shell builtin without using a full pathname.\n\n"

			"    Options:\n"
			"      -a        print a list of builtins showing whether or not each is enabled\n"
			"      -n        disable each NAME or display a list of disabled builtins\n"
			"      -p        print the list of builtins\n"
			"      -s        print only the names of Posix `special' builtins\n\n"

			"    Without options, each NAME is enabled.\n\n"

			"    Exit Status:\n"
			"      Returns success unless NAME is not a shell builtin or an error occurs.\n";

		print(STDOUT_FILENO, msg, RESET_PRINT);

		return (0);
	}

#pragma endregion

#pragma region "Enable"

	int enable(t_arg *args) {
		t_opt *opts = parse_options(args, "anps", '-', false);

		if (*opts->invalid) {
			invalid_option("enable", opts->invalid, "[-a] [-nps] [name ...]");
			return (free(opts), 1);
		}

		if (strchr(opts->valid, '?')) return (free(opts), print_help());
		if (strchr(opts->valid, '#')) return (free(opts), print_version("enable", "1.0"));

		if (strchr(opts->valid, 'a')) {
			builtin_print(2, false, true);
			return (free(opts), 0);
		}

		if (strchr(opts->valid, 'n') && strchr(opts->valid, 's')) {
			builtin_print(1, true, true);
			return (free(opts), 0);
		}
		if (strchr(opts->valid, 's')) {
			builtin_print(2, true, true);
			return (free(opts), 0);
		}

		if ((strchr(opts->valid, 'n') && !opts->args) || (strchr(opts->valid, 'n') && strchr(opts->valid, 'p'))) {
			builtin_print(1, false, true);
			return (free(opts), 0);
		}

		if (strchr(opts->valid, 'p')) {
			builtin_print(0, false, true);
			return (free(opts), 0);
		}

		int result = 0;
		char *invalues = NULL;
		if (opts->args) {
			while (opts->args) {
				t_builtin *builtin = builtin_find(opts->args->value);
				if (builtin) {
					if (strchr(opts->valid, 'n'))	builtin->disabled = true;
					else 								builtin->disabled = false;
				} else {
					char *value = ft_strjoin_sep("enable: ", opts->args->value, ": not a shell builtin\n", 0);
					if (value) invalues = ft_strjoin(invalues, value, 3);
					result = 1;
				}
				opts->args = opts->args->next;
			}
		} else builtin_print(0, false, true);

		if (invalues) { print(STDERR_FILENO, invalues, RESET_PRINT); free(invalues); }

		return (free(opts), result);
	}

#pragma endregion
