/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 13:53:15 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/19 20:06:04 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "terminal/terminal.h"
#include "main/shell.h"

t_shell	shell;

int shell_time() { return (time(NULL) - shell.started); }

#pragma region Initialize

	int shell_initialize() {
		shell.pid = getpid();
		shell.parent_pid = getppid();
		shell.subshell_level = 0;
		shell.started = time(NULL);
		srand(shell.started);
		terminal.bk_stdin = sdup(STDIN_FILENO);
		terminal.bk_stdout = sdup(STDOUT_FILENO);
		terminal.bk_stderr = sdup(STDERR_FILENO);

		return (0);
	}

#pragma endregion