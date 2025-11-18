/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:09:33 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/18 23:41:53 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "utils/libft.h"
	#include "terminal/print.h"
	#include "parser/tokenizer/args.h"
	#include "builtins/builtins.h"
	#include "builtins/options.h"
	#include "main/shell.h"
	#include "utils/paths.h"

#pragma endregion

#pragma region "Help"

	static int print_help() {
		char *msg =
			"pwd: pwd [-LP]\n"
			"    Print the name of the current working directory.\n\n"

			"    Options:\n"
			"      -L        print the value of $PWD if it names the current working\n"
			"                directory\n"
			"      -P        print the physical directory, without any symbolic links\n\n"

			"    By default, `pwd' behaves as if `-L' were specified.\n\n"

			"    Exit Status:\n"
			"      Returns 0 unless an invalid option is given or the current directory\n"
			"      cannot be read.\n";

		print(STDOUT_FILENO, msg, RESET_PRINT);

		return (0);
	}

#pragma endregion

#pragma region "PWD"

	int pwd(t_arg *args) {
		t_opt *opts = parse_options(args, "LP", '-', false);

		if (*opts->invalid) {
			invalid_option("pwd", opts->invalid, "[-LP]");
			return (free(opts), 1);
		}

		if (strchr(opts->valid, '?')) return (free(opts), print_help());
		if (strchr(opts->valid, '#')) return (free(opts), print_version("pwd", "1.0"));

		int result = 0;

		if (strchr(opts->valid, 'P')) {
			char *cwd = get_cwd("cwd");
			if (!cwd) 		result = 1;
			else {
				print(STDOUT_FILENO, cwd, RESET);
				print(STDOUT_FILENO, "\n", PRINT);
				free(cwd);
			}
		} else {
			if (shell.cwd) {
				print(STDOUT_FILENO, shell.cwd, RESET);
				print(STDOUT_FILENO, "\n", PRINT);
			} else {
				print(STDERR_FILENO, "pwd: no se ha encontrado nada\n", RESET_PRINT);
				result = 1;
			}
		}

		return (free(opts), result);
	}

#pragma endregion
