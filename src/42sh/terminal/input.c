/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:02:36 by vzurera-          #+#    #+#             */
/*   Updated: 2025/02/25 16:12:43 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma region "Includes"

	#include "libft.h"
	#include "terminal/readinput/readinput.h"
	#include "terminal/readinput/prompt.h"
	#include "terminal/readinput/history.h"
	#include "main/error.h"

#pragma endregion

#pragma region "Syntax"

	static int check_syntax(char *value) {
		int invalid = 0;

		if (invalid) {
			value = expand_history(value);
			sfree(value);
			write(1, "Error chiquitin\n", 16);
			return (1);
		}

		return (0);
	}

#pragma endregion

#pragma region "Line"

	static int unclosed(char *value) {
		(void) value;
		//size_t len = ft_max(0, ft_strlen(value) - 1);
		
		//if (value[len] != '"')		return (2);
		//if (value[len] != '\\')		return (1);

		return (0);
	}

#pragma endregion

#pragma region "Input"

	char *get_input() {
		char *input = NULL;

		if (!(input = readinput(prompt_PS1))) return (NULL);
		int unclosed_mode = 0;

		if (check_syntax(input)) return (ft_strdup(""));

		while ((unclosed_mode = unclosed(input))) {
			char *cont_line = readinput(prompt_PS2);
			if (!cont_line) { sfree(input); return (NULL); }

			if (unclosed_mode == 1)	input = ft_strjoin(input, cont_line, 3);
			else 					input = ft_strjoin_sep(input, "\n", cont_line, 6);

			if (check_syntax(input)) return (ft_strdup(""));
		}

		history_add(input, false);
		return (expand_history(input));
	}

#pragma endregion
