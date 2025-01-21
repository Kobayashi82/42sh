/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 20:28:05 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/21 22:04:10 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"
	#include "terminal/history.h"
	#include "hashes/alias.h"
	#include "hashes/variables.h"
	#include "hashes/builtin.h"
	#include "main/shell.h"
	#include "tests.h"

	#include <stdio.h>

#pragma endregion

#pragma region "Test"

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
		// 	sfree(word);
		// }

		//first_step();
		shell.exit_code = failed;
		return (1);
	}

#pragma endregion
