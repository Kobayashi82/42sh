/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:11:19 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/11 12:16:03 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Help

	static int print_help() {
		char *msg =
			"unset: unset [-v] [name ...]\n"
			"    Unset values and attributes of shell variables.\n\n"

			"    For each NAME, remove the corresponding variable.\n\n"

			"    Options:\n"
			"      -v        treat each NAME as a shell variable\n"

			"    Without options, unset first tries to unset a variable.\n\n"

			"    Some variables cannot be unset; also see `readonly'.\n\n"

			"    Exit Status:\n"
			"      Returns success unless an invalid option is given or a NAME is read-only.\n";

		print(STDOUT_FILENO, msg, RESET_PRINT);

		return (0);
	}

#pragma endregion

#pragma region Delete

	static int delete_variable(char *arg, char **invalues) {
		if (!arg) return (0);
		int result = 0;

		t_var *var = variables_find(vars_table, arg);
		if (var && var->readonly) {
			char *value = ft_strjoin_sep(PROYECTNAME ": ", arg, ": readonly variable\n", 0);
			if (value) *invalues = ft_strjoin(*invalues, value, 3);
			result = 1;
		} else variables_delete(vars_table, arg);

		return (result);
	}

#pragma endregion

#pragma region Unset

	int unset(t_arg *args) {
		t_opt *opts = parse_options(args, "v", '-', false);

		if (*opts->invalid) {
			invalid_option("unset", opts->invalid, "[-v] [name ...]");
			return (sfree(opts), 1);
		}

		if (ft_strchr(opts->valid, '?')) return (sfree(opts), print_help());
		if (ft_strchr(opts->valid, '#')) return (sfree(opts), print_version("unset", "1.0"));

		int result = 0;
		char *invalues = NULL;
		while (opts->args) {
			if (delete_variable(opts->args->value, &invalues)) result = 1;
			opts->args = opts->args->next;
		}
		
		if (invalues) { print(STDERR_FILENO, invalues, RESET_PRINT); sfree(invalues); }

		return (sfree(opts), result);
	}

#pragma endregion
