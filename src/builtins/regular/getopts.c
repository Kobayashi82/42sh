/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getopts.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:08:17 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/12 12:27:02 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "main/shell.h"
	#include "utils/utils.h"

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Help"

		static int help() {
			char *msg =
				"getopts: getopts optstring name [arg ...]\n"
				"    Parse option arguments.\n\n"

				"    Getopts is used by shell procedures to parse positional parameters\n"
				"    as options.\n\n"

				"    OPTSTRING contains the option letters to be recognized; if a letter\n"
				"    is followed by a colon, the option is expected to have an argument,\n"
				"    which should be separated from it by white space.\n\n"

				"    Each time it is invoked, getopts will place the next option in the\n"
				"    shell variable $name, initializing name if it does not exist, and\n"
				"    the index of the next argument to be processed into the shell\n"
				"    variable OPTIND.  OPTIND is initialized to 1 each time the shell or\n"
				"    a shell script is invoked.  When an option requires an argument,\n"
				"    getopts places that argument into the shell variable OPTARG.\n\n"

				"    getopts reports errors in one of two ways.  If the first character\n"
				"    of OPTSTRING is a colon, getopts uses silent error reporting.  In\n"
				"    this mode, no error messages are printed.  If an invalid option is\n"
				"    seen, getopts places the option character found into OPTARG.  If a\n"
				"    required argument is not found, getopts places a ':' into NAME and\n"
				"    sets OPTARG to the option character found.  If getopts is not in\n"
				"    silent mode, and an invalid option is seen, getopts places '?' into\n"
				"    NAME and unsets OPTARG.  If a required argument is not found, a '?'\n"
				"    is placed in NAME, OPTARG is unset, and a diagnostic message is\n"
				"    printed.\n\n"

				"    If the shell variable OPTERR has the value 0, getopts disables the\n"
				"    printing of error messages, even if the first character of\n"
				"    OPTSTRING is not a colon.  OPTERR has the value 1 by default.\n\n"

				"    Getopts normally parses the positional parameters, but if arguments\n"
				"    are supplied as ARG values, they are parsed instead.\n\n"

				"    Exit Status:\n"
				"      Returns success if an option is found; fails if the end of options is\n"
				"      encountered or an error occurs.\n";

			print(STDOUT_FILENO, msg, P_RESET_PRINT);

			return (0);
		}

	#pragma endregion

	#pragma region "Version"

		static int version() {
			char *msg =
				"getopts 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by "DEVELOPER" ("LOGIN42").\n";

			print(STDOUT_FILENO, msg, P_RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Update"

	#pragma region "Variable"

		static void update_variable(const char *key, const char *value) {
			if (variable_validate(key)) return;

			(void) value;
			// t_var *var = variable_find(shell.env->table, key);
			// if (var && var->readonly) {
			// 	print(STDERR_FILENO, shell.name, P_RESET);
			// 	print(STDERR_FILENO, ft_strjoin_sep(": ", key, ": readonly variable\n", 0),P_FREE_PRINT);
			// } else {
			// 	variables_add(shell.env->table, key, value, -1, -1, -1, 0);
			// }
		}

	#pragma endregion

	#pragma region "OPTIND"

		static void update_optind(int optind) {
			char buf[32];
			snprintf(buf, sizeof(buf), "%d", optind);
			update_variable("OPTIND", buf);
		}

	#pragma endregion

#pragma endregion

#pragma region "Invalid / Missing Argument"

	#pragma region "Invalid"

		static void invalid(char opt, int silent_mode, const char *varname) {
			char opt_str[2] = {opt, '\0'};
			update_variable(varname, "?");

			if (!silent_mode) {
				print(STDERR_FILENO, shell.name, P_RESET);
				print(STDERR_FILENO, ft_strjoin_sep(": illegal option -- ", opt_str, "\n", 0),P_FREE_PRINT);
				variable_unset(shell.env, "OPTARG", 1);
			} else {
				update_variable("OPTARG", opt_str);
			}
		}

	#pragma endregion

	#pragma region "Missing Argument"

		static void missing_argument(char opt, const char *varname, int silent_mode, int optind) {
			if (!silent_mode) {
				char opt_str[2] = {opt, '\0'};
				print(STDERR_FILENO, shell.name, P_RESET);
				print(STDERR_FILENO, ft_strjoin_sep(": option requires an argument -- ", opt_str, "\n", 0),P_FREE_PRINT);
				update_variable(varname, "?");
				variable_unset(shell.env, "OPTARG", 1);
			} else {
				char opt_str[2] = {opt, '\0'};
				update_variable(varname, ":");
				update_variable("OPTARG", opt_str);
			}

			shell.env->optpos = 1;
			update_optind(optind + 1);
		}

	#pragma endregion

#pragma endregion

#pragma region "GetOpts"

	int bt_getopts(int argc, char **argv) {
		char	*optstring, *varname;
		int		args_count, optind, opt_offset = 1, silent_mode = 0;
		char	**args_to_parse;

		if (!strcmp(argv[1], "--help"))		return (help());
		if (!strcmp(argv[1], "--version"))	return (version());

		// Detect "--"
		if (argc > 1 && !strcmp(argv[1], "--")) opt_offset = 2;

		// Validate options
		if (opt_offset == 1 && argc > 1 && argv[1][0] == '-' && argv[1][1] != '\0') {
			char buf[2] = {argv[1][1], '\0'};
			print(STDERR_FILENO, shell.name, P_RESET);
			print(STDERR_FILENO, ft_strjoin_sep(": getopts: -", buf, ": invalid option\n", 0), P_FREE_JOIN);
			print(STDERR_FILENO, "getopts: usage: getopts optstring name [arg ...]\n",P_PRINT);
			return (2);
		}

		// Validate argument count
		if (argc < opt_offset + 2) {
			print(STDERR_FILENO, "getopts: usage: getopts optstring name [arg ...]\n", P_RESET_PRINT);
			return (2);
		}

		optstring = argv[opt_offset];
		varname = argv[opt_offset + 1];

		// Determine which args to parse
		if (argc > opt_offset + 2) {
			args_to_parse = &argv[opt_offset + 2];
			args_count = argc - (opt_offset + 2);
		} else {
			args_to_parse = shell.env->argv;
			args_count = shell.env->argc;
		}

		// Silent mode if optstring starts with ':' or OPTERR == 0
		if (optstring[0] == ':') optstring += silent_mode = 1;
		if (!silent_mode) {
			char *opterr_str = variable_scalar_get(shell.env, "OPTERR");
			if (opterr_str && atoi(opterr_str) == 0) silent_mode = 1;
		}

		// Get current OPTIND
		char *optind_str = variable_scalar_get(shell.env, "OPTIND");
		optind = optind_str ? atoi(optind_str) : 1;

		// Check if we're done parsing
		if (optind > args_count) {
			shell.env->optpos = 1;
			update_variable(varname, "?");
			variable_unset(shell.env, "OPTARG", 1);
			return (1);
		}

		// Get current argument
		const char *current_arg = args_to_parse[optind - 1];

		// Must start with '-' and not be just '-'
		if (current_arg[0] != '-' || current_arg[1] == '\0') {
			shell.env->optpos = 1;
			update_variable(varname, "?");
			variable_unset(shell.env, "OPTARG", 1);
			return (1);
		}

		// Handle "--" in arguments to parse
		if (current_arg[1] == '-' && current_arg[2] == '\0') {
			shell.env->optpos = 1;
			update_optind(optind + 1);
			update_variable(varname, "?");
			variable_unset(shell.env, "OPTARG", 1);
			return (1);
		}

		// Get current option
		char opt = current_arg[shell.env->optpos];
		char *opt_ptr = strchr(optstring, opt);

		// Option not found or is ':'
		if (!opt_ptr || opt == ':') {
			invalid(opt, silent_mode, varname);

			// Move to next character
			shell.env->optpos++;
			if (current_arg[shell.env->optpos] == '\0') {
				optind++;
				shell.env->optpos = 1;
			}

			update_optind(optind);
			return (0);
		}

		if (opt_ptr[1] == ':') {
			// Requires argument
			const char *arg = NULL;
			if (current_arg[shell.env->optpos + 1] != '\0') {
				// Argument in current arg (-aVALUE)
				arg = &current_arg[shell.env->optpos + 1];
				optind++;
			} else if (optind < args_count) {
				// Argument in next arg (-a VALUE)
				arg = args_to_parse[optind];
				optind += 2;
			}

			if (!arg) {
				missing_argument(opt, varname, silent_mode, optind);
				return (0);
			}

			shell.env->optpos = 1;
			update_variable("OPTARG", arg);
		} else {
			// Doesn't require argument
			shell.env->optpos++;
			if (current_arg[shell.env->optpos] == '\0') {
				optind++;
				shell.env->optpos = 1;
			}
		}

		// Store option in variable
		char opt_str[2] = {opt, '\0'};
		update_variable(varname, opt_str);
		update_optind(optind);

		return (0);
	}

#pragma endregion
