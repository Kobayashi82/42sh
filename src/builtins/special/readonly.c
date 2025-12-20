/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readonly.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:06:39 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/20 21:13:32 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "utils/libft.h"
	#include "utils/print.h"
	#include "tests/args.h"
	#include "builtins/builtins.h"
	#include "builtins/options.h"
	#include "hashes/variable.h"
	#include "main/shell.h"

#pragma endregion

#pragma region "Help"

	static int print_help() {
		char *msg =
			"    readonly: readonly [name[=value] ...] or readonly -p\n"
			"    Mark shell variables as unchangeable.\n\n"

			"    Mark each NAME as read-only; the values of these NAMEs may not be\n"
			"    changed by subsequent assignment.  If VALUE is supplied, assign VALUE\n"
			"    before marking as read-only.\n\n"

			"    Options:\n"
			"      -p        display a list of all readonly variables.\n\n"

			"    Exit Status:\n"
			"      Returns success unless an invalid option is given or NAME is invalid.\n";

		print(STDOUT_FILENO, msg, RESET_PRINT);

		return (0);
	}

#pragma endregion

#pragma region "Add"

	static int add_readonly(char *arg) {
		if (!arg) return (0);
		int result = 0;

		if (!strchr(arg, '=')) {
			if (variables_validate(arg, NULL, "readonly", 0, 1)) return (1);
			t_var *var = variables_find(vars_table, arg);
			if (var) { var->readonly = 1; return (0); }
		}

		char *key = NULL, *value = NULL;
		get_key_value(arg, &key, &value, '=');

		int len = ft_strlen(key);
		int concatenate = 0;
		if (key && len > 0 && key[len - 1] == '+') { key[len - 1] = '\0'; concatenate = 1; }
		if (variables_validate(key, value, "readonly", 1, 1)) return (free(key), free(value), 1);

		t_var *var = variables_find(vars_table, key);
		if (var && var->readonly) {
			print(STDOUT_FILENO, NULL, RESET);
			print(STDERR_FILENO, ft_strjoin_sep(PROYECTNAME ": ", key, ": readonly variable\n", 0), FREE_PRINT);
			result = 1;
		} else {
			if (concatenate && variables_concatenate(vars_table, key, value, -1, 1, -1, -1))	result = 1;
			if (!concatenate && variables_add(vars_table, key, value, -1, 1, -1, -1))			result = 1;
		}

		return (free(key), free(value), result);
	}

#pragma endregion

#pragma region "Readonly"

	int bt_readonly(t_arg *args) {
		t_opt *opts = parse_options(args, "p", '-', 0);

		if (*opts->invalid) {
			invalid_option("readonly", opts->invalid, "[name[=value] ...] or readonly -p");
			return (free(opts), 1);
		}

		if (strchr(opts->valid, '?')) return (free(opts), print_help());
		if (strchr(opts->valid, '#')) return (free(opts), print_version("readonly", "1.0"));


		if (!opts->args) {
			variables_print(vars_table, READONLY, 1);
			return (free(opts), 0);
		}

		int result = 0;
		while (opts->args) {
			if (add_readonly(opts->args->value)) result = 1;
			opts->args = opts->args->next;
		}

		return (free(opts), result);
	}

#pragma endregion
