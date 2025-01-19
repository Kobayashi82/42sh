/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:11:49 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/19 18:36:12 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins/builtins.h"
#include "hashes/alias.h"
#include "hashes/key_value.h"

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
			"      Returns success unless a NAME is supplied for which no alias has been defined.\n";

		print(STDOUT_FILENO, msg, RESET_PRINT);

		return (0);
	}

#pragma endregion

#pragma region Add

	static int add_alias(char *arg) {
		if (!arg) return (0);

		char *key = NULL, *value = NULL;
		get_key_value(arg, &key, &value, '=');
		if (alias_validate(key, true)) return (sfree(key), sfree(value), 1);
		alias_add(key, value);
		return (sfree(key), sfree(value), 0);
	}

#pragma endregion

#pragma region Print

	static int print_alias(char *arg, char **values, char **invalues) {
		if (!arg) return (0);

		t_alias *alias = alias_find(arg);
		if (alias && alias->name) {
			char *value = ft_strjoin_sep("alias ", alias->name, "='", 0);
			if (alias->value) value = ft_strjoin_sep(value, alias->value, "'\n", 1);
			if (value) *values = ft_strjoin(*values, value, 3);
		}

		if (!alias) {
			char *value = ft_strjoin_sep("alias: ", arg, ": not found\n", 0);
			if (value) *invalues = ft_strjoin(*invalues, value, 3);
		}

		return (0);
	}

#pragma endregion

#pragma region Alias

	int alias(t_arg *args) {
		t_opt *opts = parse_options(args, "p", '-', false);

		if (*opts->invalid) {
			invalid_option("alias", opts->invalid, "[-p] [name[=value] ... ]");
			return (sfree(opts), 1);
		}

		if (ft_strchr(opts->valid, '?')) return (sfree(opts), print_help());
		if (ft_strchr(opts->valid, '#')) return (sfree(opts), print_version("alias", "1.0"));

		int result = 0;
		char *values = NULL, *invalues = NULL;
		if (opts->args) {
			while (opts->args) {
				if (opts->args->value && opts->args->value[0] != '=' && ft_strchr(opts->args->value, '=')) {
					if (add_alias(opts->args->value)) result = 1;
				} else print_alias(opts->args->value, &values, &invalues);
				opts->args = opts->args->next;
			}
		} else alias_print(true);
		
		if (ft_strchr(opts->valid, 'p')) alias_print(true);

		if (values) { print(STDOUT_FILENO, values, RESET_PRINT); sfree(values); }
		if (invalues) { print(STDERR_FILENO, invalues, RESET_PRINT); sfree(invalues); }

		return (sfree(opts), result);
	}

#pragma endregion
