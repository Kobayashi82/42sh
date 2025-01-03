/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 13:07:44 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/26 14:45:53 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Data

	//	Free the data structure
	void data_free() {
		free(terminal.input);
		for (int i = 0; i < 1024; ++i) if (terminal.msg[i]) free(terminal.msg[i]);
		close(terminal.bk_stdin);
		close(terminal.bk_stdout);
		close(terminal.bk_stderr);
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
		if (prompt_PS1) free(prompt_PS1);
		if (prompt_PS2) free(prompt_PS2);
		alias_clear();
		variables_clear(vars_table);
		history_clear();
		cmdp_clear();
		builtin_clear();
		while (tokens.args) {
			t_arg *tmp = tokens.args;
			if (tokens.args->value) free(tokens.args->value);
			tokens.args = tokens.args->next;
			free(tmp);
		}

	}

#pragma endregion
