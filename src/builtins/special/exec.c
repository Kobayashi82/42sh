/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:08:56 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/29 00:30:38 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "hashes/builtin.h"
	#include "builtins/options.h"
	#include "utils/libft.h"
	#include "utils/print.h"
	
	#include "tests/args.h"

#pragma endregion

#pragma region "Help"

	static int print_help() {
		char *msg =
		"exec: exec [-cl] [-a name] [command [argument ...]] [redirection ...]\n"
		"    Replace the shell with the given command.\n\n"

		"    Execute COMMAND, replacing this shell with the specified program.\n"
		"    ARGUMENTS become the arguments to COMMAND.  If COMMAND is not specified,\n"
		"    any redirections take effect in the current shell.\n\n"

		"    Options:\n"
		"      -a name	 pass NAME as the zeroth argument to COMMAND\n"
		"      -c	     execute COMMAND with an empty environment\n"
		"      -l	     place a dash in the zeroth argument to COMMAND\n\n"

		"    If the command cannot be executed, a non-interactive shell exits, unless\n"
		"    the shell option `execfail' is set.\n\n"

		"    Exit Status:\n"
		"      Returns success unless COMMAND is not found or a redirection error occurs.\n";

		print(STDOUT_FILENO, msg, RESET_PRINT);

		return (0);
	}

#pragma endregion

#pragma region "Exec"

	int bt_exec(t_arg *args) {
		t_opt *opts = parse_options(args, "acl", '-', 0);

		if (*opts->invalid) {
			invalid_option("exec", opts->invalid, "[-cl] [-a name] [command [argument ...]] [redirection ...]");
			return (free(opts), 1);
		}

		if (strchr(opts->valid, '?')) return (free(opts), print_help());
		if (strchr(opts->valid, '#')) return (free(opts), print_version("enable", "1.0"));

		if (strchr(opts->valid, 'c')) {
			return (free(opts), 0);
		}

		if (strchr(opts->valid, 'l') && strchr(opts->valid, 's')) {
			return (free(opts), 0);
		}
		if (strchr(opts->valid, 'a')) {
			return (free(opts), 0);
		}

		int result = 0;
		char *invalues = NULL;
		if (opts->args) {
			while (opts->args) {
				// t_builtin *builtin = builtin_find(opts->args->value);
				// if (builtin) {
				// 	if (strchr(opts->valid, 'n'))	builtin->disabled = 1;
				// 	else 								builtin->disabled = 0;
				// } else {
				// 	char *value = ft_strjoin_sep("enable: ", opts->args->value, ": not a shell builtin\n", 0);
				// 	if (value) invalues = ft_strjoin(invalues, value, 3);
				// 	result = 1;
				// }
				opts->args = opts->args->next;
			}
		} else ;

		if (invalues) { print(STDERR_FILENO, invalues, RESET_PRINT); free(invalues); }

		return (free(opts), result);
	}

#pragma endregion
