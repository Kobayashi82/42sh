/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 20:28:05 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/10 21:16:54 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Variables

	int test_variables(char **envp, int print_array) {
		printf(CYAN300" Variables:\t\t ");

		int result = 0, envp_count = 0, vars_count = 0;
		variables_from_array(main_table,envp);
		char **vars = variables_to_array(main_table, EXPORTED);
		for (int i = 0; envp[i]; ++i) ++envp_count;
		for (int i = 0; vars[i]; ++i) ++vars_count;
		if (print_array) {
			array_print(envp, true);
			printf("\n=========================\n\n");
			array_print(vars, true);
			printf("\n\n");
		}
		array_free(vars);

		if (envp_count != vars_count) { result = 1; printf(RED500"KO "YELLOW500"- "RED500"Clone\n"NC); }
		if (!result && !variables_find(main_table, "PATH")) { result = 1; printf(RED500"KO "YELLOW500"- "RED500"Find\n"NC); }
		if (!result) { variables_add(main_table, "testing", "test", true, false, false, false);
			if (!variables_find(main_table, "testing")) { result = 1; printf(RED500"KO "YELLOW500"- "RED500"Create\n"NC); }
		}
		if (!result) { variables_delete(main_table, "testing");
			if (variables_find(main_table, "testing")) { result = 1; printf(RED500"KO "YELLOW500"- "RED500"Delete\n"NC); }
		}
		if (!result) { variables_clear(main_table);
			vars = variables_to_array(main_table, EXPORTED);
			if (vars && vars[0]) { result = 1; printf(RED500"KO "YELLOW500"- "RED500"Clear\n"NC); }
		}
		if (!result) printf(GREEN500"OK\n"NC);

		return (result);
	}

#pragma endregion

#pragma region Test

	int tests(int argc, char **argv, char **envp) {
		if (argc == 1 || ft_strcmp(argv[1], "test")) return (0);

		int failed = 0;

		printf(BLUE700"\n============================\n\n"NC);
		if (test_variables(envp, false)) failed = 1;
		printf(BLUE700"\n============================\n\n"NC);
		fflush(stdout);

		shell.exit_code = failed;
		data_free();
		return (1);
	}

#pragma endregion
