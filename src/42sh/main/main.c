/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:40:36 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/01 13:59:50 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

t_data	data;

int shell_time() { return (time(NULL) - data.shell.started); }

int initialize(int argc, char **argv, char **envp) {
	(void) argc;
	(void) argv;
	(void) envp;
	data.shell.pid = getpid();
	data.shell.started = time(NULL);
	srand(data.shell.started);
	data.shell.bk_std.bk_stdin = safe_dup(STDIN_FILENO);
	data.shell.bk_std.bk_stdout = safe_dup(STDOUT_FILENO);
	data.shell.bk_std.bk_stderr = safe_dup(STDERR_FILENO);
	return (0);
}

int main(int argc, char **argv, char **envp) {
	if (initialize(argc, argv, envp))
		return (1);
	printf("Welcome to 42sh\n");
	data_free();
	return (data.shell.exit_code);
}
