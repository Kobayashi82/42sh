/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 19:10:01 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/18 19:18:11 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Alias

	int test_built_alias() {   
		t_arg arg6 = {"argu2", NULL};
		t_arg arg5 = {"k", &arg6};
		t_arg arg4 = {"k=koba", &arg5};
		t_arg arg3 = {"c", &arg4};
		t_arg arg2 = {"--", &arg3};
		t_arg arg1 = {"-p", &arg2};
		t_arg *args = &arg1;

		alias_add("a", "f");
		alias_add("b", "g");
		alias_add("c", "h");
		alias_add("d", "j");
		alias(args);

		//t_opt *options = parse_options(args, "eEn", false);

		// printf("Opciones válidas: %s\n", options->valid);
		// printf("Opciones inválidas: %s\n", options->invalid);
		// printf("Argumentos a partir de: %s\n", options->args ? options->args->value : "(null)");

		//free(options);

		return (0);
	}

#pragma endregion

#pragma region Export

	int test_built_export() {
		// t_arg arg6 = {"argu2", NULL};
		// t_arg arg5 = {"k", &arg6};
		t_arg arg4 = {"test=passed", NULL};
		t_arg arg3 = {"test2=$USER ' \\ `date` !!", &arg4};
		t_arg arg2 = {"--", &arg3};
		t_arg arg1 = {"-p", &arg2};
		t_arg *args = &arg1;

		variables_add(vars_table, "test", "readonly", false, true, false, false);
		export(args);
		export(NULL);
		printf("\n");
		variables_print(vars_table, EXPORTED, true);

		t_arg arg14 = {"test=papa", NULL};
			t_arg arg13 = {"no_var", &arg14};
			t_arg arg12 = {"--", &arg13};
			t_arg arg11 = {"-p", &arg12};
			t_arg *args1 = &arg11;

		printf("\n");
		printf("\n");
		export(args1);
		export(NULL);
		printf("\n");
		variables_print(vars_table, EXPORTED, true);

		// printf("%d\n", export(args1));
		// export(NULL);

		//t_opt *options = parse_options(args, "eEn", false);

		// printf("Opciones válidas: %s\n", options->valid);
		// printf("Opciones inválidas: %s\n", options->invalid);
		// printf("Argumentos a partir de: %s\n", options->args ? options->args->value : "(null)");

		//free(options);

		return (0);
	}

	int test_export2() {
		printf(W"\t────────────────────────\n"NC);
		printf(C"\tExport       ");

		int result = 0;

		if (!result && alias_find("testing"))		{ result = 1; printf(RD"X"RED500" find\n"NC); }
		if (!result) { alias_add("testing", "ls");
			if (!alias_find("testing"))				{ result = 1; printf(RD"X"RED500" create\n"NC); }
		}

		if (!result) { alias_delete("testing");
			if (alias_find("testing"))				{ result = 1; printf(RD"X"RED500" delete\n"NC); }
		}

		if (!result) { alias_clear();
			char **alias = alias_to_array(true);
			if (alias && alias[0])					{ result = 1; printf(RD"X"RED500" clear\n"NC); }
		}

		if (!result) printf(G"✓"GREEN500" passed\n"NC);

		return (result);
	}

#pragma endregion
