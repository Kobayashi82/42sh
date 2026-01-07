/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ulimit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:08:17 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/07 23:49:36 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ulimit -xvalue1 -v -cd value2	(funciona, -x usa value1 y -vcd usan value2)
// ulimit -x value1 -v value2		(no funciona, -x usa value1, -v y value2 se ignoran)

#pragma region "Includes"

	#include "hashes/builtin.h"
	#include "utils/utils.h"
	#include "utils/getopt.h"

	#include <sys/resource.h>

#pragma endregion

#pragma region "Variables"

	static struct {
		int			resource;
		const char	*name;
		const char	*unit;
		char		opt;
		int			divisor;
	} limits[] = {
		{	RLIMIT_RTTIME,		"real-time non-blocking time",	"us",		'R',	1		},
		{	RLIMIT_CORE,		"core file size",				"blocks",	'c',	512		},
		{	RLIMIT_DATA,		"data seg size",				"KB",		'd',	1024	},
		{	RLIMIT_NICE,		"scheduling priority",			"",			'e',	1		},
		{	RLIMIT_FSIZE,		"file size",					"blocks",	'f',	512		},
		{	RLIMIT_SIGPENDING,	"pending signals",				"",			'i',	1		},
		{	RLIMIT_MEMLOCK,		"max locked memory",			"KB",		'l',	1024	},
		{	RLIMIT_RSS,			"max memory size",				"KB",		'm',	1024	},
		{	RLIMIT_NOFILE,		"open files",					"",			'n',	1		},
		{	RLIMIT_MSGQUEUE,	"POSIX message queues",			"bytes",	'q',	1		},
		{	RLIMIT_RTPRIO,		"real-time priority",			"",			'r',	1		},
		{	RLIMIT_STACK,		"stack size",					"KB",		's',	1024	},
		{	RLIMIT_CPU,			"cpu time",						"seconds",	't',	1		},
		{	RLIMIT_NPROC,		"max user processes",			"",			'u',	1		},
		{	RLIMIT_AS,			"virtual memory",				"KB",		'v',	1024	},
		{	RLIMIT_LOCKS,		"file locks",					"",			'x',	1		},
		{	-1, NULL, NULL, 0, 0}
	};

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Help"

		int bt_ulimit_help(int format, int no_print) {
			char *name = "ulimit";
			char *syntax = "ulimit [-SHabcdefiklmnpqrstuvxPRT] [limit]";
			char *description = "Modify shell resource limits.";
			char *msg =
				"    Provides control over the resources available to the shell and processes\n"
				"    it creates, on systems that allow such control.\n\n"

				"    Options:\n"
				"      -S    use the `soft' resource limit\n"
				"      -H    use the `hard' resource limit\n"
				"      -a    all current limits are reported\n"
				"      -c    the maximum size of core files created\n"
				"      -d    the maximum size of a process's data segment\n"
				"      -e    the maximum scheduling priority (`nice')\n"
				"      -f    the maximum size of files written by the shell and its children\n"
				"      -i    the maximum number of pending signals\n"
				"      -l    the maximum size a process may lock into memory\n"
				"      -m    the maximum resident set size\n"
				"      -n    the maximum number of open file descriptors\n"
				"      -q    the maximum number of bytes in POSIX message queues\n"
				"      -r    the maximum real-time scheduling priority\n"
				"      -s    the maximum stack size\n"
				"      -t    the maximum amount of cpu time in seconds\n"
				"      -u    the maximum number of user processes\n"
				"      -v    the size of virtual memory\n"
				"      -x    the maximum number of file locks\n"
				"      -R    the maximum time a real-time process can run before blocking\n\n"

				"    Not all options are available on all platforms.\n\n"

				"    If LIMIT is given, it is the new value of the specified resource; the\n"
				"    special LIMIT values `soft', `hard', and `unlimited' stand for the\n"
				"    current soft limit, the current hard limit, and no limit, respectively.\n"
				"    Otherwise, the current value of the specified resource is printed.\n"
				"    If no option is given, then -f is assumed.\n\n"

				"    Values are in 1024-byte increments, except for -t, which is in seconds,\n"
				"    -p, which is in increments of 512 bytes, and -u, which is an unscaled\n"
				"    number of processes.\n\n"

				"    Exit Status:\n"
				"      Returns success unless an invalid option is supplied or an error occurs.\n";

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
				"ulimit 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by Kobayashi82 (vzurera-).\n";

			print(STDOUT_FILENO, msg, RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Limits"

	#pragma region "Show Limits"

		static int show_limits(int hard, int resource) {
			if (resource != -1) {
				struct rlimit rlim;
				if (getrlimit(resource, &rlim) == -1) return (1);

				rlim_t value = hard ? rlim.rlim_max : rlim.rlim_cur;

				int i = 0;
				for (; limits[i].resource != -1; ++i) {
					if (limits[i].resource == resource) break;
				}

				char header[64];
				snprintf(header, sizeof(header), "(%-c) %-35s ", limits[i].opt, limits[i].name);
				print(STDOUT_FILENO, header, JOIN);

				if (value == RLIM_INFINITY) {
					print(STDOUT_FILENO, "unlimited\n", JOIN);
				} else {
					char buf[64];
					long scaled_value = (long)(value / limits[i].divisor);
					
					if (*limits[i].unit)	snprintf(buf, sizeof(buf), "%ld %s\n", scaled_value, limits[i].unit);
					else					snprintf(buf, sizeof(buf), "%ld\n", scaled_value);
					
					print(STDOUT_FILENO, buf, JOIN);
				}

				return (0);
			}

			print(STDOUT_FILENO, NULL, RESET);

			int ret = 0;

			for (int i = 0; limits[i].resource != -1; ++i) {
				struct rlimit rlim;
				if (getrlimit(limits[i].resource, &rlim) == -1) {
					ret = 1;
					continue;
				}

				rlim_t value = hard ? rlim.rlim_max : rlim.rlim_cur;

				char header[64];
				snprintf(header, sizeof(header), "(%-c) %-35s ", limits[i].opt, limits[i].name);
				print(STDOUT_FILENO, header, JOIN);

				if (value == RLIM_INFINITY) {
					print(STDOUT_FILENO, "unlimited\n", JOIN);
				} else {
					char buf[64];
					long scaled_value = (long)(value / limits[i].divisor);
					
					if (*limits[i].unit)	snprintf(buf, sizeof(buf), "%ld %s\n", scaled_value, limits[i].unit);
					else					snprintf(buf, sizeof(buf), "%ld\n", scaled_value);

					print(STDOUT_FILENO, buf, JOIN);
				}
			}

			print(STDOUT_FILENO, NULL, PRINT);

			return (ret);
		}

	#pragma endregion

	#pragma region "Parse Limit"

		static int get_divisor(int resource) {
			for (int i = 0; limits[i].resource != -1; i++) {
				if (limits[i].resource == resource)
					return limits[i].divisor;
			}
			return 1;
		}

		static rlim_t parse_limit(const char *value, int resource, int *ret) {
			if (!strcmp(value, "unlimited"))				return (RLIM_INFINITY);
			if (!strcmp(value, "hard"))			{ *ret = 2; return (0); }
			if (!strcmp(value, "soft"))			{ *ret = 3; return (0); }

			printf("%s\n", value);

			char *endptr;
			errno = 0;
			unsigned long val = strtoul(value, &endptr, 10);

			if (errno == ERANGE || val == ULONG_MAX)	{ *ret = 1; return (0); };
			if (endptr == value || *endptr != '\0')		{ *ret = 1; return (0); };

			return ((rlim_t)(val * get_divisor(resource)));
		}

	#pragma endregion

#pragma endregion

#pragma region "Process Resource"

	static int process_resource(t_parse_result *result, const char *limit_str, const char *option_str, int hard, int resource) {
		const char *value = (option_str) ? option_str : limit_str;

		struct rlimit rlim;
		if (getrlimit(resource, &rlim) == -1) {
			perror("ulimit"); // nop
			return (1);
		}

		if (value) {
			int ret = 0;
			rlim_t new_limit = parse_limit(value, resource, &ret);
			if		(ret == 3)	new_limit = rlim.rlim_cur;
			else if (ret == 2)	new_limit = rlim.rlim_max;
			else if (ret == 1) {
				print(STDERR_FILENO, result->shell_name,                                           JOIN);
				print(STDERR_FILENO, ft_strjoin_sep(": ulimit: ", value, ": invalid number\n", 0), FREE_JOIN);
				return (1);
			}

			if (hard)	rlim.rlim_max = new_limit;
			else		rlim.rlim_cur = new_limit;

			if (setrlimit(resource, &rlim) == -1) {
				print(STDERR_FILENO, ft_strjoin(result->shell_name, ": ulimit: ", 0), JOIN);

				for (int i = 0; limits[i].resource != -1; i++) {
					if (limits[i].resource == resource) {
						print(STDERR_FILENO, limits[i].name, JOIN);
						break;
					}
				}

				print(STDERR_FILENO, ": cannot modify limit: ", JOIN);

				if (errno == EINVAL && (resource == RLIMIT_RTPRIO || resource == RLIMIT_NICE || resource == RLIMIT_MEMLOCK)) {
					print(STDERR_FILENO, "Operation not permitted\n", JOIN);
				} else {
					print(STDERR_FILENO, ft_strjoin(strerror(errno), "\n", 0), JOIN);
				}

				return (1);
			}
		} else {
			return (show_limits(hard, resource));
		}

		return (0);
	}

#pragma endregion

#pragma region "Ulimit"

	int bt_ulimit(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, "SHac::d::e::f::i::l::m::n::q::r::s::t::u::v::x::R::", NULL, long_opts, "ulimit [-SHabcdefiklmnpqrstuvxPRT] [limit]", IGNORE_OFF);
		if (!result)		return (1);
		if (result->error)	return (free_options(result), 2);

		if (find_long_option(result, "help"))		return (free_options(result), bt_ulimit_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());


		int			ret = 0, hard = 0;
		const char	*limit_str = (result->argc) ? result->argv[0] : NULL;

		if (has_option(result,'H')) hard = 1;
		if (has_option(result,'S')) hard = 0;
		if (has_option(result,'a')) return (free_options(result), show_limits(hard, -1));

		print(STDOUT_FILENO, NULL, RESET);
		print(STDERR_FILENO, NULL, RESET);

		if (!result->options       && process_resource(result, limit_str, NULL, hard, RLIMIT_FSIZE))								ret = 1;	// file size
		if (has_option(result,'c') && process_resource(result, limit_str, get_option_value(result, 'c'), hard, RLIMIT_CORE))		ret = 1;	// core file size
		if (has_option(result,'d') && process_resource(result, limit_str, get_option_value(result, 'd'), hard, RLIMIT_DATA))		ret = 1;	// data segment
		if (has_option(result,'e') && process_resource(result, limit_str, get_option_value(result, 'e'), hard, RLIMIT_NICE))		ret = 1;	// scheduling priority
		if (has_option(result,'f') && process_resource(result, limit_str, get_option_value(result, 'f'), hard, RLIMIT_FSIZE))		ret = 1;	// file size
		if (has_option(result,'i') && process_resource(result, limit_str, get_option_value(result, 'i'), hard, RLIMIT_SIGPENDING))	ret = 1;	// pending signals
		if (has_option(result,'l') && process_resource(result, limit_str, get_option_value(result, 'l'), hard, RLIMIT_MEMLOCK))		ret = 1;	// locked memory
		if (has_option(result,'m') && process_resource(result, limit_str, get_option_value(result, 'm'), hard, RLIMIT_RSS))			ret = 1;	// resident set size
		if (has_option(result,'n') && process_resource(result, limit_str, get_option_value(result, 'n'), hard, RLIMIT_NOFILE))		ret = 1;	// file descriptors
		if (has_option(result,'q') && process_resource(result, limit_str, get_option_value(result, 'q'), hard, RLIMIT_MSGQUEUE))	ret = 1;	// POSIX message queues
		if (has_option(result,'r') && process_resource(result, limit_str, get_option_value(result, 'r'), hard, RLIMIT_RTPRIO))		ret = 1;	// real-time priority
		if (has_option(result,'s') && process_resource(result, limit_str, get_option_value(result, 's'), hard, RLIMIT_STACK))		ret = 1;	// stack size
		if (has_option(result,'t') && process_resource(result, limit_str, get_option_value(result, 't'), hard, RLIMIT_CPU))			ret = 1;	// CPU time
		if (has_option(result,'u') && process_resource(result, limit_str, get_option_value(result, 'u'), hard, RLIMIT_NPROC))		ret = 1;	// number of processes
		if (has_option(result,'v') && process_resource(result, limit_str, get_option_value(result, 'v'), hard, RLIMIT_AS))			ret = 1;	// virtual memory
		if (has_option(result,'x') && process_resource(result, limit_str, get_option_value(result, 'x'), hard, RLIMIT_LOCKS))		ret = 1;	// file locks
		if (has_option(result,'R') && process_resource(result, limit_str, get_option_value(result, 'R'), hard, RLIMIT_RTTIME))		ret = 1;	// real-time timeout

		print(STDOUT_FILENO, NULL, PRINT);
		print(STDERR_FILENO, NULL, PRINT);

		return (free_options(result), ret);
	}

#pragma endregion
