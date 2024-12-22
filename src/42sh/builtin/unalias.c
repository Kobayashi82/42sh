/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unalias.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 21:38:29 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/22 20:16:07 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Help

	static int print_help() {
		char *msg =
			"unalias: unalias [-a] name [name ...]\n"
			"    Remove each NAME from the list of defined aliases.\n\n"

			"    Options:\n"
			"      -a        remove all alias definitions\n\n"

			"    Exit Status:\n"
			"      Return success unless a NAME is not an existing alias.\n";

		print(STDOUT_FILENO, msg, RESET_PRINT);

		return (0);
	}

#pragma endregion

#pragma region Delete

	static int delete_alias(char *arg, char **invalues) {
		if (!arg) return (0);

		t_alias *alias = alias_find(arg);
		if (alias && alias->name) alias_delete(arg);

		if (!alias) {
			char *value = ft_strjoin_sep("unalias: ", arg, ": not found\n", 0);
			if (value) *invalues = ft_strjoin(*invalues, value, 3);
			return (1);
		}

		return (0);
	}

#pragma endregion

#pragma region Unalias

	int unalias(t_arg *args) {
		t_opt *opts = parse_options(args, "a", '-', false);

		if (*opts->invalid) {
			invalid_option("unalias", opts->invalid, "[-a] name [name ...]");
			return (free(opts), 1);
		}

		if (ft_strchr(opts->valid, 'H')) return (free(opts), print_help());
		if (ft_strchr(opts->valid, 'V')) return (free(opts), print_version("unalias", "1.0"));
		if (ft_strchr(opts->valid, 'a')) return (free(opts), alias_clear(), 0);

		int result = 0;
		char *invalues = NULL;
		while (opts->args) {
			if (delete_alias(opts->args->value, &invalues)) result = 1;
			opts->args = opts->args->next;
		}
		
		if (invalues) { print(STDERR_FILENO, invalues, RESET_PRINT); free(invalues); }

		return (free(opts), result);
	}

#pragma endregion
