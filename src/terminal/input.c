/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:02:36 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/24 19:54:06 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Sintax tests
//
//	alias echo=date c=lala d=lolo b='lele ' f=lili g=h h='j b d ' j='h'
//	c ; d ; (g) d d | f $(( (b) $(echo `b g` $((b * 3)) ) + $(b b 3 | wc -l) )) || d

#pragma region "Includes"

	#include "utils/libft.h"
	#include "terminal/readinput/prompt.h"
	#include "terminal/readinput/readinput.h"
	#include "terminal/readinput/history.h"
	#include "terminal/input.h"
	#include "terminal/signals.h"
	#include "expansion/alias.h"
	#include "expansion/history.h"
	#include "main/shell.h"
	#include "parser/syntax.h"

#pragma endregion

#pragma region "Expand"

	char *expand_input(char *input) {
		if (!input || ft_isspace_s(input)) return (input);

		char *input_hist = NULL;

		t_context ctx_history;	memset(&ctx_history, 0, sizeof(t_context));
		t_context ctx_alias;	memset(&ctx_alias, 0, sizeof(t_context));

		expand_history(&input, &ctx_history, true);
		input_hist = ft_strdup(input);

		expand_alias(&input, &ctx_alias);
		printf("%s\n", input);

		if (shell.interactive) history_add(input_hist, false);
		free(input_hist);
		stack_clear(&ctx_history.stack);
		stack_clear(&ctx_alias.stack);

		return (input);
	}

	#pragma endregion

#pragma region "Readfile"

	static char *readfile() {
		size_t size = 1024, bytes_read = 0;
		char *value = calloc(size + 1, sizeof(char));

		while (1) {
			size_t bytes_to_read = size - bytes_read;
			ssize_t read_now = read(STDIN_FILENO, value + bytes_read, bytes_to_read);

			if (read_now == -1) { free(value); return (NULL); }

			bytes_read += read_now;
			value[bytes_read] = '\0';

			if (bytes_read == size) {
				size *= 2;
				value = realloc(value, size + 1);
			}

			if (read_now == 0) break;
		}

		return (value);
	}

#pragma endregion

#pragma region "Input"

	char *get_input() {
		char *input = NULL;

		if (!shell.interactive) {
			if (!(input = readfile()))				return (NULL);
		} else {
			if (!(input = readinput(prompt_PS1)))	return (NULL);
		}

		// expand_input() debe de expandir alias, historial y hacer una comprobación básica de sintaxis
		// o si eso, no hacer comprobación y delegar en el lexer/parser
		return (ft_isspace_s(input) ? input : expand_input(input));
	}

#pragma endregion
