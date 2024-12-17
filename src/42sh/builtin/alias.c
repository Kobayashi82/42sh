/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:11:49 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/17 23:47:56 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Help

	static int print_help() {
		char *msg =
			"alias: alias [-p] [name[=value] ... ]\n"
			"    Define or display aliases.\n\n"

			"    Without arguments, `alias' prints the list of aliases in the reusable\n"
			"    form `alias NAME=VALUE' on standard output.\n\n"

			"    Otherwise, an alias is defined for each NAME whose VALUE is given.\n"
			"    A trailing space in VALUE causes the next word to be checked for\n"
			"    alias substitution when the alias is expanded.\n\n"

			"    Options:\n"
			"      -p        print all defined aliases in a reusable format\n\n"

			"    Exit Status:\n"
			"    alias returns true unless a NAME is supplied for which no alias has been defined.\n";

		print(STDOUT_FILENO, msg, RESET_PRINT);

		return (0);
	}

#pragma endregion

#pragma region Add

	static void add_alias(char *arg) {
		if (!arg) return;

		char *key = NULL, *value = NULL;
		get_key_value(arg, &key, &value, '=');
		alias_add(key, value);
		free(key); free(value);
	}

#pragma endregion

#pragma region Print

	static char *print_alias(char *arg, char *values) {
		if (!arg) return (values);

		t_alias *alias = alias_find(arg);
		if (alias && alias->name) {
			char *value = ft_strjoin_sep("alias: ", alias->name, "='", 0);
			if (alias->value) value = ft_strjoin_sep(value, alias->value, "'\n", 1);
			if (value) values = ft_strjoin(values, value, 3);
		}

		if (!alias) {
			char *value = ft_strjoin_sep("alias: ", arg, ": not found\n", 0);
			if (value) values = ft_strjoin(values, value, 3);
		}

		return (values);
	}

#pragma endregion

#pragma region Alias

	int alias(t_arg *args) {
		t_opt *opts = parse_options(args, "p", false);

		if (ft_strchr(opts->valid, 'H')) return (free(opts), print_help());
		if (ft_strchr(opts->valid, 'V')) return (free(opts), print_version("alias", "1.0"));

		if (*opts->invalid) {
			invalid_option("alias", opts->invalid, "[-p] [name[=value] ... ]");
			return (free(opts), 1);
		}

		char *values = NULL;
		while (opts->args) {
			if (ft_strchr(opts->args->value, '='))	add_alias(opts->args->value);
			else									values = print_alias(opts->args->value, values);
			opts->args = opts->args->next;
		}

		if (!opts->args || ft_strchr(opts->valid, 'p')) alias_print(true);
		if (values) { print(STDOUT_FILENO, values, RESET_PRINT); free(values); }

		return (free(opts), 0);
	}

#pragma endregion
