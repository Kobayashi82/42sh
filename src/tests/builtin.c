/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 19:10:01 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/18 23:41:53 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "utils/libft.h"
	#include "terminal/print.h"
	#include "hashes/alias.h"
	#include "hashes/variables.h"
	#include "hashes/builtin.h"
	#include "hashes/cmdp.h"
	#include "parser/tokenizer/args.h"
	#include "builtins/builtins.h"
	#include "tests/tests.h"

	#include <stdio.h>

#pragma endregion

#pragma region "Alias"

	static int test_alias() {   
		int result = 0;

		t_arg *args = test_create_args("alias a=b b=c c=h");
		builtin_exec(args); test_free_args(args);
		if (alias_length() != 3) result = 1;
		
		args = test_create_args("alias a=d");
		builtin_exec(args); test_free_args(args);
		if (!result && strcmp(alias_find("a")->value, "d")) result = 1;

		alias_clear();

		return (result);
	}

#pragma endregion

#pragma region "Unalias"

	static int test_unalias() {   
		int result = 0;

		alias_add("a", "b");
		alias_add("b", "c");
		alias_add("c", "h");

		t_arg *args = test_create_args("unalias a");
		builtin_exec(args); test_free_args(args);
		if (alias_length() != 2) result = 1;
		
		args = test_create_args("unalias b");
		builtin_exec(args); test_free_args(args);
		if (alias_length() != 1) result = 1;

		args = test_create_args("unalias -a invalid");
		builtin_exec(args); test_free_args(args);
		if (alias_length() != 0) result = 1;

		alias_clear();

		return (result);
	}

#pragma endregion

#pragma region "Readonly"

	static int test_readonly() {
		int result = 0;

		t_arg *args = test_create_args("readonly -p test_fixed");
		builtin_exec(args); test_free_args(args);
		if (variables_length(vars_table, READONLY) != 1) result = 1;

		variables_clear(vars_table);

		return (result);
	}

#pragma endregion

#pragma region "Export"

	static int test_export() {
		int result = 0;

		t_arg *args = test_create_args("export a=b test");
		builtin_exec(args); test_free_args(args);
		if (variables_length(vars_table, EXPORTED_LIST) != 2) result = 1;
		
		args = test_create_args("export -n a=c test");
		builtin_exec(args); test_free_args(args);
		if (!result && variables_length(vars_table, EXPORTED_LIST) != 0) result = 1;

		args = test_create_args("export a+=k");
		builtin_exec(args); test_free_args(args);
		if (!result && variables_length(vars_table, EXPORTED_LIST) != 1) result = 1;
		if (!result && strcmp(variables_find_value(vars_table, "a"), "ck")) result = 1;

		variables_clear(vars_table);

		return (result);
	}

#pragma endregion

#pragma region "Unset"

	static int test_unset() {
		int result = 0;

		variables_add(vars_table, "a", "b", 1, 0, 0, 0);
		variables_add(vars_table, "test", NULL, 1, 0, 0, 0);

		t_arg *args = test_create_args("unset a=b test");
		builtin_exec(args); test_free_args(args);
		if (variables_length(vars_table, EXPORTED_LIST) != 1) result = 1;
		if (!result && variables_length(vars_table, INTERNAL) != 0) result = 1;
		
		variables_add(vars_table, "test", NULL, 1, 0, 0, 0);

		args = test_create_args("unset -v a test");
		builtin_exec(args); test_free_args(args);
		if (!result && variables_length(vars_table, EXPORTED_LIST) != 0) result = 1;
		if (!result && variables_length(vars_table, INTERNAL) != 0) result = 1;

		variables_clear(vars_table);

		return (result);
	}

#pragma endregion

#pragma region "Enable"

	static int test_enable() {
		int result = 0;

		t_arg *args = test_create_args("enable -n readonly");
		builtin_exec(args); test_free_args(args);
		if (!result && builtin_length(1, false) != 1) result = 1;

		args = test_create_args("enable readonly");
		builtin_exec(args); test_free_args(args);
		if (!result && builtin_length(1, false) != 0) result = 1;
		
		return (result);
	}

#pragma endregion

#pragma region "Type"

	static int test_type(const char **envp) {
		int result = 0;
		return (0);

		variables_from_array(vars_table, envp);

		t_arg *args = test_create_args("type -pP echo popo date");
		builtin_exec(args); test_free_args(args);

		alias_clear();
		cmdp_clear();

		return (result);
	}

#pragma endregion

#pragma region "Command"

	static int test_command(const char **envp) {
		int result = 0;
		return (0);

		variables_from_array(vars_table, envp);

		t_arg *args = test_create_args("command -v echo popo date");
		builtin_exec(args); test_free_args(args);

		alias_clear();
		cmdp_clear();

		return (result);
	}

#pragma endregion

#pragma region "Hash"

	static int test_hash(const char **envp) {
		int result = 0;
		return (0);
		variables_from_array(vars_table, envp);
		printf("\n");
		cmdp_add("./42/pipi", false, false);
		cmdp_add("/usr/bin/date", false, false);

		t_arg *args = test_create_args("hash -t");
		builtin_exec(args); test_free_args(args);

		cmdp_clear();

		return (result);
	}

#pragma endregion

#pragma region "Builtins"

	int test_builtins(const char **envp) {
		printf(W"\t────────────────────────\n"NC);
		printf(C"\tBuiltin     "); fflush(stdout);
		int result = 0;

		if (!result && test_alias())		{ result = 1; printf(RD"X"RED500" alias\n"NC);		}
		if (!result && test_unalias())		{ result = 1; printf(RD"X"RED500" unalias\n"NC);	}
		if (!result && test_readonly())		{ result = 1; printf(RD"X"RED500" readonly\n"NC);	}
		if (!result && test_export())		{ result = 1; printf(RD"X"RED500" export\n"NC);		}
		if (!result && test_unset())		{ result = 1; printf(RD"X"RED500" unset\n"NC);		}
		if (!result && test_enable())		{ result = 1; printf(RD"X"RED500" enable\n"NC);		}
		if (!result && test_type(envp))		{ result = 1; printf(RD"X"RED500" type\n"NC);		}
		if (!result && test_command(envp))	{ result = 1; printf(RD"X"RED500" command\n"NC);	}
		if (!result && test_hash(envp))		{ result = 1; printf(RD"X"RED500" hash\n"NC);		}

		if (!result) printf(G"✓"GREEN500" passed\n"NC);

		return (result);
	}

#pragma endregion
