/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   positional.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 15:16:17 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/24 16:22:39 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma region "Includes"

	#include "internal/shell.h"
	#include "utils/utils.h"

#pragma endregion

#pragma region "Shift"

	int positional_params_shift(t_env *env, int offset) {
		shell.error = 0;
		if (!env) return (0);

		if (!offset)	return (0);
		if (offset < 0)	return (shell.error = E_POS_PARAM_RANGE, 1);
		if (offset > shell.env->argc) {
			if (shell.options.shift_verbose) return (shell.error = E_POS_PARAM_RANGE, 1);
			return (1);
		}

		for (int i = 0; i < offset; ++i)
			free(shell.env->argv[i]);
		memmove(shell.env->argv, shell.env->argv + offset, (shell.env->argc - offset + 1) * sizeof(char *));
		shell.env->argc -= offset;

		return (0);
	}

#pragma endregion

#pragma region "Set"

	int positional_params_set(t_env *env, int argc, char **argv) {
		if (!env) return (1);

		positional_params_clear(env);

		char **new_argv = malloc(sizeof(char *) * (argc + 1));
		if (!new_argv) return (shell.error = E_NO_MEMORY, 1);

		for (int i = 0; i < argc; ++i) {
			new_argv[i] = ft_strdup(argv[i]);
			if (!new_argv[i]) {
				array_free(new_argv);
				return (shell.error = E_NO_MEMORY, 1);
			}
		}
		new_argv[argc] = NULL;
		env->argc = argc;
		env->argv = new_argv;

		return (0);
	}

#pragma endregion

#pragma region "Clear"

	void positional_params_clear(t_env *env) {
		if (!env) return;

		for (int i = 0; i < env->argc; ++i)
			free(env->argv[i]);
		free(env->argv);
		env->argv = NULL;
		env->argc = 0;
	}

#pragma endregion

#pragma region "Print"

	void positional_params_print(t_env *env, int index) {
		(void) index;
		if (env) array_print((const char **)env->argv, STDOUT_FILENO, 0);
	}

#pragma endregion
