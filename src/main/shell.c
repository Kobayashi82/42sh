/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 13:53:15 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/10 20:20:18 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "terminal/terminal.h"
	#include "terminal/readinput/history.h"
	#include "terminal/readinput/prompt.h"
	#include "main/shell.h"
	#include "utils/utils.h"

#pragma endregion

#pragma region "Variables"

	t_shell	shell;

#pragma endregion

#pragma region "Initialize"

	int initialize(int argc, const char **argv, const char **envp) {
		t_env *env = calloc(1, sizeof(t_env));
		if (!env) exit_error(NO_MEMORY, 1, NULL, 0, EE_EXIT);

		shell.name_exec = (argv[0][0] == '-') ? argv[0] + 1 : argv[0];
		shell.name_bin = strrchr(shell.name_exec, '/');
		shell.name_bin = (shell.name_bin) ? shell.name_bin + 1 : shell.name_exec;
		shell.name = (shell.login_shell) ? "-"PROYECTNAME : PROYECTNAME;

		// Hay que procesar antes las opciones

		shell.env = env;
		if (argc > 0) {
			static const char *argus[5];
			argus[0] = "-a";
			argus[1] = "patata";
			argus[2] = "-b";
			argus[3] = "frita";
			argus[4] = NULL;
			shell.env->argc = 4;
			shell.env->argv = array_clone(shell.env->argc, argus);

			// shell.env->argc = argc - 1;
			// shell.env->argv = array_clone(shell.env->argc, &argv[1]);
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
		alias_initialize();
		variable_initialize(shell.env, envp);
		prompt_initialize();
		shell.mode = MD_NO_INTERACTIVE;
		shell.pid = getpid();
		shell.parent_pid = getppid();
		shell.subshell_level = 0;
		shell.started = seconds_since(0);
		srand(shell.started);
		shell.cwd = get_cwd("shell-init");

		terminal.bk_stdin = dup(STDIN_FILENO);
		terminal.bk_stdout = dup(STDOUT_FILENO);
		terminal.bk_stderr = dup(STDERR_FILENO);
		//	Execute ~/.42shrc
		history_initialize();

		return (0);
	}

#pragma endregion