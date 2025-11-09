/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 13:53:15 by vzurera-          #+#    #+#             */
/*   Updated: 2025/03/02 19:01:08 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"
	#include "terminal/terminal.h"
	#include "main/shell.h"
	#include "utils/paths.h"

#pragma endregion

#pragma region "Variables"

	t_shell	shell;

#pragma endregion

int shell_time() { return (time(NULL) - shell.started); }

#pragma region "Initialize"

	int shell_initialize() {
		shell.interactive = false;
		shell.pid = getpid();
		shell.parent_pid = getppid();
		shell.subshell_level = 0;
		shell.started = time(NULL);
		srand(shell.started);
		shell.cwd = get_cwd("shell-init");
		terminal.bk_stdin = sdup(STDIN_FILENO);
		terminal.bk_stdout = sdup(STDOUT_FILENO);
		terminal.bk_stderr = sdup(STDERR_FILENO);

		return (0);
	}

#pragma endregion