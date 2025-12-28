/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getopts.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:08:17 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/28 17:36:53 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "hashes/variable.h"
	#include "main/shell.h"
	#include "utils/libft.h"
	#include "utils/print.h"

#pragma endregion

#pragma region "Update Variable"

	static void update_variable(const char *key, const char *value) {
		if (variables_validate(key, NULL, shell.arg0, 0, 1)) return;

		t_var *var = variables_find(vars_table, key);
		if (var && var->readonly) {
			print(STDERR_FILENO, shell.arg0, RESET);
			print(STDERR_FILENO, ft_strjoin_sep(": ", key, ": readonly variable\n", 0), FREE_PRINT);
		} else {
			variables_add(vars_table, key, value, 0, 0, 0, 0);
		}
	}

#pragma endregion

#pragma region "Update OPTIND"

	static void update_optind(int optind) {
		char buf[32];
		snprintf(buf, sizeof(buf), "%d", optind);
		update_variable("OPTIND", buf);
	}

#pragma endregion

#pragma region "Invalid Option"

	static void invalid_option2(char opt, int silent_mode, const char *varname) {
		char opt_str[2] = {opt, '\0'};
		update_variable(varname, "?");

		if (!silent_mode) {
			print(STDERR_FILENO, shell.arg0, RESET);
			print(STDERR_FILENO, ft_strjoin_sep(": illegal option -- ", opt_str, "\n", 0), FREE_PRINT);
			variables_delete(vars_table, "OPTARG");
		} else {
			update_variable("OPTARG", opt_str);
		}
	}

#pragma endregion

#pragma region "Missing Argument"

	static void missing_argument(char opt, const char *varname, int silent_mode, int optind) {
		if (!silent_mode) {
			char opt_str[2] = {opt, '\0'};
			print(STDERR_FILENO, shell.arg0, RESET);
			print(STDERR_FILENO, ft_strjoin_sep(": option requires an argument -- ", opt_str, "\n", 0), FREE_PRINT);
			update_variable(varname, "?");
			variables_delete(vars_table, "OPTARG");
		} else {
			char opt_str[2] = {opt, '\0'};
			update_variable(varname, ":");
			update_variable("OPTARG", opt_str);
		}

		shell.optpos = 1;
		update_optind(optind + 1);
	}

#pragma endregion

#pragma region "GetOpts"

	int bt_getopts(int argc, char **argv) {
		char	*optstring, *varname;
		int		optind, opt_offset = 1, silent_mode = 0;

		// Detect "--"
		if (argc > 1 && !strcmp(argv[1], "--")) opt_offset = 2;

		// Validate options
		if (opt_offset == 1 && argc > 1 && argv[1][0] == '-' && argv[1][1] != '\0') {
			char buf[2] = {argv[1][1], '\0'};
			print(STDERR_FILENO, shell.arg0, RESET);
			print(STDERR_FILENO, ft_strjoin_sep(": getopts: -", buf, ": invalid option\n", 0), FREE_JOIN);
			print(STDERR_FILENO, "getopts: usage: getopts optstring name [arg ...]\n", PRINT);
			return (2);
		}

		// Validate argument count
		if (argc < opt_offset + 2) {
			print(STDERR_FILENO, "getopts: usage: getopts optstring name [arg ...]\n", RESET_PRINT);
			return (2);
		}

		optstring = argv[opt_offset];
		varname = argv[opt_offset + 1];

		// Silent mode if optstring starts with ':'
		if (optstring[0] == ':') optstring += silent_mode = 1;

		// Get current OPTIND
		char *optind_str = variables_find_value(vars_table, "OPTIND");
		optind = optind_str ? atoi(optind_str) : 1;

		// Check if we're done parsing
		if (optind > shell.argc) {
			shell.optpos = 1;
			update_variable(varname, "?");
			variables_delete(vars_table, "OPTARG");
			return (1);
		}

		// Get current argument
		const char *current_arg = shell.argv[optind - 1];

		// Must start with '-' and not be just '-'
		if (current_arg[0] != '-' || current_arg[1] == '\0') {
			shell.optpos = 1;
			update_variable(varname, "?");
			variables_delete(vars_table, "OPTARG");
			return (1);
		}

		// Handle "--" in arguments to parse
		if (current_arg[1] == '-' && current_arg[2] == '\0') {
			shell.optpos = 1;
			update_optind(optind + 1);
			update_variable(varname, "?");
			variables_delete(vars_table, "OPTARG");
			return (1);
		}

		// Get current option
		char opt = current_arg[shell.optpos];
		char *opt_ptr = strchr(optstring, opt);

		// Option not found or is ':'
		if (!opt_ptr || opt == ':') {
			invalid_option2(opt, silent_mode, varname);

			// Move to next character
			shell.optpos++;
			if (current_arg[shell.optpos] == '\0') {
				optind++;
				shell.optpos = 1;
			}

			update_optind(optind);
			return (0);
		}

		if (opt_ptr[1] == ':') {
			// Requires argument
			const char *arg = NULL;
			if (current_arg[shell.optpos + 1] != '\0') {
				// Argument in current arg (-aVALUE)
				arg = &current_arg[shell.optpos + 1];
				optind++;
			} else if (optind < shell.argc) {
				// Argument in next arg (-a VALUE)
				arg = shell.argv[optind];
				optind += 2;
			}

			if (!arg) {
				missing_argument(opt, varname, silent_mode, optind);
				return (0);
			}

			shell.optpos = 1;
			update_variable("OPTARG", arg);
		} else {
			// Doesn't require argument
			shell.optpos++;
			if (current_arg[shell.optpos] == '\0') {
				optind++;
				shell.optpos = 1;
			}
		}

		// Store option in variable
		char opt_str[2] = {opt, '\0'};
		update_variable(varname, opt_str);
		update_optind(optind);

		return (0);
	}

#pragma endregion
