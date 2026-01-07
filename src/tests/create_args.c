/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 20:20:50 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/07 23:56:38 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "utils/utils.h"
	#include "tests/args.h"

	#include <stdarg.h>

#pragma endregion

static char *extract_word(char *line, int *index) {
    int start = *index;
	int quoted = 0, move = 0;

	if (line[*index] == '#')		{ (*index) += ft_strlen(&line[*index]); return (ft_strdup(&line[*index])); }
	if (line[*index] == ';')		{ (*index)++; return (ft_strdup(";")); }
	if (line[*index] == '\n')		{ (*index)++; return (ft_strdup("\n")); }

	if (line[*index] == '\\')		{ quoted = 1; move = 1; }
	else if (line[*index] == '\'')	{ quoted = 2; move = 1; }
	else if (line[*index] == '"')	{ quoted = 3; move = 1; }

	if (move) { memmove(&line[*index], &line[*index + 1], ft_strlen(&line[*index + 1]) + 1); move = 0; }

    while (line[*index] && !(isspace(line[*index]) && !quoted) && !((line[*index] == ';' || line[*index] == '\n') && !quoted) && !(line[*index] == '#' && !quoted)) {
		if (!quoted && line[*index] == '\\')			{ quoted = 1; move = 1; }
		else if (quoted == 1)							quoted = 0;
		else if (!quoted && line[*index] == '\'')		{ quoted = 2; move = 1; }
		else if (quoted == 2 && line[*index] == '\'')	{ quoted = 0; move = 1; }
		else if (!quoted && line[*index] == '"')		{ quoted = 3; move = 1; }
		else if (quoted == 3 && line[*index] == '"')	{ quoted = 0; move = 1; }
		
		if (move) { memmove(&line[*index], &line[*index + 1], ft_strlen(&line[*index + 1]) + 1); move = 0; }
		else (*index)++;
	}

	if (start == *index) return (ft_strdup(""));
    return (ft_strndup(&line[start], *index - start));
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
        while (line[i] && isspace(line[i])) i++;
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


	char **test_create_argv(int argc, ...) {
		va_list args;
		char **argv;
		int i;

		argv = malloc(sizeof(char *) * (argc + 1));
		if (!argv) return (NULL);

		va_start(args, argc);
		for (i = 0; i < argc; i++)
			argv[i] = ft_strdup(va_arg(args, char *));
		va_end(args);

		argv[i] = NULL;

		return (argv);
	}

	void test_free_argv(char **argv) {
		if (!argv) return;
		for (int i = 0; argv[i]; ++i) free(argv[i]);
		free(argv);
	}
