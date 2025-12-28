/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ulimit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:08:17 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/28 18:42:15 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "utils/libft.h"
	#include "utils/print.h"

	#include <getopt.h>
	#include <sys/resource.h>

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Help"

		static int help() {
			char *msg =
				"ulimit: ulimit [-SHabcdefiklmnpqrstuvxPRT] [limit]\n"
				"    Modify shell resource limits.\n\n"

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

			print(STDOUT_FILENO, msg, RESET_PRINT);

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

#pragma region "Invalid"

	static int invalid() {
		print(STDERR_FILENO, "Try 'ulimit --help' for more information\n", RESET_PRINT);

		return (1);
	}

#pragma endregion

#pragma region "Limits"

	#pragma region "Show Limits"

		static int show_limits(int hard) {
			struct {
				int resource;
				const char *name;
				const char *unit;
				char opt;
				int divisor;
			} limits[] = {
				{RLIMIT_RTTIME,     "real-time non-blocking time", "us",      'R', 1},
				{RLIMIT_CORE,       "core file size",              "blocks",  'c', 512},
				{RLIMIT_DATA,       "data seg size",               "KB",      'd', 1024},
				{RLIMIT_NICE,       "scheduling priority",         "",        'e', 1},
				{RLIMIT_FSIZE,      "file size",                   "blocks",  'f', 512},
				{RLIMIT_SIGPENDING, "pending signals",             "",        'i', 1},
				{RLIMIT_MEMLOCK,    "max locked memory",           "KB",      'l', 1024},
				{RLIMIT_RSS,        "max memory size",             "KB",      'm', 1024},
				{RLIMIT_NOFILE,     "open files",                  "",        'n', 1},
				{RLIMIT_MSGQUEUE,   "POSIX message queues",        "bytes",   'q', 1},
				{RLIMIT_RTPRIO,     "real-time priority",          "",        'r', 1},
				{RLIMIT_STACK,      "stack size",                  "KB",      's', 1024},
				{RLIMIT_CPU,        "cpu time",                    "seconds", 't', 1},
				{RLIMIT_NPROC,      "max user processes",          "",        'u', 1},
				{RLIMIT_AS,         "virtual memory",              "KB",      'v', 1024},
				{RLIMIT_LOCKS,      "file locks",                  "",        'x', 1},
				{-1, NULL, NULL, 0, 0}
			};

			print(STDOUT_FILENO, NULL, RESET);

			for (int i = 0; limits[i].resource != -1; i++) {
				struct rlimit rlim;
				if (getrlimit(limits[i].resource, &rlim) == -1) continue;

				rlim_t value = hard ? rlim.rlim_max : rlim.rlim_cur;

				char header[64];
				snprintf(header, sizeof(header), "(%-c) %-35s ", limits[i].opt, limits[i].name);
				print(STDOUT_FILENO, header, JOIN);

				if (value == RLIM_INFINITY) {
					print(STDOUT_FILENO, "unlimited\n", JOIN);
				} else {
					char buf[64];
					long scaled_value = (long)(value / limits[i].divisor);
					
					if (*limits[i].unit) {
						snprintf(buf, sizeof(buf), "%ld %s\n", scaled_value, limits[i].unit);
					} else {
						snprintf(buf, sizeof(buf), "%ld\n", scaled_value);
					}

					print(STDOUT_FILENO, buf, JOIN);
				}
			}

			print(STDOUT_FILENO, NULL, PRINT);
			return (0);
		}

	#pragma endregion

	#pragma region "Parse Limit"

		// No es -1 RLIM_INFINITY? el error se confundiria?
		static rlim_t parse_limit(const char *str) {
			if (!strcmp(str, "unlimited"))	return (RLIM_INFINITY);
			if (!strcmp(str, "hard"))		return (-2);
			if (!strcmp(str, "soft"))		return (-3);

			char *endptr;
			errno = 0;
			unsigned long val = strtoul(str, &endptr, 10);

			if (errno == ERANGE || val == ULONG_MAX)	return (-1);
			if (endptr == str || *endptr != '\0')		return (-1);
			
			return ((rlim_t)val);
		}

	#pragma endregion

#pragma endregion

#pragma region "Ulimit"

	int bt_ulimit(int argc, char **argv) {
		int		soft = 1, hard = 0;
		int		resource = RLIMIT_FSIZE;
		int		resource_count = 0;
		char	*limit_str = NULL;

		struct option long_options[] = {
			{"help",    no_argument, 0, 'h'},
			{"version", no_argument, 0, 'V'},
			{0, 0, 0, 0}
		};

		int opt;
		optind = opterr = 0;

		while ((opt = getopt_long(argc, argv, "SHacdefilmnqrstuvxRhV", long_options, NULL)) != -1) {
			switch (opt) {
				case 'S': soft = 1; hard = 0; break;
				case 'H': soft = 0; hard = 1; break;
				case 'a': return (show_limits(hard));
				case 'c':
				case 'd':
				case 'e':
				case 'f':
				case 'i':
				case 'l':
				case 'm':
				case 'n':
				case 'q':
				case 'r':
				case 's':
				case 't':
				case 'u':
				case 'v':
				case 'x':
				case 'R':
					resource_count++;
					if (resource_count > 1) {
						print(STDERR_FILENO, "ulimit: cannot specify multiple resources\n", RESET_PRINT);
						return (1);
					}

					if		(opt == 'c') resource = RLIMIT_CORE;		// core file size
					else if (opt == 'd') resource = RLIMIT_DATA;		// data segment
					else if (opt == 'e') resource = RLIMIT_NICE;		// scheduling priority
					else if (opt == 'f') resource = RLIMIT_FSIZE;		// file size
					else if (opt == 'i') resource = RLIMIT_SIGPENDING;	// pending signals
					else if (opt == 'l') resource = RLIMIT_MEMLOCK;		// locked memory
					else if (opt == 'm') resource = RLIMIT_RSS;			// resident set size
					else if (opt == 'n') resource = RLIMIT_NOFILE;		// file descriptors
					else if (opt == 'q') resource = RLIMIT_MSGQUEUE;	// POSIX message queues
					else if (opt == 'r') resource = RLIMIT_RTPRIO;		// real-time priority
					else if (opt == 's') resource = RLIMIT_STACK;		// stack size
					else if (opt == 't') resource = RLIMIT_CPU;			// CPU time
					else if (opt == 'u') resource = RLIMIT_NPROC;		// number of processes
					else if (opt == 'v') resource = RLIMIT_AS;			// virtual memory
					else if (opt == 'x') resource = RLIMIT_LOCKS;		// file locks
					else if (opt == 'R') resource = RLIMIT_RTTIME;		// real-time timeout
					break;
				case '?': {
					char buf[2] = {optopt, '\0'};
					print(STDERR_FILENO, ft_strjoin_sep("ulimit: -", buf, ": invalid option\n", 0), FREE_RESET_PRINT);
					return (invalid());
				}
				case 'h':
					return (help());
				case 'V':
					return (version());
			}
		}

		if (optind < argc) limit_str = argv[optind];

		struct rlimit rlim;
		if (getrlimit(resource, &rlim) == -1) {
			perror("ulimit"); // nop
			return (1);
		}

		if (limit_str) {
			rlim_t new_limit = parse_limit(limit_str);
			if (new_limit == (rlim_t)-1) {
				print(STDERR_FILENO, ft_strjoin_sep("ulimit: ", limit_str, ": invalid number\n", 0), FREE_RESET_PRINT);
				return (1);
			}

			if (soft) rlim.rlim_cur = new_limit;
			if (hard) rlim.rlim_max = new_limit;

			if (setrlimit(resource, &rlim) == -1) {
				perror("ulimit");
				return (1);
			}
		} else {
			rlim_t value = hard ? rlim.rlim_max : rlim.rlim_cur;
			if (value == RLIM_INFINITY)
				print(STDOUT_FILENO, "unlimited\n", RESET_PRINT);
			else {
				char buf[32];
				snprintf(buf, sizeof(buf), "%ld", (long)value);
				print(STDOUT_FILENO, ft_strjoin(buf, "\n", 0), FREE_RESET_PRINT);
			}
		}

		return (0);
	}

#pragma endregion
