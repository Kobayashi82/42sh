/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 20:28:05 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/18 21:21:55 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Test

	int tests(int argc, const char **argv, const char **envp) {
		if (argc == 1 || (ft_strcmp(argv[1], "test") && ft_strcmp(argv[1], "tests"))) return (0);
		int failed = 0;
		printf(W"\t────────────────────────\n"NC);
		printf ("\t\033[7;36m\033[40m          TESTS         \033[7;36m\033[40m\n"NC);

		if (test_options())			failed = 1;
		if (test_aliases())			failed = 1;
		if (test_variables(envp))	failed = 1;
		if (test_history())			failed = 1;
		if (test_builtin())			failed = 1;

		printf(W"\t────────────────────────\n"NC); fflush(stdout);

		shell.exit_code = failed;
		data_free();
		return (1);
	}

#pragma endregion
