/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 13:53:15 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/26 11:37:18 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "terminal/terminal.h"
	#include "terminal/readinput/history.h"
	#include "terminal/readinput/prompt.h"
	#include "internal/shell.h"
	#include "utils/utils.h"

#pragma endregion

#pragma region "Variables"

	t_shell	shell;

#pragma endregion

#pragma region "Initialize"

	int initialize(int argc, const char **argv, const char **envp) {
		t_env *env = calloc(1, sizeof(t_env));
		if (!env) exit_error(E_NO_MEMORY, 1, NULL, NULL, EE_FREE_NONE, EE_EXIT);

		shell.name_exec = (argv[0][0] == '-') ? argv[0] + 1 : argv[0];
		shell.name_bin = strrchr(shell.name_exec, '/');
		shell.name_bin = (shell.name_bin) ? shell.name_bin + 1 : shell.name_exec;
		shell.name = (shell.login_shell) ? "-"PROYECTNAME : PROYECTNAME;

		// Hay que procesar antes las opciones

		shell.env = env;
		if (argc > 0) {
			char *my_argv[5];
			my_argv[0] = "-a";
			my_argv[1] = "patata";
			my_argv[2] = "-b";
			my_argv[3] = "frita";
			my_argv[4] = NULL;
			positional_params_set(shell.env, 4, my_argv);
			// positional_params_set(shell.env, argc - 1, &argv[1]);
		}
		shell.env->argv0 = argv[0];
		shell.env->optpos = 1;
		shell.env->sourced = 0;
		shell.env->parent = NULL;

		//	uid, euid
		//	PS1, PS2
		//	column, row
		builtin_initialize();
		options_initialize();
		variable_initialize(shell.env, envp);
		shell.mode = MD_NO_INTERACTIVE;
		shell.pid = getpid();
		shell.parent_pid = getppid();
		shell.subshell_level = 0;
		shell.started = seconds_since(0);
		srand(shell.started);
		shell.dirs.cwd = get_cwd("shell-init");

		terminal.bk_stdin = dup(STDIN_FILENO);
		terminal.bk_stdout = dup(STDOUT_FILENO);
		terminal.bk_stderr = dup(STDERR_FILENO);
		prompt_initialize();
		//	Execute ~/.42shrc
		history_initialize();

		return (0);
	}

#pragma endregion