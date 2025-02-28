/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:02:36 by vzurera-          #+#    #+#             */
/*   Updated: 2025/02/28 18:14:44 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// TMOUT close shell after prompt
// interctive variable
// ignoreeof of set "Use "logout" to leave the shell." en stderr
// datos por stdin a 42sh se considera script
// cdspell effect only in interactive
// \'"'!!'"\' CTRL + Left / CTRL + Right jodido

// alias echo=date c=lala d=lolo b='lele ' f=lili g=h h='j b d ' j='h'
//char *input2 = ft_strdup("c ; d ; (g) d d | f $(( (b) $(echo `b g` $((b * 3)) ) + $(b b 3 | wc -l) )) || d");

// \& \&& \| \\ \" \' \( \` \${ $\{ no es válido

#pragma region "Includes"

	#include "libft.h"
	#include "terminal/readinput/prompt.h"
	#include "terminal/readinput/readinput.h"
	#include "terminal/readinput/history.h"
	#include "terminal/input.h"

#pragma endregion

#pragma region "Input"

	char *get_input() {
		char *input = NULL; char *input_hist = NULL;

		if (!(input = readinput(prompt_PS1)))	return (NULL);
		if (ft_isspace_s(input))				return (input);

		t_context main;		ft_memset(&main, 0, sizeof(t_context));
		t_context alias;	ft_memset(&alias, 0, sizeof(t_context));

		expand_history(&input, &main); input_hist = ft_strdup(input);

		expand_alias(&input, &alias);
		context_copy(&main, &alias);

		if (check_syntax(input, PARTIAL, &main)) {
			stack_clear(&main.stack);
			stack_clear(&alias.stack);
			sfree(input_hist);
			return (ft_strdup(""));
		}

		context_copy(&main, &alias);
		
		while (main.in_token || main.stack || main.in_quotes || main.in_dquotes) {
			bool add_newline = !main.in_token;

			char *cont_line = readinput(prompt_PS2);
			if (!cont_line) {
				stack_clear(&main.stack);
				stack_clear(&alias.stack);
				sfree(input); sfree(input_hist);
				return (NULL);
			}

			expand_history(&cont_line, &main);

			if (add_newline)
				input_hist = ft_strjoin_sep(input_hist, "\n", cont_line, 1);
			else if (alias.in_escape) {
				size_t len = ft_strlen(input_hist);
				while (len && ft_isspace(input_hist[len - 1])) input_hist[--len] = '\0';
				if (input_hist[len - 1] == '\\') input_hist[--len] = '\0';
				input_hist = ft_strjoin(input_hist, cont_line, 1);
			} else
				input_hist = ft_strjoin_sep(input_hist, " ", cont_line, 1);

			expand_alias(&cont_line, &alias);
			context_copy(&main, &alias);

			if (check_syntax(input, PARTIAL, &main)) {
				sfree(input); sfree(cont_line); sfree(input_hist);
				stack_clear(&main.stack);
				stack_clear(&alias.stack);
				return (ft_strdup(""));
			}
	
			if (add_newline)
				input = ft_strjoin_sep(input, "\n", cont_line, 6);
			else if (alias.in_escape) {
				size_t len = ft_strlen(input);
				while (len && ft_isspace(input[len - 1])) input[--len] = '\0';
				if (input[len - 1] == '\\') input[--len] = '\0';
				input = ft_strjoin(input, cont_line, 1);
			} else
				input = ft_strjoin_sep(input, " ", cont_line, 6);

			context_copy(&main, &alias);
		}

		stack_clear(&main.stack);
		stack_clear(&alias.stack);
		history_add(input_hist, false);	sfree(input_hist);

		return (input);
	}

#pragma endregion
