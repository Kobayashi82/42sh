/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:06:34 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/24 14:22:40 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Help

	static int print_help() {
		char *msg =
			"export: export [-n] [name[=value] ...] or export -p\n"
			"    Set export attribute for shell variables.\n\n"

			"    Marks each NAME for automatic export to the environment of subsequently\n"
			"    executed commands.  If VALUE is supplied, assign VALUE before exporting.\n\n"

			"    Options:\n"
			"      -n        remove the export property from each NAME\n"
			"      -p        display a list of all exported variables and functions\n\n"

			"    Exit Status:\n"
			"      Returns success unless an invalid option is given or NAME is invalid.\n";

		print(STDOUT_FILENO, msg, RESET_PRINT);

		return (0);
	}

#pragma endregion

#pragma region Add

	static int add_export(char *arg) {
		if (!arg) return (0);
		int result = 0;

		if (!ft_strchr(arg, '=')) {
			if (variables_validate(arg, NULL, "export", false, true)) return (1);
			t_var *var = variables_find(vars_table, arg);
			if (var) { var->exported = true; return (0); }
		}

		char *key = NULL, *value = NULL;
		get_key_value(arg, &key, &value, '=');

		int len = ft_strlen(key);
		bool concatenate = false;
		if (key && len > 0 && key[len - 1] == '+') { key[len - 1] = '\0'; concatenate = true; }
		if (variables_validate(key, value, "export", true, true)) return (free(key), free(value), 1);

		t_var *var = variables_find(vars_table, key);
		if (var && var->readonly) {
			print(STDOUT_FILENO, NULL, RESET);
			print(STDERR_FILENO, ft_strjoin_sep(PROYECTNAME ": ", key, ": readonly variable\n", 0), FREE_PRINT);
			result = 1;
		} else {
			if (concatenate && variables_concatenate(vars_table, key, value, 1, -1, -1, -1))	result = 1;
			if (!concatenate && variables_add(vars_table, key, value, 1, -1, -1, -1))			result = 1;
		}

		return (free(key), free(value), result);
	}

#pragma endregion

#pragma region Delete

	static int delete_export(char *arg) {
		if (!arg) return (0);
		int result = 0;

		if (!ft_strchr(arg, '=')) {
			if (variables_validate(arg, NULL, "export", false, true)) return (1);
			t_var *var = variables_find(vars_table, arg);
			if (var) var->exported = false;
			return (0);
		}

		char *key = NULL, *value = NULL;
		get_key_value(arg, &key, &value, '=');

		int len = ft_strlen(key);
		bool concatenate = false;
		if (key && len > 0 && key[len - 1] == '+') { key[len - 1] = '\0'; concatenate = true; }

		if (variables_validate(key, value, "export", true, true)) return (free(key), free(value), 1);
		t_var *var = variables_find(vars_table, key);
		if (var && var->readonly) {
			print(STDOUT_FILENO, NULL, RESET);
			print(STDERR_FILENO, ft_strjoin_sep(PROYECTNAME ": ", key, ": readonly variable\n", 0), FREE_PRINT);
			result = 1;
		} else {
			if (concatenate && variables_concatenate(vars_table, key, value, 0, -1, -1, -1))	result = 1;
			if (!concatenate && variables_add(vars_table, key, value, 0, -1, -1, -1))			result = 1;
		}
		
		return (free(key), free(value), result);
	}

#pragma endregion

#pragma region Export

	int export(t_arg *args) {
		t_opt *opts = parse_options(args, "np", '-', false);

		if (*opts->invalid) {
			invalid_option("export", opts->invalid, "[-n] [name[=value] ...] or export -p");
			return (free(opts), 1);
		}

		if (ft_strchr(opts->valid, '?')) return (free(opts), print_help());
		if (ft_strchr(opts->valid, '#')) return (free(opts), print_version("export", "1.0"));

		if (!opts->args) {
			variables_print(vars_table, EXPORTED_LIST, true);
			return (free(opts), 0);
		}

		int result = 0;
		while (opts->args) {
			if (ft_strchr(opts->valid, 'n')) { if (delete_export(opts->args->value)) result = 1; }
			else if (add_export(opts->args->value)) result = 1;
			opts->args = opts->args->next;
		}

		return (free(opts), result);
	}

#pragma endregion
