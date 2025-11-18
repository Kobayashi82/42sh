/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:09:10 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/18 11:21:08 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"
	#include "terminal/terminal.h"
	#include "terminal/print.h"
	#include "parser/tokenizer/args.h"
	#include "builtins/builtins.h"
	#include "builtins/options.h"
	#include "main/shell.h"
	#include "main/error.h"

#pragma endregion


#pragma region "Help"

	static int print_help() {
		char *msg =
		"exit: exit [n]\n"
		"    Exit the shell.\n\n"

		"    Exits the shell with a status of N.  If N is omitted, the exit status\n"
		"    is that of the last command executed.\n";

		print(STDOUT_FILENO, msg, RESET_PRINT);

		return (0);
	}

#pragma endregion

#pragma region "Builtin"

	int bt_exit(t_arg *args) {
		t_opt *opts = parse_options(args->next, "", '-', false);

		if (ft_strchr(opts->valid, '?')) return (free(opts), print_help());
		if (ft_strchr(opts->valid, '#')) return (free(opts), print_version("builtin", "1.0"));

		int result = 0;

		if (opts->args && opts->args->next) {
			print(STDOUT_FILENO, "exit: too many arguments", RESET_PRINT);
			result = 1;
		} else if (opts->args && !ft_isdigit_s(opts->args->value)) {
			print(STDOUT_FILENO, "exit: numeric argument required", RESET_PRINT);
			result = 2;
		} else if (opts->args && opts->args->value)
			result = ft_atol(opts->args->value);
		
		free(opts);
		args_clear(&args);
		free(terminal.input);
		exit_error(NOTHING, result, NULL, true);

		return (result);
	}

#pragma endregion


