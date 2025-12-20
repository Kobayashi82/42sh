/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:09:10 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/20 12:47:13 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "utils/libft.h"
	#include "terminal/terminal.h"
	#include "utils/print.h"
	#include "tests/args.h"
	#include "builtins/builtins.h"
	#include "builtins/options.h"
	#include "main/shell.h"

	#include "parser/parser.h"

#pragma endregion


#pragma region "Help"

	static int print_help() {
		char *msg =
		"exit: exit [n]\n"
		"    Exit the shell.\n\n"

		"    Exits the shell with a status of N. If N is omitted, the exit status\n"
		"    is that of the last command executed.\n";

		print(STDOUT_FILENO, msg, RESET_PRINT);

		return (0);
	}

#pragma endregion

#pragma region "Exit"

	int bt_exit(t_arg *args) {
		t_opt *opts = parse_options(args->next, "", '-', 0);

		if (strchr(opts->valid, '?')) return (free(opts), print_help());
		if (strchr(opts->valid, '#')) return (free(opts), print_version("exit", "1.0"));

		int result = 0;

		if (opts->args && opts->args->next) {
			print(STDOUT_FILENO, "exit: too many arguments\n", RESET_PRINT);
			result = 1;
		} else if (opts->args && !ft_isdigit_s(opts->args->value)) {
			print(STDOUT_FILENO, "exit: numeric argument required\n", RESET_PRINT);
			result = 2;
		} else if (opts->args && opts->args->value)
			result = atol(opts->args->value);

		free(opts);
		args_clear(&args);
		exit_error(NOTHING, result, NULL, 1);

		return (result);
	}

#pragma endregion


