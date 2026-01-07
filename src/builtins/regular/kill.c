/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kill.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:22:34 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/07 16:32:27 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// extern char **environ;
// total space used to store the environment and the arguments to the process is limited to {ARG_MAX} bytes.

#pragma region "Includes"

	#include "terminal/terminal.h"
	#include "hashes/builtin.h"
	#include "utils/libft.h"
	#include "utils/print.h"
	#include "utils/getopt.h"

	#include <signal.h>

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Help"

		int bt_kill_help(int format, int no_print) {
			char *name = "kill";
			char *syntax = "kill [-s sig_spec | -n signum | -sig_spec] pid | job_spec ... or kill -l [sig_spec]";
			char *description = "Send a signal to a job.";
			char *msg =
				"    Send the processes identified by PID or JOB_SPEC the signal named by\n"
				"    SIG_SPEC or SIGNUM.  If neither SIG_SPEC nor SIGNUM is present, then\n"
				"    SIGTERM is assumed.\n\n"

				"    Options:\n"
				"      -s sig    SIG is a signal name\n"
				"      -n sig    SIG is a signal number\n"
				"      -l        list the signal names; if arguments follow `-l' they are\n"
				"                assumed to be signal numbers for which names should be listed\n"
				"      -L        synonym for -l\n\n"

				"    Kill is a shell builtin for two reasons: it allows job IDs to be used\n"
				"    instead of process IDs, and allows processes to be killed if the limit\n"
				"    on processes that you can create is reached.\n\n"

				"    Exit Status:\n"
				"      Returns success unless an invalid option is given or an error occurs.\n";

			if (!no_print) print(STDOUT_FILENO, NULL, RESET);

			if (format == HELP_SYNTAX) {
				print(STDOUT_FILENO, ft_strjoin(name, ": ", 0),   FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin(syntax, "\n", 0), FREE_JOIN);
			}

			if (format == HELP_DESCRIPTION) {
				print(STDOUT_FILENO, ft_strjoin(name, " - ", 0),       FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin(description, "\n", 0), FREE_JOIN);
			}

			if (format == HELP_NORMAL) {
				print(STDOUT_FILENO, ft_strjoin(name, ": ", 0),                      FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin(syntax, "\n", 0),                    FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin_sep("    ", description, "\n\n", 0), FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin(msg, "\n", 0),                       FREE_JOIN);
			}

			if (format == HELP_MANPAGE) {
				print(STDOUT_FILENO, "NAME\n",                                       JOIN);
				print(STDOUT_FILENO, ft_strjoin_sep("    ", name, " - ", 0),         FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin(description, "\n\n", 0),             FREE_JOIN);
				print(STDOUT_FILENO, "SYNOPSYS\n",                                   JOIN);
				print(STDOUT_FILENO, ft_strjoin_sep("    ", syntax, "\n\n", 0),      FREE_JOIN);
				print(STDOUT_FILENO, "DESCRIPTION\n",                                JOIN);
				print(STDOUT_FILENO, ft_strjoin_sep("    ", description, "\n\n", 0), FREE_JOIN);
				print(STDOUT_FILENO, ft_strjoin(msg, "\n\n", 0),                     FREE_JOIN);
				print(STDOUT_FILENO, "SEE ALSO\n    42sh(1)\n\n",                    JOIN);
			}

			if (!no_print) print(STDOUT_FILENO, NULL, PRINT);

			return (0);
		}

	#pragma endregion

	#pragma region "Version"

		static int version() {
			char *msg =
				"kill 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by Kobayashi82 (vzurera-).\n";

			print(STDOUT_FILENO, msg, RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Display Signal"

	static int display_signal(t_parse_result *result) {
		if (!result->argc) {
			char *msg =
				" 1) SIGHUP       2) SIGINT       3) SIGQUIT      4) SIGILL       5) SIGTRAP\n"
				" 6) SIGABRT      7) SIGBUS       8) SIGFPE       9) SIGKILL     10) SIGUSR1\n"
				"11) SIGSEGV     12) SIGUSR2     13) SIGPIPE     14) SIGALRM     15) SIGTERM\n"
				"16) SIGSTKFLT   17) SIGCHLD     18) SIGCONT     19) SIGSTOP     20) SIGTSTP\n"
				"21) SIGTTIN     22) SIGTTOU     23) SIGURG      24) SIGXCPU     25) SIGXFSZ\n"
				"26) SIGVTALRM   27) SIGPROF     28) SIGWINCH    29) SIGIO       30) SIGPWR\n"
				"31) SIGSYS      34) SIGRTMIN    35) SIGRTMIN+1  36) SIGRTMIN+2  37) SIGRTMIN+3\n"
				"38) SIGRTMIN+4  39) SIGRTMIN+5  40) SIGRTMIN+6  41) SIGRTMIN+7  42) SIGRTMIN+8\n"
				"43) SIGRTMIN+9  44) SIGRTMIN+10 45) SIGRTMIN+11 46) SIGRTMIN+12 47) SIGRTMIN+13\n"
				"48) SIGRTMIN+14 49) SIGRTMIN+15 50) SIGRTMAX-14 51) SIGRTMAX-13 52) SIGRTMAX-12\n"
				"53) SIGRTMAX-11 54) SIGRTMAX-10 55) SIGRTMAX-9  56) SIGRTMAX-8  57) SIGRTMAX-7\n"
				"58) SIGRTMAX-6  59) SIGRTMAX-5  60) SIGRTMAX-4  61) SIGRTMAX-3  62) SIGRTMAX-2\n"
				"63) SIGRTMAX-1  64) SIGRTMAX\n";

			print(STDOUT_FILENO, msg, RESET_PRINT);
			return (0);
		}

		int ret = 0;
		int signal = 0;

		print(STDOUT_FILENO, NULL, RESET);
		print(STDERR_FILENO, NULL, RESET);

		for (int i = 0; i < result->argc; ++i) {
			int mode = 0;
			if (result->argv[i][0] != '-' && ft_isdigit_s(result->argv[i])) {
				signal = atoi(result->argv[i]);
				mode = 0;
			} else {
				signal = str_to_signal(result->argv[i]);
				mode = 1;
			}

			if (signal < 0 || signal > 64) {
				print(STDERR_FILENO, ft_strjoin(result->shell_name, ": kill: ", 0),                      FREE_JOIN);
				print(STDERR_FILENO, ft_strjoin(result->argv[i], ": invalid signal specification\n", 0), FREE_JOIN);
				ret = 1;
			} else {
				if (!mode)	print(STDOUT_FILENO, ft_strjoin(signal_to_str(signal), "\n", 0), FREE_JOIN);
				else		print(STDOUT_FILENO, ft_strjoin(ft_itoa(signal), "\n", 1),       FREE_JOIN);
			}
		}

		print(STDOUT_FILENO, NULL, PRINT);
		print(STDERR_FILENO, NULL, PRINT);

		return (ret);
	}

#pragma endregion

#pragma region "Get Signal"

	static int get_signal(t_parse_result *result) {
		int		sig_num = 15;
		char	*failed_signal = NULL;

		for (t_opt_value *curr = result->options; curr; curr = curr->next) {
			if (curr->opt == 's' || curr->opt == 'n') {
				sig_num = 0;
				if (curr->value[0] != '-' && ft_isdigit_s(curr->value))	sig_num = atoi(curr->value);
				else													sig_num = str_to_signal(curr->value);

				if (sig_num < 0 || sig_num > 64) failed_signal = curr->value;
			}
		}

		if (!result->options && result->argc && result->argv[0][0] == '-') {
			char *new_str = &result->argv[0][1];
			if (new_str[0] != '-' && ft_isdigit_s(new_str))	sig_num = atoi(new_str);
			else											sig_num = str_to_signal(new_str);

			if (sig_num < 0 || sig_num > 64) failed_signal = new_str;
			else {
				free(result->argv[0]);
				memmove(result->argv, result->argv + 1, result->argc * sizeof(char *));
				result->argc--;
			}
		}

		if (sig_num < 0 || sig_num > 64) {
			print(STDERR_FILENO, ft_strjoin(result->shell_name, ": kill: ", 0),                    FREE_RESET);
			print(STDERR_FILENO, ft_strjoin(failed_signal, ": invalid signal specification\n", 0), FREE_PRINT);

			return (-1);
		}

		return (sig_num);
	}

#pragma endregion

#pragma region "Send Signal"

	static int send_signal(t_parse_result *result, int sig_num) {
		if (sig_num < 0 || sig_num > 64) return (1);

		if (!result->argc) {
			print(STDERR_FILENO, "kill: usage: kill [-s sig_spec | -n signum | -sig_spec] pid | job_spec ... or kill -l [sig_spec]\n", RESET_PRINT);
			return (2);
		}

		int ret = 0;

		print(STDERR_FILENO, NULL, RESET);

		for (int i = 0; i < result->argc; ++i) {
			int pid = -1;

			if (result->argv[i][0] != '%' && !ft_isdigit_s(result->argv[i])) {
				print(STDERR_FILENO, ft_strjoin(result->shell_name, ": kill: ", 0),                              FREE_JOIN);
				print(STDERR_FILENO, ft_strjoin(result->argv[i], ": arguments must be process or job IDs\n", 0), FREE_JOIN);
				ret = 1;
				continue;
			}

			if (result->argv[i][0] == '%') {
				char *job = NULL;
				if (!job) {
					print(STDERR_FILENO, ft_strjoin(result->shell_name, ": kill: ", 0),     FREE_JOIN);
					print(STDERR_FILENO, ft_strjoin(result->argv[i], ": no such job\n", 0), FREE_JOIN);
					ret = 1;
					continue;
				}
				// pid = job->pid;
			} else {
				pid = atoi(result->argv[i]);
			}

			if (kill(0, sig_num)) {
				print(STDERR_FILENO, ft_strjoin_sep(result->shell_name, ": kill: (", ft_itoa(pid), 3),    FREE_JOIN);
				print(STDERR_FILENO, ft_strjoin_sep(") - : ", strerror(errno), "\n", 0), FREE_JOIN);
			}
		}

		print(STDERR_FILENO, NULL, PRINT);

		return (ret);
	}

#pragma endregion

#pragma region "Kill"

	int bt_kill(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, "Lls:n:", NULL, long_opts, "kill [-s sig_spec | -n signum | -sig_spec] pid | job_spec ... or kill -l [sig_spec]", IGNORE_STRING);
		if (!result)		return (1);
		if (result->error)	return (free_options(result), 2);

		if (find_long_option(result, "help"))		return (free_options(result), bt_kill_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());


		int ret = 0;

		if (has_option(result, 'L') || has_option(result, 'l')) {
			ret = display_signal(result);
			return (free_options(result), ret);
		}

		int sig_num = get_signal(result);
		if (sig_num < 0) return (free_options(result), 1);
		ret = send_signal(result, sig_num);

		return (free_options(result), ret);
	}

#pragma endregion
