/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getopt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 13:27:08 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/07 16:32:33 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "main/shell.h"
	#include "utils/libft.h"
	#include "utils/print.h"
	#include "utils/getopt.h"

#pragma endregion

#pragma region "Internal"

	#pragma region "Max options"

		static int max_options(t_parse_result *result, int opt_count) {
			if (opt_count >= MAX_OPTIONS) {
				print(STDERR_FILENO, ft_strjoin_sep(shell.arg0, ": ", result->name, 0),                        FREE_RESET);
				print(STDERR_FILENO, ft_strjoin_sep("Too many options (max ", ft_itoa(MAX_OPTIONS), ")\n", 2), FREE_JOIN);
				print(STDERR_FILENO, ft_strjoin(result->name, ": usage: ", 0),                                 FREE_JOIN);
				print(STDERR_FILENO, ft_strjoin(result->usage, "\n", 0),                                       FREE_PRINT);
				result->error = 2;
				return (1);
			}

			return (0);
		}

	#pragma endregion

	#pragma region "Create Opt Value"

		static t_opt_value *create_opt_value(char opt, const char *value, int is_long, const char *long_name) {
			t_opt_value *node = calloc(1, sizeof(t_opt_value));
			if (!node) return (NULL);

			node->opt = opt;
			node->value = ft_strdup(value);
			node->is_long = is_long;
			node->long_name = ft_strdup(long_name);
			node->next = NULL;

			return (node);
		}

	#pragma endregion

	#pragma region "Add Opt Value"

		static void add_opt_value(t_opt_value **list, t_opt_value *node) {
			if (!list || !node) return;

			if (!*list) {
				*list = node;
			} else {
				t_opt_value *curr = *list;
				while (curr->next) curr = curr->next;
				curr->next = node;
			}
		}

	#pragma endregion

	#pragma region "Free Opt List"

		static void free_opt_list(t_opt_value *list) {
			while (list) {
				t_opt_value *next = list->next;
				free(list->value);
				free(list->long_name);
				free(list);
				list = next;
			}
		}

	#pragma endregion

	#pragma region "Get Short Type"

		static int get_short_type(const char *short_opts, char opt) {
			if (!short_opts) return (0);						// Not Required

			const char *pos = strchr(short_opts, opt);
			if (!pos) return (-1);								// Invalid

			if (pos[1] == ':' && pos[2] == ':')	return (2);		// Optional
			if (pos[1] == ':')					return (1);		// Required

			return (0);											// Not Required
		}

	#pragma endregion

	#pragma region "Find Long Opt"

		static const t_long_option *find_long_opt(const t_long_option *long_opts, const char *name, size_t len) {
			if (!long_opts || !name) return (NULL);

			for (int i = 0; long_opts[i].name; ++i) {
				if (!strncmp(long_opts[i].name, name, len) && long_opts[i].name[len] == '\0') return (&long_opts[i]);
			}

			return (NULL);
		}

	#pragma endregion


	#pragma region "Parse Long"

		static int parse_long_option(t_parse_result *result, char **argv, int *idx, int argc, const t_long_option *long_opts) {
			const char	*arg = argv[*idx] + 2;
			const char	*equals = strchr(arg, '=');

			// Find long option
			size_t name_len = (equals) ? (size_t)(equals - arg) : ft_strlen(arg);
			const t_long_option *opt = find_long_opt(long_opts, arg, name_len);
			if (!opt) {
				result->invalid_long = ft_strdup(arg);
				result->error = 1;
				if (!result->silent_mode) {
					print(STDERR_FILENO, ft_strjoin_sep(shell.arg0, ": ", result->name, 0), FREE_RESET);
					print(STDERR_FILENO, ": --: invalid option\n",                          JOIN);
					print(STDERR_FILENO, ft_strjoin(result->name, ": usage: ", 0),          FREE_JOIN);
					print(STDERR_FILENO, ft_strjoin(result->usage, "\n", 0),                FREE_PRINT);
				}
				return (-1);
			}

			char *value = NULL;

			if (opt->has_arg == REQUIRED_ARG) {
				if (equals) {
					// --option=value
					value = ft_strdup(equals + 1);
				} else if (*idx + 1 < argc) {
					// --option value
					(*idx)++;
					value = ft_strdup(argv[*idx]);
				} else {
					// Missing Argument
					result->missing_arg = (opt->short_equiv) ? opt->short_equiv : '-';
					result->invalid_long = ft_strdup(opt->name);
					result->error = 1;
					if (!result->silent_mode) {
						print(STDERR_FILENO, ft_strjoin_sep(shell.arg0, ": ", result->name, 0),                                  FREE_RESET);
						print(STDERR_FILENO, ft_strjoin_sep(": --", result->invalid_long, ": option requires an argument\n", 0), FREE_JOIN);
						print(STDERR_FILENO, ft_strjoin(result->name, ": usage: ", 0),                                           FREE_JOIN);
						print(STDERR_FILENO, ft_strjoin(result->usage, "\n", 0),                                                 FREE_PRINT);
					}
					return (-1);
				}
			} else if (opt->has_arg == OPTIONAL_ARG) {
				if (equals) {
					// --option=value	(If there is no '=', the optional argument is not taken from the next argv)
					value = ft_strdup(equals + 1);
				}
			} else if (opt->has_arg == NO_ARGUMENT) {
				if (equals) {
					result->invalid_long = ft_strdup(arg);
					result->error = 1;
					if (!result->silent_mode) {
						print(STDERR_FILENO, ft_strjoin_sep(shell.arg0, ": ", result->name, 0),                                FREE_RESET);
						print(STDERR_FILENO, ft_strjoin_sep(": --", result->invalid_long, ": doesn't allow an argument\n", 0), FREE_JOIN);
						print(STDERR_FILENO, ft_strjoin(result->name, ": usage: ", 0),                                         FREE_JOIN);
						print(STDERR_FILENO, ft_strjoin(result->usage, "\n", 0),                                               FREE_PRINT);
					}
					return (-1);
				}
			}

			t_opt_value *node = create_opt_value(opt->short_equiv, value, 1, opt->name);
			free(value);

			if (node) add_opt_value(&result->options, node);

			return (0);
		}

	#pragma endregion

	#pragma region "Parse Short"

		static int parse_short_options(t_parse_result *result, char **argv, int *idx, int argc, const char *short_opts, int is_plus, int ignore_mode) {
			const char *arg = argv[*idx] + 1;
			int silent_mode = (is_plus) ? result->silent_mode_plus : result->silent_mode;

			for (int i = 0; arg[i]; i++) {
				char opt = arg[i];
				int arg_type = get_short_type(short_opts, opt);

				if (arg_type == -1) {
					if (ignore_mode == 2) return (2);
					result->invalid_opt = opt;
					result->error = 1;
					if (!silent_mode) {
						char buff[2] = {opt, '\0'};
						print(STDERR_FILENO, ft_strjoin_sep(shell.arg0, ": ", result->name, 0),                    FREE_RESET);
						if (is_plus)	print(STDERR_FILENO, ft_strjoin_sep(": +", buff, ": invalid option\n", 0), FREE_JOIN);
						else			print(STDERR_FILENO, ft_strjoin_sep(": -", buff, ": invalid option\n", 0), FREE_JOIN);
						print(STDERR_FILENO, ft_strjoin(result->name, ": usage: ", 0),                             FREE_JOIN);
						print(STDERR_FILENO, ft_strjoin(result->usage, "\n", 0),                                   FREE_PRINT);
					}
					return (-1);
				}

				char *value = NULL;

				if (arg_type == REQUIRED_ARG) {
					if (arg[i + 1]) {
						// -avalue
						value = ft_strdup(arg + i + 1);
						i = ft_strlen(arg) - 1;
					} else if (*idx + 1 < argc) {
						// -a value
						(*idx)++;
						value = ft_strdup(argv[*idx]);
					} else {
						// Missing Argument
						result->missing_arg = opt;
						result->error = 1;
						if (!silent_mode) {
							char buff[2] = {opt, '\0'};
							print(STDERR_FILENO, ft_strjoin_sep(shell.arg0, ": ", result->name, 0),                 FREE_RESET);
							print(STDERR_FILENO, ft_strjoin_sep(": -", buff, ": option requires an argument\n", 0), FREE_JOIN);
							print(STDERR_FILENO, ft_strjoin(result->name, ": usage: ", 0),                          FREE_JOIN);
							print(STDERR_FILENO, ft_strjoin(result->usage, "\n", 0),                                FREE_PRINT);
						}
						return (-1);
					}
				} else if (arg_type == OPTIONAL_ARG) {
					if (arg[i + 1]) {
						// -avalue		(Don't take the next argv for optional arguments)
						value = ft_strdup(arg + i + 1);
						i = ft_strlen(arg) - 1;
					}
				}

				t_opt_value *node = create_opt_value(opt, value, 0, NULL);
				free(value);

				if (node) {
					if (is_plus)	add_opt_value(&result->plus_options, node);
					else			add_opt_value(&result->options, node);
				}
			}

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Methods"

	#pragma region "Short"

		#pragma region "Find Option"

			t_opt_value *find_option(t_parse_result *result, char opt) {
				if (!result) return (NULL);

				t_opt_value *found = NULL;
				for (t_opt_value *curr = result->options; curr; curr = curr->next) {
					if (curr->opt == opt) found = curr;
				}

				return (found);
			}

		#pragma endregion

		#pragma region "Has Option"

			int has_option(t_parse_result *result, char opt) {
				return (find_option(result, opt) != NULL);
			}

		#pragma endregion

		#pragma region "Get Option Value"

			const char *get_option_value(t_parse_result *result, char opt) {
				t_opt_value *node = find_option(result, opt);
				if (!node) return (NULL);

				return (node->value);
			}

		#pragma endregion

	#pragma endregion

	#pragma region "Short Plus"

		#pragma region "Find Plus Option"

			t_opt_value *find_plus_option(t_parse_result *result, char opt) {
				if (!result) return (NULL);

				for (t_opt_value *curr = result->plus_options; curr; curr = curr->next) {
					if (curr->opt == opt) return (curr);
				}

				return (NULL);
			}

		#pragma endregion

		#pragma region "Has Plus Option"

			int has_plus_option(t_parse_result *result, char opt) {
				if (!result) return (0);

				for (t_opt_value *curr = result->plus_options; curr; curr = curr->next)
					if (curr->opt == opt) return (1);

				return (0);
			}

		#pragma endregion

		#pragma region "Get Plus Option Value"

			const char *get_plus_option_value(t_parse_result *result, char opt) {
				t_opt_value *node = find_plus_option(result, opt);
				if (!node) return (NULL);

				return (node->value);
			}

		#pragma endregion

	#pragma endregion

	#pragma region "Long"

		#pragma region "Find Long Option"

			t_opt_value *find_long_option(t_parse_result *result, const char *name) {
				if (!result || !name) return (NULL);

				for (t_opt_value *curr = result->options; curr; curr = curr->next) {
					if (curr->is_long && curr->long_name && !strcmp(curr->long_name, name)) return (curr);
				}

				return (NULL);
			}

		#pragma endregion

		#pragma region "Has Long Option"

			int has_long_option(t_parse_result *result, const char *name) {
				if (!result || !name) return (0);

				for (t_opt_value *curr = result->options; curr; curr = curr->next) {
					if (curr->is_long && curr->long_name && !strcmp(curr->long_name, name)) return (1);
				}

				return (0);
			}

		#pragma endregion

		#pragma region "Get Long Option Value"

			const char *get_long_option_value(t_parse_result *result, const char *name) {
				t_opt_value *node = find_long_option(result, name);
				if (!node) return (NULL);

				return (node->value);
			}

		#pragma endregion

	#pragma endregion

	#pragma region "Free Options"

		void free_argv_original(t_parse_result *result) {
			if (!result) return;

			for (int i = 0; result->argv_original && result->argv_original[i]; ++i) {
				free(result->argv_original[i]);
			}

			free(result->argv_original);
			result->argv_original = NULL;
		}

		void free_options(t_parse_result *result) {
			if (!result) return;

			free_opt_list(result->options);
			free_opt_list(result->plus_options);

			if (result->argv) {
				for (int i = 0; i < result->argc; i++)
					free(result->argv[i]);
				free(result->argv);
			}

			free(result->invalid_long);
			free(result);
		}

	#pragma endregion

	#pragma region "Parse Options"

		t_parse_result *parse_options(int argc, char **argv, const char *short_opts, const char *short_opts_plus, const t_long_option *long_opts, const char *usage, int ignore_mode) {
			if (!argv || argc < 1) return (NULL);

			t_parse_result *result = calloc(1, sizeof(t_parse_result));
			if (!result) return (NULL);

			char **tmp_args = calloc(argc, sizeof(char*));
			if (!tmp_args) return (free(result), NULL);

			int opt_count = 0;
			int arg_count = 0;
			int done_with_opts = 0;

			if (short_opts && *short_opts == ':') {
				result->silent_mode = 1;
				short_opts++;
			}

			if (short_opts_plus && *short_opts_plus == ':') {
				result->silent_mode_plus = 1;
				short_opts_plus++;
			}

			result->argv_original = argv;
			result->usage = usage;
			result->name = argv[0];
			result->shell_name = shell.arg0;
			for (int i = 1; i < argc; i++) {
				if (done_with_opts) {
					tmp_args[arg_count++] = ft_strdup(argv[i]);
					continue;
				}

				const char *arg = argv[i];

				// End of options '--'
				if (!strcmp(arg, "--")) {
					done_with_opts = 1;
					continue;
				}

				// Ignore -number or +number if ignore_numbers is active
				if (ignore_mode == IGNORE_NUMBER && (arg[0] == '-' || arg[0] == '+') && arg[1] != '\0' && ft_isnum_s(&arg[1])) {
					tmp_args[arg_count++] = ft_strdup(arg);
					done_with_opts = 1;
					continue;
				}

				// Detect "-" or "+" alone (they are arguments, not options)
				if (!strcmp(arg, "-") || !strcmp(arg, "+")) {
					tmp_args[arg_count++] = ft_strdup(arg);
					done_with_opts = 1;
					continue;
				}

				// Long options (--...)
				if (arg[0] == '-' && arg[1] == '-') {
					if (max_options(result, opt_count++)) break;													// Error
					if (parse_long_option(result, argv, &i, argc, long_opts) < 0) break;							// Error
					continue;
				}

				// Options with '-'
				if (arg[0] == '-' && arg[1]) {
					if (max_options(result, opt_count)) break;														// Error (Note: max_option error may trigger even when the option is an argument in ignore_mode = 2)
					int ret = parse_short_options(result, argv, &i, argc, short_opts, 0, ignore_mode);
					if (ret == 0) opt_count++;
					if (ret < 0) break;																				// Error
					if (ret == 2) {
						tmp_args[arg_count++] = ft_strdup(arg);
						done_with_opts = 1;
					}
					continue;
				}

				// Options with '+'
				if (arg[0] == '+' && arg[1]) {
					if (max_options(result, opt_count)) break;														// Error (Note: max_option error may trigger even when the option is an argument in ignore_mode = 2)
					int ret = parse_short_options(result, argv, &i, argc, short_opts_plus, 1, ignore_mode);
					if (ret == 0) opt_count++;
					if (ret < 0) break;																				// Error
					if (ret == 2) {
						tmp_args[arg_count++] = ft_strdup(arg);
						done_with_opts = 1;
					}
					continue;
				}

				// Argument
				tmp_args[arg_count++] = ft_strdup(arg);
				done_with_opts = 1;
			}

			result->argc = arg_count;
			if (arg_count > 0) {
				result->argv = calloc(arg_count + 1, sizeof(char*));
				if (result->argv) {
					for (int i = 0; i < arg_count; i++)
						result->argv[i] = tmp_args[i];
					result->argv[arg_count] = NULL;
				}
			}

			free(tmp_args);
			return (result);
		}

	#pragma endregion

#pragma endregion
