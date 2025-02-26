/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:02:36 by vzurera-          #+#    #+#             */
/*   Updated: 2025/02/26 23:12:57 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// TMOUT close shell after prompt
// interctive variable
// ignoreeof of set "Use "logout" to leave the shell." en stderr
// datos por stdin a 42sh se considera script
// cdspell effect only in interactive
// \'"'!!'"\' CTRL + Left / CTRL + Right jodido

#pragma region "Includes"

	#include "libft.h"
	#include "terminal/readinput/prompt.h"
	#include "terminal/readinput/readinput.h"
	#include "terminal/readinput/history.h"
	#include "terminal/input.h"

#pragma endregion

#include <stdio.h>

#pragma region "Input"

	char *get_input() {
		char *input = NULL;

		if (!(input = readinput(prompt_PS1))) return (NULL);
		if (ft_isspace_s(input)) return (input);
		int partial_mode = 0;

		// alias echo=date c=lala d=lolo b=lele f=lili
		// $(( $(echo $((2 ** 3)) ) + $(seq 1 3 | wc -l) ))
		char *input2 = ft_strdup("c ; d ; (d) d d | f $(( (b) $(echo $((b ** 3)) ) + $(b b 3 | wc -l) )) || d");

		expand_aliases(&input2);
		ft_printf(1, "%s\n", input2);

		sfree(input2);

		// size_t i = 0;
		// size_t end = find_arithmetic_expression(input2, &i);

		// if (end > 0) {
		// 	printf("Expresión aritmética encontrada. Termina en la posición: %zu\n", end);
		// 	printf("Contenido: %.*s\n", (int)end, input2);
		// } else {
		// 	printf("No se encontró una expresión aritmética.\n");
		// }

		expand_history(&input, partial_mode);
		//expand_alias(&input, partial_mode);
		if ((partial_mode = check_syntax(&input, PARTIAL, partial_mode)) > 3) return (ft_strdup(""));
		
		while (partial_mode) {
			char *cont_line = readinput(prompt_PS2);
			if (!cont_line) { sfree(input); return (NULL); }

			expand_history(&cont_line, partial_mode);
			//expand_alias(&cont_line, partial_mode);

			int old_partial_mode = partial_mode;
			if ((partial_mode = check_syntax(&cont_line, PARTIAL, partial_mode)) > 2) {
				sfree(input); sfree(cont_line);
				return (ft_strdup(""));
			}

			if (old_partial_mode == 1)	input = ft_strjoin(input, cont_line, 3);
			else 						input = ft_strjoin_sep(input, "\n", cont_line, 6);
		}

		history_add(input, false);
		return (input);
	}

#pragma endregion
