/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   declare.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:06:19 by vzurera-          #+#    #+#             */
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

//	if -i atoi to de operands
// declare patata		crea la variable sin atributos (si ya existe, no le hace cambios)
// declare -irx patata	crea o añade la variable con los atributos
// declare -pirx patata	ignora atributos y muestra patata si existe (prevalece -p)
// declare -pirx		muestra las variables que cumplan con los atributos indicados
// declare -irx			muestra las variables que cumplan con los atributos indicados
// declare -p			-p es opcional, muestra todas las variables
// declare +irx patata	quita los atributos a patata

#pragma region "Help"

	static int print_help() {
		char *msg =
			"declare: declare [-ilrux] [name[=value] ...] or declare -p [-ilrux] [name ...]\n"
			"    Set variable values and attributes.\n\n"

			"    Declare variables and give them attributes.  If no NAMEs are given,\n"
			"    display the attributes and values of all variables.\n\n"

			"    Options:\n"
			"      -p        display the attributes and value of each NAME\n\n"

			"    Options which set attributes:\n"
			"      -i        to make NAMEs have the `integer' attribute\n"
			"      -l        to convert the value of each NAME to lower case on assignment\n"
			"      -r        to make NAMEs readonly\n"
			"      -u        to convert the value of each NAME to upper case on assignment\n"
			"      -x        to make NAMEs export\n\n"

			"    Using `+' instead of `-' turns off the given attribute.\n\n"

			"    Variables with the integer attribute have arithmetic evaluation\n"
			"    performed when the variable is assigned a value.\n\n"

			"    Exit Status:\n"
			"      Returns success unless an invalid option is supplied or a variable assignment\n"
			"      error occurs.\n";

		print(STDOUT_FILENO, msg, RESET_PRINT);

		return (0);
	}

#pragma endregion

#pragma region "Add"

	static int add_declare(char *arg) {
		if (!arg) return (0);
		int result = 0;

		if (!strchr(arg, '=')) {
			if (variables_validate(arg, NULL, "declare", 0, 1)) return (1);
			t_var *var = variables_find(vars_table, arg);
			if (var) { var->exported = 1; return (0); }
		}

		char *key = NULL, *value = NULL;
		get_key_value(arg, &key, &value, '=');

		int len = ft_strlen(key);
		int concatenate = 0;
		if (key && len > 0 && key[len - 1] == '+') { key[len - 1] = '\0'; concatenate = 1; }
		if (variables_validate(key, value, "declare", 1, 1)) return (free(key), free(value), 1);

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

#pragma region "Delete"

	static int delete_declare(char *arg) {
		if (!arg) return (0);
		int result = 0;

		if (!strchr(arg, '=')) {
			if (variables_validate(arg, NULL, "declare", 0, 1)) return (1);
			t_var *var = variables_find(vars_table, arg);
			if (var) var->exported = 0;
			return (0);
		}

		char *key = NULL, *value = NULL;
		get_key_value(arg, &key, &value, '=');

		int len = ft_strlen(key);
		int concatenate = 0;
		if (key && len > 0 && key[len - 1] == '+') { key[len - 1] = '\0'; concatenate = 1; }

		if (variables_validate(key, value, "declare", 1, 1)) return (free(key), free(value), 1);
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

#pragma region "Declare"

	int bt_declare(t_arg *args) {
		t_opt *opts = parse_options(args, "pilrux", '-', 0);
		t_opt *opts_plus = parse_options(args, "ilrux", '+', 0);

		(void) opts_plus;

		if (*opts->invalid) {
			invalid_option("declare", opts->invalid, "[-ilrux] [name[=value] ...] or declare -p [-ilrux] [name ...]");
			return (free(opts), 1);
		}

		if (strchr(opts->valid, '?')) return (free(opts), print_help());
		if (strchr(opts->valid, '#')) return (free(opts), print_version("declare", "1.0"));

		if (!opts->args) {
			variables_print(vars_table, EXPORTED_LIST, 1);
			return (free(opts), 1);
		}

		int result = 0;
		while (opts->args) {
			if (strchr(opts->valid, 'n')) { if (delete_declare(opts->args->value)) result = 1; }
			else if (add_declare(opts->args->value)) result = 1;
			opts->args = opts->args->next;
		}

		return (free(opts), result);
	}

#pragma endregion
