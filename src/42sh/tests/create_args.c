/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 20:20:50 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/18 22:52:25 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"
	#include "parser/tokenizer/args.h"

#pragma endregion

static char *extract_word(char *line, int *index) {
    int start = *index;
	int quoted = 0, move = 0;

	if (line[*index] == '#')		{ (*index) += ft_strlen(&line[*index]); return (strdup(&line[*index])); }
	if (line[*index] == ';')		{ (*index)++; return (strdup(";")); }
	if (line[*index] == '\n')		{ (*index)++; return (strdup("\n")); }

	if (line[*index] == '\\')		{ quoted = 1; move = true; }
	else if (line[*index] == '\'')	{ quoted = 2; move = true; }
	else if (line[*index] == '"')	{ quoted = 3; move = true; }

	if (move) { memmove(&line[*index], &line[*index + 1], ft_strlen(&line[*index + 1]) + 1); move = false; }

    while (line[*index] && !(ft_isspace(line[*index]) && !quoted) && !((line[*index] == ';' || line[*index] == '\n') && !quoted) && !(line[*index] == '#' && !quoted)) {
		if (!quoted && line[*index] == '\\')			{ quoted = 1; move = true; }
		else if (quoted == 1)							quoted = 0;
		else if (!quoted && line[*index] == '\'')		{ quoted = 2; move = true; }
		else if (quoted == 2 && line[*index] == '\'')	{ quoted = 0; move = true; }
		else if (!quoted && line[*index] == '"')		{ quoted = 3; move = true; }
		else if (quoted == 3 && line[*index] == '"')	{ quoted = 0; move = true; }
		
		if (move) { memmove(&line[*index], &line[*index + 1], ft_strlen(&line[*index + 1]) + 1); move = false; }
		else (*index)++;
	}

	if (start == *index) return (strdup(""));
    return (strndup(&line[start], *index - start));
}

static t_arg *add_arg(t_arg **head, char *value) {
    t_arg *new_arg = calloc(1, sizeof(t_arg));

    new_arg->value = value;

    if (!*head) *head = new_arg;
    else {
        t_arg *current = *head;
        while (current->next)
            current = current->next;
        new_arg->prev = current;
        current->next = new_arg;
    }

    return (new_arg);
}

t_arg *test_create_args(char *line) {
    t_arg *args = NULL;

    int i = 0;
    while (line[i]) {
        while (line[i] && ft_isspace(line[i])) i++;
        if (!line[i]) break;

        char *arg = NULL;

		arg = extract_word(line, &i);
		if (*arg == '#') return (args);
        add_arg(&args, arg);
    }

    return (args);
}

void test_free_args(t_arg *args) {
    t_arg *current = args;
    t_arg *next;

    while (current) {
        next = current->next;
        free(current->value);
        free(current);
        current = next;
    }
}
