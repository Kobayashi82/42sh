/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 11:01:35 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/18 23:03:39 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"
	#include "terminal/print.h"
	#include "parser/tokenizer/args.h"
	#include "builtins/builtins.h"
	#include "builtins/options.h"
	#include "hashes/builtin.h"

#pragma endregion

#pragma region "Help"

	static int print_help() {
		char *msg =
		"builtin: builtin [shell-builtin [arg ...]]\n"
		"    Execute shell builtins.\n\n"

		"    Execute SHELL-BUILTIN with arguments ARGs without performing command\n"
		"    lookup.  This is useful when you wish to reimplement a shell builtin\n"
		"    as a shell function, but need to execute the builtin within the function.\n\n"

		"    Exit Status:\n"
		"      Returns the exit status of SHELL-BUILTIN, or false if SHELL-BUILTIN is\n"
		"      not a shell builtin.\n";

		print(STDOUT_FILENO, msg, RESET_PRINT);

		return (0);
	}

#pragma endregion

#pragma region "Builtin"

	int bt_builtin(t_arg *args) {
		t_opt *opts = parse_options(args, "", '-', false);

		if (*opts->invalid) {
			invalid_option("builtin", opts->invalid, "[shell-builtin [arg ...]]");
			return (free(opts), 1);
		}

		if (strchr(opts->valid, '?')) return (free(opts), print_help());
		if (strchr(opts->valid, '#')) return (free(opts), print_version("builtin", "1.0"));

		int result = 0;
		if (opts->args) {
			t_builtin *builtin = builtin_find(opts->args->value);
			if (builtin && !builtin->disabled) {
				result = builtin_exec(opts->args);
			} else {
				print(STDERR_FILENO, ft_strjoin_sep("builtin: ", opts->args->value, ": not a shell builtin\n", 0), FREE_RESET_PRINT);
				result = 1;
			}
		}

		return (free(opts), result);
	}

#pragma endregion
