/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:10:33 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/22 20:04:32 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "internal/shell.h"
	#include "utils/utils.h"
	#include "utils/getopt.h"

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Help"

		int bt_set_help(int format, int no_print) {
			char *name = "set";
			char *syntax = "set [-abefhmnpuvxBCEHPTSVXLFN] [-o option-name] [--] [-] [arg ...]";
			char *description = "Set or unset values of shell options and positional parameters.";
			char *msg =
				"    Change the value of shell attributes and positional parameters, or\n"
				"    display the names and values of shell variables.\n\n"

				"    Options:\n"
				"      -a        Mark variables which are modified or created for export.\n"
				"      -b        Notify of job termination immediately.\n"
				"      -e        Exit immediately if a command exits with a non-zero status.\n"
				"      -f        Disable file name generation (globbing).\n"
				"      -h        Remember the location of commands as they are looked up.\n"
				"                command, not just those that precede the command name.\n"
				"      -m        Job control is enabled.\n"
				"      -n        Read commands but do not execute them.\n"
				"      -o option-name\n"
				"                Set the variable corresponding to option-name:\n"
				"                    allexport    same as -a\n"
				"                    braceexpand  same as -B\n"
				"                    emacs        use an emacs-style line editing interface\n"
				"                    errexit      same as -e\n"
				"                    errtrace     same as -E\n"
				"                    functrace    same as -T\n"
				"                    hashall      same as -h\n"
				"                    histexpand   same as -H\n"
				"                    history      enable command history\n"
				"                    ignoreeof    the shell will not exit upon reading EOF\n"
				"                    interactive-comments\n"
				"                                 allow comments to appear in interactive commands\n"
				"                    monitor      same as -m\n"
				"                    noclobber    same as -C\n"
				"                    noexec       same as -n\n"
				"                    noglob       same as -f\n"
				"                    notify       same as -b\n"
				"                    nounset      same as -u\n"
				"                    physical     same as -P\n"
				"                    pipefail     the return value of a pipeline is the status of\n"
				"                                 the last command to exit with a non-zero status,\n"
				"                                 or zero if no command exited with a non-zero status\n"
				"                    privileged   same as -p\n"
				"                    verbose      same as -v\n"
				"                    vi           use a vi-style line editing interface\n"
				"                    xtrace       same as -x\n"
				"      -p        Turned on whenever the real and effective user ids do not match.\n"
				"                Disables processing of the $ENV file and importing of shell\n"
				"                functions.  Turning this option off causes the effective uid and\n"
				"                gid to be set to the real uid and gid.\n"
				"      -u        Treat unset variables as an error when substituting.\n"
				"      -v        Print shell input lines as they are read.\n"
				"      -x        Print commands and their arguments as they are executed.\n"
				"      -B        the shell will perform brace expansion\n"
				"      -C        If set, disallow existing regular files to be overwritten\n"
				"                by redirection of output.\n"
				"      -E        If set, the ERR trap is inherited by shell functions.\n"
				"      -H        Enable ! style history substitution.  This flag is on\n"
				"                by default when the shell is interactive.\n"
				"      -P        If set, do not resolve symbolic links when executing commands\n"
				"                such as cd which change the current directory.\n"
				"      -T        If set, the DEBUG and RETURN traps are inherited by shell functions.\n"
				"      --        Assign any remaining arguments to the positional parameters.\n"
				"                If there are no remaining arguments, the positional parameters\n"
				"                are unset.\n"
				"      -         Assign any remaining arguments to the positional parameters.\n"
				"                The -x and -v options are turned off.\n\n"

				"      -S --positional            show all shell positional parameters\n"
				"      -V --variables             show all variables (local and exported)\n"
				"      -X --variables_exported    show exported variables\n"
				"      -L --variables_local       show all local variables\n"
				"      -F --functions             show all functions\n"
				"      -N --functions_name        show function names only\n\n"

				"    Using + rather than - causes these flags to be turned off.  The\n"
				"    flags can also be used upon invocation of the shell.  The current\n"
				"    set of flags may be found in $-.  The remaining n ARGs are positional\n"
				"    parameters and are assigned, in order, to $1, $2, .. $n.  If no\n"
				"    ARGs are given, all shell variables are printed.\n\n"
				"    Exit Status:\n"
				"      Returns success unless an invalid option is given.\n";

			if (!no_print) print(STDOUT_FILENO, NULL, P_RESET);

			if (format == HELP_SYNTAX) {
				print(STDOUT_FILENO, ft_strjoin(name, ": ", 0), P_FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin(syntax, "\n", 0), P_FREE_JOIN);
			}

			if (format == HELP_DESCRIPTION) {
				print(STDOUT_FILENO, ft_strjoin(name, " - ", 0), P_FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin(description, "\n", 0), P_FREE_JOIN);
			}

			if (format == HELP_NORMAL) {
				print(STDOUT_FILENO, ft_strjoin(name, ": ", 0), P_FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin(syntax, "\n", 0), P_FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin_sep("    ", description, "\n\n", 0), P_FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin(msg, "\n", 0), P_FREE_JOIN);
			}

			if (format == HELP_MANPAGE) {
				print(STDOUT_FILENO, "NAME\n", P_JOIN);
				print(STDOUT_FILENO, ft_strjoin_sep("    ", name, " - ", 0), P_FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin(description, "\n\n", 0), P_FREE_JOIN);
				print(STDOUT_FILENO, "SYNOPSYS\n", P_JOIN);
				print(STDOUT_FILENO, ft_strjoin_sep("    ", syntax, "\n\n", 0), P_FREE_JOIN);
				print(STDOUT_FILENO, "DESCRIPTION\n", P_JOIN);
				print(STDOUT_FILENO, ft_strjoin_sep("    ", description, "\n\n", 0), P_FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin(msg, "\n\n", 0), P_FREE_JOIN);
				print(STDOUT_FILENO, "SEE ALSO\n    42sh(1)\n\n", P_JOIN);
			}

			if (!no_print) print(STDOUT_FILENO, NULL, P_PRINT);

			return (0);
		}

	#pragma endregion

	#pragma region "Version"

		static int version() {
			char *msg =
				"set 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by "DEVELOPER" ("LOGIN42").\n";

			print(STDOUT_FILENO, msg, P_RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Set"

	int bt_set(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, "abefhmno.puvxBCEHPTSVXLFN", "abefhmno.puvxBCEHPT", long_opts, "set [-abefhmnpuvxBCEHPTSVXLFN] [-o option-name] [--] [-] [arg ...]", IGNORE_OFF);
		if (!result) return (free_options(result), (shell.error == E_OPT_MAX || shell.error == E_OPT_INVALID) ? 2 : 1);

		if (find_long_option(result, "help"))		return (free_options(result), bt_set_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());


		int ret = 0;
		int positional	= has_option(result, 'S', 0);
		int variable	= has_option(result, 'V', 0);
		int local		= has_option(result, 'L', 0);
		int exported	= has_option(result, 'X', 0);
		int func		= has_option(result, 'F', 0);
		int func_name	= has_option(result, 'N', 0);

		if (positional) {
			positional_params_print(shell.env, -1);
			return (free_options(result), ret);
		}

		if (variable || local || exported) {
			variable_print(shell.env, ((exported) ? VAR_EXPORTED : VAR_NONE), SORT_NORMAL, local, 0);
			if (shell.error == E_NO_MEMORY) ret = exit_error(E_NO_MEMORY, 1, "set", NULL, EE_FREE_NONE, EE_RETURN);
			return (free_options(result), ret);
		}

		if (func || func_name) {
			print(STDOUT_FILENO, "mostrar funciones\n", P_RESET_PRINT);
			// function_print(shell.env, ((exported) ? VAR_EXPORTED : VAR_NONE), SORT_NORMAL, local, func_name);
			if (shell.error == E_NO_MEMORY) ret = exit_error(E_NO_MEMORY, 1, "set", NULL, EE_FREE_NONE, EE_RETURN);
			return (free_options(result), ret);
		}


		for (t_opt_value *curr = result->options; curr; curr = curr->next) {
			int enable = (curr->is_plus) ? 0 : 1;

			if (curr->opt == 'o') {
				if (curr->value) {
					if (option_set(curr->value, enable, O_SET)) {
						if (shell.error == E_SOPT_INVALID) exit_error(E_SOPT_INVALID, 1, "set: ", curr->value, EE_FREE_NONE, EE_RETURN);
						ret = 1;
					}
				} else {
					if (options_print(O_SET, 0, 0)) {
						if (shell.error == E_NO_MEMORY) {
							exit_error(E_NO_MEMORY, 1, "set", NULL, EE_FREE_NONE, EE_RETURN);
							return (free_options(result), 1);
						}
						ret = 1;
					}
				}
			} else {
				if (option_set_char(curr->opt, enable)) {
					if (shell.error == E_SOPT_INVALID) {
						char val[3] = {(curr->is_plus) ? '+' : '-', curr->opt, 0};
						exit_error(E_SOPT_INVALID, 1, "set: ", val, EE_FREE_NONE, EE_RETURN);
					}
					ret = 1;
				}
			}
		}

		if (result->double_dash) {
			if (result->argc)	positional_params_set(shell.env, result->argc, result->argv);
			else				positional_params_clear(shell.env);
		} else if (result->argc && !strcmp(result->argv[0], "-")) {
			if (option_set_char('v', 0)) ret = 1;
			if (option_set_char('x', 0)) ret = 1;
			if (result->argc > 1)	positional_params_set(shell.env, result->argc - 1, &result->argv[1]);
			else					positional_params_clear(shell.env);
		}

		return (free_options(result), ret);
	}

#pragma endregion
