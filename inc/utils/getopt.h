/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getopt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 13:26:41 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/07 20:24:40 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include <stddef.h>

	#define MAX_OPTIONS	128

#pragma endregion

#pragma region "Variables"

	#pragma region "Structures"

		enum e_getopt {
			NO_ARGUMENT,
			REQUIRED_ARG,
			OPTIONAL_ARG,
			IGNORE_OFF,
			IGNORE_NUMBER,
			IGNORE_STRING
		};

		typedef struct s_long_option {
			const char			*name;				// Long option name
			int					has_arg;			// NO_ARG, REQUIRED_ARG, OPTIONAL_ARG
			char				short_equiv;		// Short equivalent option (0 if none)
		} t_long_option;

		typedef struct s_opt_value {
			char				opt;				// Option character
			char				*value;				// Option value (NULL if none)
			int					is_long;			// 1 if long option, 0 if short option
			char				*long_name;			// Long option name (NULL if short option)
			struct s_opt_value	*next;
		} t_opt_value;

		typedef struct s_parse_result {
			t_opt_value			*options;			// List of parsed options
			t_opt_value			*plus_options;		// List of parsed '+' options
			char				**argv_original;	// Original argv
			int					argc_original;		// Original argc
			int					argc;				// Number of non-option arguments
			char				**argv;				// Non-option arguments
			const char			*shell_name;		// Shell name (same as shell.name)
			const char			*name;				// Name of the command calling getopt
			const char			*usage;				// Usage syntax of the calling command
			char				*invalid_opt;		// Invalid option (NULL if none)
			char				*invalid_long;		// Invalid long option (NULL if none)
			char				missing_arg;		// Option missing a required argument (0 if none)
			int					error;				// 2 if MAX_OPTIONS, 1 if any error, 0 if no error
			int					silent_mode;		// Suppress error messages for '-' options
			int					silent_mode_plus;	// Suppress error messages for '+' options
		} t_parse_result;

	#pragma endregion

#pragma endregion

#pragma region "Methods"

	// Long
	t_opt_value     *find_long_option(t_parse_result *result, const char *name);
	int             has_long_option(t_parse_result *result, const char *name);
	const char      *get_long_option_value(t_parse_result *result, const char *name);

	// Short
	t_opt_value     *find_option(t_parse_result *result, char opt);
	int             has_option(t_parse_result *result, char opt);
	const char      *get_option_value(t_parse_result *result, char opt);

	// Short Plus
	t_opt_value     *find_plus_option(t_parse_result *result, char opt);
	int             has_plus_option(t_parse_result *result, char opt);
	const char      *get_plus_option_value(t_parse_result *result, char opt);

	// Parsing
	t_parse_result *parse_options(int argc, char **argv, const char *short_opts, const char *short_opts_plus, const t_long_option *long_opts, const char *usage, int ignore_mode);

	// Free
	void			free_argv_original(t_parse_result *result);
	void            free_options(t_parse_result *result);

#pragma endregion
