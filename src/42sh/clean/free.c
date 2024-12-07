/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 13:07:44 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/07 21:33:30 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Data

	//	Free the data structure
	void data_free() {
		free(data.terminal.input);
		free(data.terminal.msg);
		close(data.shell.bk_std.bk_stdin);
		close(data.shell.bk_std.bk_stdout);
		close(data.shell.bk_std.bk_stderr);
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
		if (prompt_PS1) free(prompt_PS1);
		if (prompt_PS2) free(prompt_PS2);
		variable_clear(main_table);
	}

#pragma endregion

//	Free a char array
void free_array(char **array) {
	char **tmp_array = array;

	while (array && *array) free(*(array++));
	if (tmp_array)			free(tmp_array);
}
