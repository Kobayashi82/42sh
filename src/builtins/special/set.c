/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:10:33 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/12 20:51:11 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "main/shell.h"
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

#pragma region "Help"

	int bt_set(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, "abefhmnopuvxBCEHPTSVXLFN", NULL, long_opts, "set [-abefhmnpuvxBCEHPTSVXLFN] [-o option-name] [--] [-] [arg ...]", IGNORE_OFF);
		if (!result)		return (1);

		if (find_long_option(result, "help"))		return (free_options(result), bt_set_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());


		int ret = 0;

		if (!result->argc) {
			print(STDOUT_FILENO, "main\n", P_RESET_PRINT);
		}

		if (result->argv) {
			int add_newline = 0;
			print(STDOUT_FILENO, NULL, P_RESET);
			print(STDERR_FILENO, NULL, P_RESET);
			for (int i = 0; i < result->argc; ++i) {
				t_builtin *builtin = builtin_find(result->argv[i]);
				if (builtin) {
					if		(has_option(result, 'd'))	builtin->help(HELP_DESCRIPTION, 1);
					else if	(has_option(result, 'm')) {
						if (add_newline) print(STDOUT_FILENO, "\n", P_JOIN);
						builtin->help(HELP_MANPAGE, 1);
					} else if	(has_option(result, 's'))	builtin->help(HELP_SYNTAX, 1);
					else {
						if (add_newline) print(STDOUT_FILENO, "\n", P_JOIN);
						builtin->help(HELP_NORMAL, 1);
					}
					add_newline = 1;
				} else {
					print(STDERR_FILENO, shell.name,                                                                            P_JOIN);
					print(STDERR_FILENO, ft_strjoin_sep(": help: no help topics match `", result->argv[i], "'.",  J_FREE_NONE), P_FREE_JOIN);
					print(STDERR_FILENO, ft_strjoin_sep("  Try `help help' or `man -k ", result->argv[i], "'.\n", J_FREE_NONE), P_FREE_JOIN);
				}
			}
			print(STDOUT_FILENO, NULL, P_PRINT);
			print(STDERR_FILENO, NULL, P_PRINT);
		}

		return (free_options(result), ret);
	}

#pragma endregion

//	set -P (set +L)
//	set -L (set +P)

//	set -o emacs
//	set -o vi
//	set -o history;
//	set -o hist_local;

// set -- arg1 arg2 arg3  # Establece $1, $2, $3
// set --                 # Limpia todos los argumentos ($# = 0)
