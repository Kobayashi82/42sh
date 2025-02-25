/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:02:36 by vzurera-          #+#    #+#             */
/*   Updated: 2025/02/25 21:50:20 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"
	#include "terminal/readinput/prompt.h"
	#include "terminal/readinput/readinput.h"
	#include "terminal/readinput/history.h"
	#include "terminal/input.h"

#pragma endregion

#pragma region "Line"

	static int unclosed(char *input) {
		(void) input;
		//size_t len = ft_max(0, ft_strlen(input) - 1);
		
		//if (input[len] != '"')		return (2);
		//if (input[len] != '\\')		return (1);

		return (0);
	}

#pragma endregion

#pragma region "Input"

	char *get_input() {
		char *input = NULL;

		if (!(input = readinput(prompt_PS1))) return (NULL);
		if (ft_isspace_s(input)) return (input);
		int unclosed_mode = 0;

		expand_alias(&input);
		if (check_syntax(&input)) return (ft_strdup(""));

		while ((unclosed_mode = unclosed(input))) {
			char *cont_line = readinput(prompt_PS2);
			if (!cont_line) { sfree(input); return (NULL); }

			expand_alias(&input);
			if (unclosed_mode == 1)	input = ft_strjoin(input, cont_line, 3);
			else 					input = ft_strjoin_sep(input, "\n", cont_line, 6);

			if (check_syntax(&input)) return (ft_strdup(""));
		}

		expand_history(&input);
		history_add(input, false);
		return (input);
	}

#pragma endregion
