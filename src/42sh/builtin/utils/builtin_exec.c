/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 21:45:05 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/24 15:10:09 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

// unalias y unalias -a
// unset y unset -v

int builtin_exec(t_arg *args) {
	if (!args || !args->value) return (0);

	t_builtin *builtin = builtin_find(args->value);
	if (builtin && !builtin->disabled) {
		int exit_code = builtin->execute(args->next);
		if (exit_code != -1) shell.exit_code = exit_code;
		return (1);
	}

	return (0);
}
