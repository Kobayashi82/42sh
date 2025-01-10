/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 13:07:44 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/10 14:21:49 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Data

	//	Free the data structure
	void data_free() {
		safe_free(terminal.input);
		for (int i = 0; i < 1024; ++i) if (terminal.msg[i]) safe_free(terminal.msg[i]);
		close(terminal.bk_stdin);
		close(terminal.bk_stdout);
		close(terminal.bk_stderr);
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
		if (prompt_PS1) safe_free(prompt_PS1);
		if (prompt_PS2) safe_free(prompt_PS2);
		alias_clear();
		variables_clear(vars_table);
		history_clear();
		cmdp_clear();
		builtin_clear();
		while (tokens.args) {
			t_arg *tmp = tokens.args;
			if (tokens.args->value) safe_free(tokens.args->value);
			tokens.args = tokens.args->next;
			safe_free(tmp);
		}
		safe_free_all();
	}

#pragma endregion
