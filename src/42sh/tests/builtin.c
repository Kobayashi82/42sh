/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 19:10:01 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/19 14:37:24 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Alias

	static int test_alias() {   
		int result = 0;

		t_arg *args = test_create_args("alias a=b b=c");
		alias(args->next); test_free_args(args);
		if (alias_length() != 2) result = 1;
		
		args = test_create_args("alias a=d");
		alias(args->next); test_free_args(args);
		if (!result && ft_strcmp(alias_find("a")->value, "d")) result = 1;

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

		return (result);
	}

#pragma endregion

#pragma region Builtin

	int test_builtin() {
		printf(W"\t────────────────────────\n"NC);
		printf(C"\tBuiltin     ");
		int result = 0;

		if (!result && test_alias())		{ result = 1; printf(RD"X"RED500" alias\n"NC); }
		if (!result && test_export())		{ result = 1; printf(RD"X"RED500" export\n"NC); }

		if (!result) printf(G"✓"GREEN500" passed\n"NC);

		return (result);
	}

#pragma endregion
