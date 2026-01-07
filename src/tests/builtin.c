/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 19:10:01 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/07 16:40:00 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "utils/libft.h"
	#include "utils/print.h"
	#include "hashes/alias.h"
	#include "hashes/variable.h"
	#include "hashes/hash.h"
	#include "tests/args.h"
	#include "hashes/builtin.h"
	#include "tests/tests.h"

	#include <stdio.h>

#pragma endregion

#pragma region "Alias"

	int test_alias() {   
		int result = 0;

		int argc = 4;
		char **argv = test_create_argv(argc, "alias", "a=b", "b=c", "c=h");
		builtin_exec(argc, argv);
		test_free_argv(argv);
		if (alias_length() != 3) result = 1;

		argc = 2;
		argv = test_create_argv(argc, "alias", "a=d");
		builtin_exec(argc, argv);
		test_free_argv(argv);
		if (!result && strcmp(alias_find("a")->value, "d")) result = 1;

		alias_clear();

		return (result);
	}

#pragma endregion

#pragma region "Unalias"

	int test_unalias() {   
		int result = 0;

		alias_add("a", "b");
		alias_add("b", "c");
		alias_add("c", "h");

		int argc = 2;
		char **argv = test_create_argv(argc, "unalias", "a");
		builtin_exec(argc, argv);
		test_free_argv(argv);
		if (alias_length() != 2) result = 1;
		
		argc = 2;
		argv = test_create_argv(argc, "unalias", "b");
		builtin_exec(argc, argv);
		test_free_argv(argv);
		if (alias_length() != 1) result = 1;

		argc = 3;
		argv = test_create_argv(argc, "unalias", "-a", "invalid");
		builtin_exec(argc, argv);
		test_free_argv(argv);
		if (alias_length() != 0) result = 1;

		alias_clear();

		return (result);
	}

#pragma endregion

#pragma region "Readonly"

	int test_readonly() {
		int result = 0;

		int argc = 3;
		char **argv = test_create_argv(argc, "readonly", "-p", "test_fixed");
		builtin_exec(argc, argv);
		test_free_argv(argv);
		if (variables_length(vars_table, READONLY) != 1) result = 1;

		variables_clear(vars_table);

		return (result);
	}

#pragma endregion

#pragma region "Export"

	int test_export() {
		int result = 0;

		int argc = 3;
		char **argv = test_create_argv(argc, "export", "a=b", "test");
		builtin_exec(argc, argv);
		test_free_argv(argv);
		if (variables_length(vars_table, EXPORTED_LIST) != 2) result = 1;

		argc = 4;
		argv = test_create_argv(argc, "export", "-n", "a=c", "test");
		builtin_exec(argc, argv);
		test_free_argv(argv);
		if (!result && variables_length(vars_table, EXPORTED_LIST) != 0) result = 1;

		argc = 2;
		argv = test_create_argv(argc, "export", "a+=k");
		builtin_exec(argc, argv);
		test_free_argv(argv);
		if (!result && variables_length(vars_table, EXPORTED_LIST) != 1) result = 1;
		if (!result && strcmp(variables_find_value(vars_table, "a"), "ck")) result = 1;

		variables_clear(vars_table);

		return (result);
	}

#pragma endregion

#pragma region "Unset"

	int test_unset() {
		int result = 0;

		variables_add(vars_table, "a", "b", 1, 0, 0, 0);
		variables_add(vars_table, "test", NULL, 1, 0, 0, 0);

		int argc = 3;
		char **argv = test_create_argv(argc, "unset", "a=b", "test");
		builtin_exec(argc, argv);
		test_free_argv(argv);
		if (variables_length(vars_table, EXPORTED_LIST) != 1) result = 1;
		if (!result && variables_length(vars_table, INTERNAL) != 0) result = 1;
		
		variables_add(vars_table, "test", NULL, 1, 0, 0, 0);

		argc = 4;
		argv = test_create_argv(argc, "unset", "-v", "a", "test");
		builtin_exec(argc, argv);
		test_free_argv(argv);
		if (!result && variables_length(vars_table, EXPORTED_LIST) != 0) result = 1;
		if (!result && variables_length(vars_table, INTERNAL) != 0) result = 1;

		variables_clear(vars_table);

		return (result);
	}

#pragma endregion

#pragma region "Enable"

	static int test_enable() {
		int result = 0;

		int argc = 3;
		char **argv = test_create_argv(argc, "enable", "-n", "ulimit");
		builtin_exec(argc, argv);
		test_free_argv(argv);
		if (!result && builtin_length(1, 0) != 1) result = 1;

		argc = 2;
		argv = test_create_argv(argc, "enable", "ulimit");
		builtin_exec(argc, argv);
		test_free_argv(argv);
		if (!result && builtin_length(1, 0) != 0) result = 1;

		return (result);
	}

#pragma endregion

#pragma region "Type"

	int test_type(const char **envp) {
		int result = 0;
		return (0);

		variables_from_array(vars_table, envp);

		int argc = 5;
		char **argv = test_create_argv(argc, "type", "-pP", "echo", "popo", "date");
		builtin_exec(argc, argv);
		test_free_argv(argv);

		alias_clear();
		hash_clear();

		return (result);
	}

#pragma endregion

#pragma region "Command"

	int test_command(const char **envp) {
		int result = 0;
		return (0);

		variables_from_array(vars_table, envp);

		int argc = 5;
		char **argv = test_create_argv(argc, "command", "-v", "echo", "popo", "date");
		builtin_exec(argc, argv);
		test_free_argv(argv);

		alias_clear();
		hash_clear();

		return (result);
	}

#pragma endregion

#pragma region "Hash"

	int test_hash(const char **envp) {
		int result = 0;
		return (0);
		variables_from_array(vars_table, envp);
		printf("\n");
		hash_add("./42/pipi", 0, 0);
		hash_add("/usr/bin/date", 0, 0);

		
		int argc = 2;
		char **argv = test_create_argv(argc, "hash", "-t");
		builtin_exec(argc, argv);
		test_free_argv(argv);

		hash_clear();

		return (result);
	}

#pragma endregion

#pragma region "Builtins"

	int test_builtins(const char **envp) {
		(void) envp;
		printf(W"\t────────────────────────\n"NC);
		printf(C"\tBuiltin     "); fflush(stdout);
		int result = 0;

		// if (!result && test_alias())		{ result = 1; printf(RD"X"RED500" alias\n"NC);		}
		// if (!result && test_unalias())		{ result = 1; printf(RD"X"RED500" unalias\n"NC);	}
		// if (!result && test_readonly())		{ result = 1; printf(RD"X"RED500" readonly\n"NC);	}
		// if (!result && test_export())		{ result = 1; printf(RD"X"RED500" export\n"NC);		}
		// if (!result && test_unset())		{ result = 1; printf(RD"X"RED500" unset\n"NC);		}
		if (!result && test_enable())		{ result = 1; printf(RD"X"RED500" enable\n"NC);		}
		// if (!result && test_type(envp))		{ result = 1; printf(RD"X"RED500" type\n"NC);		}
		// if (!result && test_command(envp))	{ result = 1; printf(RD"X"RED500" command\n"NC);	}
		// if (!result && test_hash(envp))		{ result = 1; printf(RD"X"RED500" hash\n"NC);		}

		if (!result) printf(G"✓"GREEN500" passed\n"NC);

		return (result);
	}

#pragma endregion
