/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 21:45:05 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/28 00:28:52 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "utils/libft.h"
	#include "tests/args.h"
	#include "builtins/builtins.h"
	#include "hashes/builtin.h"
	#include "main/shell.h"

#pragma endregion

int builtin_exec(t_arg *args) {
	if (!args || !args->value) return (0);

	t_builtin *builtin = builtin_find(args->value);
	if (builtin && !builtin->disabled) {
		if (strcmp(args->value, "ulimit") && strcmp(args->value, "exit") && strcmp(args->value, "logout")) args = args->next;
		int exit_code = builtin->execute(args);
		if (exit_code != -1) shell.exit_code = exit_code;
		return (1);
	}

	return (0);
}
