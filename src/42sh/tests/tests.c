/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 20:28:05 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/26 14:37:55 by vzurera-         ###   ########.fr       */
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
		if (test_builtin(envp))			failed = 1;

		printf(W"\t────────────────────────\n"NC); fflush(stdout);

		//echo $"Hello" $var $$ ${path} $(command) 'text literal' > output
		//echo "Hello\\\" World" \\\$var 'Single\\'quote' > output

		//const char *input = "echo $var $$ ${path} $(command) 'text literal' > output";
		// const char *input = "echo \"Hello\\\\\\\" World\" \\\\\\$var 'Single\\\\''quote' > output";
		// size_t pos = 0;
		// char *word;

		// printf("Input: %s\n", input);

		// while ((word = get_next_word(input, &pos)) != NULL) {
		// 	printf("Word: %s\n", word);
		// 	free(word);
		// }

		first_step();
		shell.exit_code = failed;
		data_free();
		return (1);
	}

#pragma endregion
