/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 15:57:35 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/06 22:03:08 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "terminal/terminal.h"
	#include "terminal/readinput/termcaps.h"
	#include "terminal/readinput/readinput.h"
	#include "main/options.h"
	#include "main/shell.h"
	#include "utils/libft.h"

	#include <signal.h>
	#include <sys/ioctl.h>

#pragma endregion

#pragma region "Signal to STR"

	const char *signal_to_str(int sig) {
		switch (sig) {
			case 0:			return ("EXIT");
			case SIGHUP:	return ("SIGHUP");
			case SIGINT:	return ("SIGINT");
			case SIGQUIT:	return ("SIGQUIT");
			case SIGILL:	return ("SIGILL");
			case SIGTRAP:	return ("SIGTRAP");
			case SIGABRT:	return ("SIGABRT");
			case SIGBUS:	return ("SIGBUS");
			case SIGFPE:	return ("SIGFPE");
			case SIGKILL:	return ("SIGKILL");
			case SIGUSR1:	return ("SIGUSR1");
			case SIGSEGV:	return ("SIGSEGV");
			case SIGUSR2:	return ("SIGUSR2");
			case SIGPIPE:	return ("SIGPIPE");
			case SIGALRM:	return ("SIGALRM");
			case SIGTERM:	return ("SIGTERM");
			case SIGCHLD:	return ("SIGCHLD");
			case SIGSTKFLT:	return ("SIGSTKFLT");
			case SIGCONT:	return ("SIGCONT");
			case SIGSTOP:	return ("SIGSTOP");
			case SIGTSTP:	return ("SIGTSTP");
			case SIGTTIN:	return ("SIGTTIN");
			case SIGTTOU:	return ("SIGTTOU");
			case SIGURG:	return ("SIGURG");
			case SIGXCPU:	return ("SIGXCPU");
			case SIGXFSZ:	return ("SIGXFSZ");
			case SIGVTALRM:	return ("SIGVTALRM");
			case SIGPROF:	return ("SIGPROF");
			case SIGWINCH:	return ("SIGWINCH");
			case SIGIO:		return ("SIGIO");
			case SIGPWR:	return ("SIGPWR");
			case SIGSYS:	return ("SIGSYS");

			if (sig == SIGRTMIN)	return ("SIGRTMIN");
			if (sig == SIGRTMIN+1)	return ("SIGRTMIN+1");
			if (sig == SIGRTMIN+2)	return ("SIGRTMIN+2");
			if (sig == SIGRTMIN+3)	return ("SIGRTMIN+3");
			if (sig == SIGRTMIN+4)	return ("SIGRTMIN+4");
			if (sig == SIGRTMIN+5)	return ("SIGRTMIN+5");
			if (sig == SIGRTMIN+6)	return ("SIGRTMIN+6");
			if (sig == SIGRTMIN+7)	return ("SIGRTMIN+7");
			if (sig == SIGRTMIN+8)	return ("SIGRTMIN+8");
			if (sig == SIGRTMIN+9)	return ("SIGRTMIN+9");
			if (sig == SIGRTMIN+10)	return ("SIGRTMIN+10");
			if (sig == SIGRTMIN+11)	return ("SIGRTMIN+11");
			if (sig == SIGRTMIN+12)	return ("SIGRTMIN+12");
			if (sig == SIGRTMIN+13)	return ("SIGRTMIN+13");
			if (sig == SIGRTMIN+14)	return ("SIGRTMIN+14");
			if (sig == SIGRTMIN+15)	return ("SIGRTMIN+15");

			if (sig == SIGRTMAX-14)	return ("SIGRTMAX-14");
			if (sig == SIGRTMAX-13)	return ("SIGRTMAX-13");
			if (sig == SIGRTMAX-12)	return ("SIGRTMAX-12");
			if (sig == SIGRTMAX-11)	return ("SIGRTMAX-11");
			if (sig == SIGRTMAX-10)	return ("SIGRTMAX-10");
			if (sig == SIGRTMAX-9)	return ("SIGRTMAX-9");
			if (sig == SIGRTMAX-8)	return ("SIGRTMAX-8");
			if (sig == SIGRTMAX-7)	return ("SIGRTMAX-7");
			if (sig == SIGRTMAX-6)	return ("SIGRTMAX-6");
			if (sig == SIGRTMAX-5)	return ("SIGRTMAX-5");
			if (sig == SIGRTMAX-4)	return ("SIGRTMAX-4");
			if (sig == SIGRTMAX-3)	return ("SIGRTMAX-3");
			if (sig == SIGRTMAX-2)	return ("SIGRTMAX-2");
			if (sig == SIGRTMAX-1)	return ("SIGRTMAX-1");
			if (sig == SIGRTMAX)	return ("SIGRTMAX");
		}

		return (NULL);
	}

#pragma endregion

#pragma region "STR to Signal"

	int str_to_signal(const char *sig) {
		if (!sig) return (-1);

		char *signal = ft_strdup(sig);
		if (!signal) return (-1);
		signal = ft_toupper_s(signal);

		if (!strcmp(signal, "EXIT"))										return (free(signal), 0);
		if (!strcmp(signal, "HUP")		||	!strcmp(signal, "SIGHUP"))		return (free(signal), SIGHUP);
		if (!strcmp(signal, "INT")		||	!strcmp(signal, "SIGINT"))		return (free(signal), SIGINT);
		if (!strcmp(signal, "QUIT")		||	!strcmp(signal, "SIGQUIT"))		return (free(signal), SIGQUIT);
		if (!strcmp(signal, "ILL")		||	!strcmp(signal, "SIGILL"))		return (free(signal), SIGILL);
		if (!strcmp(signal, "TRAP")		||	!strcmp(signal, "SIGTRAP"))		return (free(signal), SIGTRAP);
		if (!strcmp(signal, "ABRT")		||	!strcmp(signal, "SIGABRT"))		return (free(signal), SIGABRT);
		if (!strcmp(signal, "BUS")		||	!strcmp(signal, "SIGBUS"))		return (free(signal), SIGBUS);
		if (!strcmp(signal, "FPE")		||	!strcmp(signal, "SIGFPE"))		return (free(signal), SIGFPE);
		if (!strcmp(signal, "KILL")		||	!strcmp(signal, "SIGKILL"))		return (free(signal), SIGKILL);
		if (!strcmp(signal, "USR1")		||	!strcmp(signal, "SIGUSR1"))		return (free(signal), SIGUSR1);
		if (!strcmp(signal, "SEGV")		||	!strcmp(signal, "SIGSEGV"))		return (free(signal), SIGSEGV);
		if (!strcmp(signal, "USR2")		||	!strcmp(signal, "SIGUSR2"))		return (free(signal), SIGUSR2);
		if (!strcmp(signal, "PIPE")		||	!strcmp(signal, "SIGPIPE"))		return (free(signal), SIGPIPE);
		if (!strcmp(signal, "ALRM")		||	!strcmp(signal, "SIGALRM"))		return (free(signal), SIGALRM);
		if (!strcmp(signal, "TERM")		||	!strcmp(signal, "SIGTERM"))		return (free(signal), SIGTERM);
		if (!strcmp(signal, "STKFLT")	||	!strcmp(signal, "SIGSTKFLT"))	return (free(signal), SIGSTKFLT);
		if (!strcmp(signal, "CHLD")		||	!strcmp(signal, "SIGCHLD"))		return (free(signal), SIGCHLD);
		if (!strcmp(signal, "CONT")		||	!strcmp(signal, "SIGCONT"))		return (free(signal), SIGCONT);
		if (!strcmp(signal, "STOP")		||	!strcmp(signal, "SIGSTOP"))		return (free(signal), SIGSTOP);
		if (!strcmp(signal, "TSTP")		||	!strcmp(signal, "SIGTSTP"))		return (free(signal), SIGTSTP);
		if (!strcmp(signal, "TTIN")		||	!strcmp(signal, "SIGTTIN"))		return (free(signal), SIGTTIN);
		if (!strcmp(signal, "TTOU")		||	!strcmp(signal, "SIGTTOU"))		return (free(signal), SIGTTOU);
		if (!strcmp(signal, "URG")		||	!strcmp(signal, "SIGURG"))		return (free(signal), SIGURG);
		if (!strcmp(signal, "XCPU")		||	!strcmp(signal, "SIGXCPU"))		return (free(signal), SIGXCPU);
		if (!strcmp(signal, "XFSZ")		||	!strcmp(signal, "SIGXFSZ"))		return (free(signal), SIGXFSZ);
		if (!strcmp(signal, "VTALRM")	||	!strcmp(signal, "SIGVTALRM"))	return (free(signal), SIGVTALRM);
		if (!strcmp(signal, "PROF")		||	!strcmp(signal, "SIGPROF"))		return (free(signal), SIGPROF);
		if (!strcmp(signal, "WINCH")	||	!strcmp(signal, "SIGWINCH"))	return (free(signal), SIGWINCH);
		if (!strcmp(signal, "IO")		||	!strcmp(signal, "SIGIO"))		return (free(signal), SIGIO);
		if (!strcmp(signal, "PWR")		||	!strcmp(signal, "SIGPWR"))		return (free(signal), SIGPWR);
		if (!strcmp(signal, "SYS")		||	!strcmp(signal, "SIGSYS"))		return (free(signal), SIGSYS);

		if (!strcmp(signal, "RTMIN")	||	!strcmp(signal, "SIGRTMIN"))	return (free(signal), SIGRTMIN);
		if (!strcmp(signal, "RTMIN+1")	||	!strcmp(signal, "SIGRTMIN+1"))	return (free(signal), SIGRTMIN+1);
		if (!strcmp(signal, "RTMIN+2")	||	!strcmp(signal, "SIGRTMIN+2"))	return (free(signal), SIGRTMIN+2);
		if (!strcmp(signal, "RTMIN+3")	||	!strcmp(signal, "SIGRTMIN+3"))	return (free(signal), SIGRTMIN+3);
		if (!strcmp(signal, "RTMIN+4")	||	!strcmp(signal, "SIGRTMIN+4"))	return (free(signal), SIGRTMIN+4);
		if (!strcmp(signal, "RTMIN+5")	||	!strcmp(signal, "SIGRTMIN+5"))	return (free(signal), SIGRTMIN+5);
		if (!strcmp(signal, "RTMIN+6")	||	!strcmp(signal, "SIGRTMIN+6"))	return (free(signal), SIGRTMIN+6);
		if (!strcmp(signal, "RTMIN+7")	||	!strcmp(signal, "SIGRTMIN+7"))	return (free(signal), SIGRTMIN+7);
		if (!strcmp(signal, "RTMIN+8")	||	!strcmp(signal, "SIGRTMIN+8"))	return (free(signal), SIGRTMIN+8);
		if (!strcmp(signal, "RTMIN+9")	||	!strcmp(signal, "SIGRTMIN+9"))	return (free(signal), SIGRTMIN+9);
		if (!strcmp(signal, "RTMIN+10")	||	!strcmp(signal, "SIGRTMIN+10"))	return (free(signal), SIGRTMIN+10);
		if (!strcmp(signal, "RTMIN+11")	||	!strcmp(signal, "SIGRTMIN+11"))	return (free(signal), SIGRTMIN+11);
		if (!strcmp(signal, "RTMIN+12")	||	!strcmp(signal, "SIGRTMIN+12"))	return (free(signal), SIGRTMIN+12);
		if (!strcmp(signal, "RTMIN+13")	||	!strcmp(signal, "SIGRTMIN+13"))	return (free(signal), SIGRTMIN+13);
		if (!strcmp(signal, "RTMIN+14")	||	!strcmp(signal, "SIGRTMIN+14"))	return (free(signal), SIGRTMIN+14);
		if (!strcmp(signal, "RTMIN+15")	||	!strcmp(signal, "SIGRTMIN+15"))	return (free(signal), SIGRTMIN+15);

		if (!strcmp(signal, "RTMAX-14")	||	!strcmp(signal, "SIGRTMAX-14"))	return (free(signal), SIGRTMAX-14);
		if (!strcmp(signal, "RTMAX-13")	||	!strcmp(signal, "SIGRTMAX-13"))	return (free(signal), SIGRTMAX-13);
		if (!strcmp(signal, "RTMAX-12")	||	!strcmp(signal, "SIGRTMAX-12"))	return (free(signal), SIGRTMAX-12);
		if (!strcmp(signal, "RTMAX-11")	||	!strcmp(signal, "SIGRTMAX-11"))	return (free(signal), SIGRTMAX-11);
		if (!strcmp(signal, "RTMAX-10")	||	!strcmp(signal, "SIGRTMAX-10"))	return (free(signal), SIGRTMAX-10);
		if (!strcmp(signal, "RTMAX-9")	||	!strcmp(signal, "SIGRTMAX-9"))	return (free(signal), SIGRTMAX-9);
		if (!strcmp(signal, "RTMAX-8")	||	!strcmp(signal, "SIGRTMAX-8"))	return (free(signal), SIGRTMAX-8);
		if (!strcmp(signal, "RTMAX-7")	||	!strcmp(signal, "SIGRTMAX-7"))	return (free(signal), SIGRTMAX-7);
		if (!strcmp(signal, "RTMAX-6")	||	!strcmp(signal, "SIGRTMAX-6"))	return (free(signal), SIGRTMAX-6);
		if (!strcmp(signal, "RTMAX-5")	||	!strcmp(signal, "SIGRTMAX-5"))	return (free(signal), SIGRTMAX-5);
		if (!strcmp(signal, "RTMAX-4")	||	!strcmp(signal, "SIGRTMAX-4"))	return (free(signal), SIGRTMAX-4);
		if (!strcmp(signal, "RTMAX-3")	||	!strcmp(signal, "SIGRTMAX-3"))	return (free(signal), SIGRTMAX-3);
		if (!strcmp(signal, "RTMAX-2")	||	!strcmp(signal, "SIGRTMAX-2"))	return (free(signal), SIGRTMAX-2);
		if (!strcmp(signal, "RTMAX-1")	||	!strcmp(signal, "SIGRTMAX-1"))	return (free(signal), SIGRTMAX-1);
		if (!strcmp(signal, "RTMAX")	||	!strcmp(signal, "SIGRTMAX"))	return (free(signal), SIGRTMAX);

		return (free(signal), -1);
	}

#pragma endregion

#pragma region "SIG_INT"

	static void sigint_handler(int sig) {
		shell.exit_code = 128 + sig;
		terminal.signal = sig;
		if (raw_mode) {
			char byte = 3;
			ioctl(STDIN_FILENO, TIOCSTI, &byte);
		}
	}

#pragma endregion

#pragma region "SIG_WINCH"

	static void sigwinch_handler(int sig) {
		(void) sig;
		terminal_initialize();
		if (raw_mode) cursor_get();
	}

#pragma endregion

#pragma region "Set"

	void signals_set() {
		signal(SIGINT, sigint_handler);
		signal(SIGWINCH, sigwinch_handler);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);
		signal(SIGTERM, SIG_IGN);
		terminal.signal = 0;
	}

#pragma endregion

// kill -SIGSEGV $(pgrep 42sh)
// kill -SIGSEGV $(pgrep -f 'valgrind.*42sh')