/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 21:45:05 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/28 12:20:57 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "utils/libft.h"
	#include "tests/args.h"
	#include "builtins/builtins.h"
	#include "hashes/builtin.h"
	#include "main/shell.h"

#pragma endregion

#pragma region "Args to Argv"

	static char **args_to_argv(t_arg *args, int *argc) {
		int count = 0;

		t_arg *tmp = args;
		while (tmp) {
			if (tmp->value) count++;
			tmp = tmp->next;
		}

		char **argv = malloc(sizeof(char *) * (count + 1));
		if (!argv) return (NULL);
		*argc = count;

		int i = 0;
		tmp = args;
		while (tmp) {
			if (tmp->value) argv[i++] = strdup(tmp->value);
			tmp = tmp->next;
		}
		argv[i] = NULL;

		return (argv);
	}

#pragma endregion

#pragma region "Free Argv"

	static void free_argv(char **argv) {
		if (!argv) return;
		for (int i = 0; argv[i]; ++i) free(argv[i]);
		free(argv);
	}

#pragma endregion

int builtin_exec(t_arg *args) {
	if (!args || !args->value) return (0);

	t_builtin *builtin = builtin_find(args->value);
	if (builtin && !builtin->disabled) {
		if (!strcmp(args->value, "getopts")) {
			int argc;
			char **argv = args_to_argv(args, &argc);
			int exit_code = builtin->execute2(argc, argv);
			free_argv(argv);
			if (exit_code != -1) shell.exit_code = exit_code;
			return (1);
		}
		if (strcmp(args->value, "ulimit") && strcmp(args->value, "exit") && strcmp(args->value, "logout")) args = args->next;
		int exit_code = builtin->execute(args);
		if (exit_code != -1) shell.exit_code = exit_code;
		return (1);
	}

	return (0);
}
