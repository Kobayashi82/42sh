/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 20:20:50 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/22 19:14:38 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#include <string.h>
#include <stdio.h>

static int is_quote(char c) { return c == '\'' || c == '"'; }

static char *extract_quoted(char *line, char quote, int *index) {
    int start = ++(*index);

    while (line[*index] && line[*index] != quote) (*index)++;

    if (line[*index] == quote) {
        int len = *index - start;
        char *arg = strndup(&line[start], len);
        (*index)++;
        return (arg);
    }

    return (NULL);
}

static char *extract_word(char *line, int *index) {
    int start = *index;
    while (line[*index] && !ft_isspace(line[*index]) && !is_quote(line[*index]))
        (*index)++;
    return (strndup(&line[start], *index - start));
}

static t_arg *add_arg(t_arg **head, char *value) {
    t_arg *new_arg = malloc(sizeof(t_arg));
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

        if (is_quote(line[i]))	arg = extract_quoted(line, line[i], &i);
        else					arg = extract_word(line, &i);

        if (!arg) {
            while (args) {
                t_arg *tmp = args;
                args = args->next;
                free(tmp->value);
                free(tmp);
            }
            return (NULL);
        }

        if (!add_arg(&args, arg)) {
            free(arg);
            while (args) {
                t_arg *tmp = args;
                args = args->next;
                free(tmp->value);
                free(tmp);
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
        free(current->value); // Liberar el valor del argumento
        free(current);        // Liberar el nodo actual
        current = next;       // Avanzar al siguiente nodo
    }
}
