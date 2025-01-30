/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 20:20:50 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/30 13:30:37 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"
	#include "parser/args.h"

	#include <string.h>
	#include <stdio.h>

#pragma endregion

// int is_quote(char c) { return c == '\'' || c == '"'; }

// char *extract_quoted(char *line, char quote, int *index) {
//     int start = ++(*index);

//     while (line[*index] && line[*index] != quote) (*index)++;

//     if (line[*index] == quote) {
//         int len = *index - start;
//         char *arg = strndup(&line[start], len);
//         (*index)++;
//         return (arg);
//     }

//     return (NULL);
// }

static char *extract_word(char *line, int *index) {
    int start = *index;
	int quoted = 0, move = 0;

	if (line[*index] == ';')		{ (*index)++; return (ft_strndup(&line[start], *index - start)); }

	if (line[*index] == '\\')		{ quoted = 1; move = true; }
	else if (line[*index] == '\'')	{ quoted = 2; move = true; }
	else if (line[*index] == '"')	{ quoted = 3; move = true; }

	if (move) { ft_memmove(&line[*index], &line[*index + 1], ft_strlen(&line[*index + 1]) + 1); move = false; }

    while (line[*index] && !(ft_isspace(line[*index]) && !quoted) && !(line[*index] == ';' && !quoted)) {
		if (!quoted && line[*index] == '\\')			{ quoted = 1; move = true; }
		else if (quoted == 1)							quoted = 0;
		else if (!quoted && line[*index] == '\'')		{ quoted = 2; move = true; }
		else if (quoted == 2 && line[*index] == '\'')	{ quoted = 0; move = true; }
		else if (!quoted && line[*index] == '"')		{ quoted = 3; move = true; }
		else if (quoted == 3 && line[*index] == '"')	{ quoted = 0; move = true; }
		
		if (move) { ft_memmove(&line[*index], &line[*index + 1], ft_strlen(&line[*index + 1]) + 1); move = false; }
		else (*index)++;
	}

	if (start == *index) return (NULL);
    return (ft_strndup(&line[start], *index - start));
}

static t_arg *add_arg(t_arg **head, char *value) {
    t_arg *new_arg = ft_calloc(1, sizeof(t_arg));
    if (!new_arg) return (NULL);

    new_arg->value = value;
    new_arg->next = NULL;

    if (!*head) *head = new_arg;
    else {
        t_arg *current = *head;
        while (current->next)
            current = current->next;
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

        if (!arg) {
            while (args) {
                t_arg *tmp = args;
                args = args->next;
                sfree(tmp->value);
                sfree(tmp);
            }
            return (NULL);
        }

        if (!add_arg(&args, arg)) {
            sfree(arg);
            while (args) {
                t_arg *tmp = args;
                args = args->next;
                sfree(tmp->value);
                sfree(tmp);
            }
            return (NULL);
        }
    }

    return (args);
}

void test_free_args(t_arg *args) {
    t_arg *current = args;
    t_arg *next;

    while (current) {
        next = current->next; // Guardar el siguiente nodo
        sfree(current->value); // Liberar el valor del argumento
        sfree(current);        // Liberar el nodo actual
        current = next;       // Avanzar al siguiente nodo
    }
}
