/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 19:10:01 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/19 19:21:11 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Alias

	static int test_alias() {   
		int result = 0;

		t_arg *args = test_create_args("alias a=b b=c c=h");
		alias(args->next); test_free_args(args);
		if (alias_length() != 3) result = 1;
		
		args = test_create_args("alias a=d");
		alias(args->next); test_free_args(args);
		if (!result && ft_strcmp(alias_find("a")->value, "d")) result = 1;

		alias_clear();

		return (result);
	}

#pragma endregion

#pragma region Unalias

	static int test_unalias() {   
		int result = 0;

		alias_add("a", "b");
		alias_add("b", "c");
		alias_add("c", "h");

		t_arg *args = test_create_args("unalias a");
		unalias(args->next); test_free_args(args);
		if (alias_length() != 2) result = 1;
		
		args = test_create_args("unalias b");
		unalias(args->next); test_free_args(args);
		if (alias_length() != 1) result = 1;

		args = test_create_args("unalias -a invalid");
		unalias(args->next); test_free_args(args);
		if (alias_length() != 0) result = 1;

		alias_clear();

		return (result);
	}

#pragma endregion

#pragma region Readonly

	static int test_readonly() {
		int result = 0;

		t_arg *args = test_create_args("readonly -p test_fixed");
		readonly(args->next); test_free_args(args);
		if (variables_length(vars_table, READONLY) != 1) result = 1;

		variables_clear(vars_table);

		return (result);
	}

#pragma endregion

#pragma region Export

	static int test_export() {
		int result = 0;

		t_arg *args = test_create_args("export a=b test");
		export(args->next); test_free_args(args);
		if (variables_length(vars_table, EXPORTED_LIST) != 2) result = 1;
		
		args = test_create_args("export -n a=c test");
		export(args->next); test_free_args(args);
		if (!result && variables_length(vars_table, EXPORTED_LIST) != 0) result = 1;

		args = test_create_args("export a+=k");
		export(args->next); test_free_args(args);
		if (!result && variables_length(vars_table, EXPORTED_LIST) != 1) result = 1;
		if (!result && ft_strcmp(variables_find(vars_table, "a")->value, "ck")) result = 1;

		variables_clear(vars_table);

		return (result);
	}

#pragma endregion

#pragma region Unset

	static int test_unset() {
		int result = 0;

		variables_add(vars_table, "a", "b", 1, 0, 0, 0);
		variables_add(vars_table, "test", NULL, 1, 0, 0, 0);

		t_arg *args = test_create_args("unset a=b test");
		unset(args->next); test_free_args(args);
		if (variables_length(vars_table, EXPORTED_LIST) != 1) result = 1;
		if (!result && variables_length(vars_table, INTERNAL) != 0) result = 1;
		
		variables_add(vars_table, "test", NULL, 1, 0, 0, 0);

		args = test_create_args("unset -v a test");
		unset(args->next); test_free_args(args);
		if (!result && variables_length(vars_table, EXPORTED_LIST) != 0) result = 1;
		if (!result && variables_length(vars_table, INTERNAL) != 0) result = 1;

		variables_clear(vars_table);

		return (result);
	}

#pragma endregion

#pragma region Builtin

	int test_builtin() {
		printf(W"\t────────────────────────\n"NC);
		printf(C"\tBuiltin     ");
		int result = 0;

		if (!result && test_alias())		{ result = 1; printf(RD"X"RED500" alias\n"NC); }
		if (!result && test_unalias())		{ result = 1; printf(RD"X"RED500" unalias\n"NC); }
		if (!result && test_readonly())		{ result = 1; printf(RD"X"RED500" readonly\n"NC); }
		if (!result && test_export())		{ result = 1; printf(RD"X"RED500" export\n"NC); }
		if (!result && test_unset())		{ result = 1; printf(RD"X"RED500" unset\n"NC); }

		if (!result) printf(G"✓"GREEN500" passed\n"NC);

		return (result);
	}

#pragma endregion
