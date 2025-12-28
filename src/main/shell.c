/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 13:53:15 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/28 16:49:25 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "terminal/terminal.h"
	#include "terminal/readinput/prompt.h"
	#include "terminal/readinput/history.h"

	#include "hashes/alias.h"
	#include "hashes/builtin.h"
	#include "hashes/variable.h"

	#include "main/options.h"
	#include "main/shell.h"

	#include "utils/libft.h"
	#include "utils/paths.h"

	#include <time.h>

#pragma endregion

#pragma region "Variables"

	t_shell	shell;

#pragma endregion

int shell_time() { return (time(NULL) - shell.started); }

#pragma region "Initialize"

	int initialize(int argc, const char **argv, const char **envp) {
		//	uid, euid
		//	PS1, PS2
		//	column, row
		builtin_initialize();
		options_initialize();
		alias_initialize();
		variables_initialize(vars_table, envp);
		prompt_initialize();
		shell.source = SRC_NO_INTERACTIVE;
		shell.pid = getpid();
		shell.parent_pid = getppid();
		shell.subshell_level = 0;
		shell.started = time(NULL);
		srand(shell.started);
		shell.cwd = get_cwd("shell-init");

		// Args
		shell.optpos = 1;
		if (argc > 0 && argv && argv[0]) {
			shell.fullname = (argv[0][0] == '-') ? argv[0] + 1 : argv[0];
    		shell.name = strrchr(shell.fullname, '/');
    		shell.name = (shell.name) ? shell.name + 1 : shell.fullname;
			shell.arg0 = (argv[0][0] == '-') ? "-42sh" : "42sh";
			shell.argv = (argc > 1) ? &argv[1] : NULL;
			shell.argc = argc - 1;

			static const char *argus[5];
			argus[0] = "-a";
			argus[1] = "patata";
			argus[2] = "-b";
			argus[4] = NULL;
			shell.argv = argus;
			shell.argc = 3;
		} else {
			shell.fullname = "42sh";
			shell.name = "42sh";
			shell.arg0 = "42sh";
			shell.argv = NULL;
			shell.argc = 0;
		}

		terminal.bk_stdin = dup(STDIN_FILENO);
		terminal.bk_stdout = dup(STDOUT_FILENO);
		terminal.bk_stderr = dup(STDERR_FILENO);
		//	Execute ~/.42shrc
		history_initialize();

		return (0);
	}

#pragma endregion