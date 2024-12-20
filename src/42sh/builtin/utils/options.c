/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 17:28:26 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/19 21:52:19 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Parse

	t_opt *parse_options(t_arg *args, const char *valid_opts, char opt_char, bool no_invalid) {
		t_opt *result = safe_calloc(1, sizeof(t_opt));
		result->options = valid_opts; result->args = args;
		if (!args || !valid_opts || !*valid_opts) return (result);

		int valid_index = 0, invalid_index = 0, done = 0;
		while (args && args->value && args->value[0] == opt_char && !done) {
			if (valid_index >= MAX_OPTIONS - 1 || invalid_index >= MAX_OPTIONS - 1) {
				result->too_many = true;
				args = args->next; break;
			}

			char *arg = args->value;
			if (!ft_strcmp(arg, "--")) { args = args->next; break; }
			else if (opt_char == '-' && !ft_strcmp(arg, "--help"))		{ if (!ft_strchr(result->valid, 'H')) result->valid[valid_index++] = 'H'; }
			else if (opt_char == '-' && !ft_strcmp(arg, "--version"))	{ if (!ft_strchr(result->valid, 'V')) result->valid[valid_index++] = 'V'; }
			else {
				for (int i = 1; arg[i]; ++i) {
					if (valid_index >= MAX_OPTIONS - 1 || invalid_index >= MAX_OPTIONS - 1)	{ result->too_many = true; break; }
					if (ft_strchr(valid_opts, arg[i])) {
						if (!ft_strchr(result->valid, arg[i])) result->valid[valid_index++] = arg[i];
					} else if (no_invalid) {
						done = true; break;
					} else {
						if (!ft_strchr(result->invalid, arg[i])) result->invalid[invalid_index++] = arg[i];
					} 
				}
			} if (!done && !result->too_many) args = args->next;
		}

		result->valid[valid_index] = '\0';
		result->invalid[invalid_index] = '\0';
		result->args = args;

		return (result);
	}

#pragma endregion

#pragma region Invalid

	int invalid_option(char *name, char *opts, char *usage) {
		if (!name) return (0);

		print(STDERR_FILENO, NULL, RESET);
		if (opts) {
			print(STDERR_FILENO, name, JOIN);
			print(STDERR_FILENO, ": -", JOIN);
			print(STDERR_FILENO, opts, JOIN);
			if (ft_strlen(opts) > 1)	print(STDERR_FILENO, ": invalid options\n", JOIN);
			else 						print(STDERR_FILENO, ": invalid option\n", JOIN);
		}
		if (usage) {
			print(STDERR_FILENO, "usage: ", JOIN);
			print(STDERR_FILENO, name, JOIN);
			print(STDERR_FILENO, " ", JOIN);
			print(STDERR_FILENO, usage, JOIN);
			print(STDERR_FILENO, "\n", JOIN);
		}
		print(STDERR_FILENO, "Try '", JOIN);
		print(STDERR_FILENO, name, JOIN);
		print(STDERR_FILENO, " --help' for more information\n", PRINT);

		return (0);
	}

#pragma endregion

#pragma region Version

	int print_version(char *name, char *version) {
		if (!name || !version) return (0);

		print(STDOUT_FILENO, name, RESET);
		print(STDOUT_FILENO, " version ", JOIN);
		print(STDOUT_FILENO, version, JOIN);
		print(STDOUT_FILENO, "\n\n", JOIN);

		print(STDOUT_FILENO, "This is free software: you are free to change and redistribute it.\n", JOIN);
		print(STDOUT_FILENO, "There is NO WARRANTY, to the extent permitted by law.\n\n", JOIN);

		print(STDOUT_FILENO, "Copyright (C) 2025 The "PROYECTNAME" Project.\n\n", JOIN);
		print(STDOUT_FILENO, "Written by "DEVELOPER".\n", PRINT);

		return (0);
	}

#pragma endregion

#pragma region Format for Shell

	char *format_for_shell(const char *value, char quote_type) {
		if (!value || (quote_type != '\'' && quote_type != '\"')) return (NULL);

		size_t length = ft_strlen(value), j = 0;
		char *escaped = safe_malloc(length * 6 + 3);

		if (quote_type == '\'') escaped[j++] = '\'';
		if (quote_type == '\"') escaped[j++] = '\"';

		for (size_t i = 0; i < length; i++) {
			if (quote_type == '\'' && value[i] == '\'') {
				escaped[j++] = '\'';
				escaped[j++] = '\\';
				escaped[j++] = '\'';
				escaped[j++] = '\'';
			} else if (quote_type == '\"' && value[i] == '\"') {
				escaped[j++] = '\\';
				escaped[j++] = '\"';
			} else if (quote_type == '\"' && (value[i] == '$' || value[i] == '`' || value[i] == '\\')) {
				escaped[j++] = '\\';
				escaped[j++] = value[i];
			} else escaped[j++] = value[i];
		}

		if (quote_type == '\'') escaped[j++] = '\'';
		if (quote_type == '\"') escaped[j++] = '\"';
		escaped[j] = '\0';

		return (escaped);
	}

#pragma endregion
