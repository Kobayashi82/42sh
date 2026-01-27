/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 14:04:42 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/27 22:21:21 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "internal/shell.h"
	#include "utils/utils.h"
	#include "utils/getopt.h"

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Help"

		int bt_command_help(int format, int no_print) {
			char *name = "command";
			char *syntax = "command [-pVv] command [arg ...]";
			char *description = "Execute a simple command or display information about commands.";
			char *msg =
			"    Runs COMMAND with ARGS suppressing shell function lookup, or display\n"
			"    information about the specified COMMANDs.  Can be used to invoke commands\n"
			"    on disk when a function with the same name exists.\n\n"

			"    Options:\n"
			"      -p        use a default value for PATH that is guaranteed to find all of\n"
			"                the standard utilities\n"
			"      -v        print a description of COMMAND similar to the `type' builtin\n"
			"      -V        print a more verbose description of each COMMAND\n\n"

			"    Exit Status:\n"
			"      Returns exit status of COMMAND, or failure if COMMAND is not found.\n";

			if (!no_print) print(STDOUT_FILENO, NULL, P_RESET);

				if (format == HELP_SYNTAX) {
					print(STDOUT_FILENO, ft_strjoin(name, ": ",   J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin(syntax, "\n", J_FREE_NONE), P_FREE_JOIN);
				}

				if (format == HELP_DESCRIPTION) {
					print(STDOUT_FILENO, ft_strjoin(name, " - ",       J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin(description, "\n", J_FREE_NONE), P_FREE_JOIN);
				}

				if (format == HELP_NORMAL) {
					print(STDOUT_FILENO, ft_strjoin(name, ": ",                      J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin(syntax, "\n",                    J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin_sep("    ", description, "\n\n", J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin(msg, "\n",                       J_FREE_NONE), P_FREE_JOIN);
				}

				if (format == HELP_MANPAGE) {
					print(STDOUT_FILENO, "NAME\n",                                                 P_JOIN);
					print(STDOUT_FILENO, ft_strjoin_sep("    ", name, " - ",         J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin(description, "\n\n",             J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, "SYNOPSYS\n",                                             P_JOIN);
					print(STDOUT_FILENO, ft_strjoin_sep("    ", syntax, "\n\n",      J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, "DESCRIPTION\n",                                          P_JOIN);
					print(STDOUT_FILENO, ft_strjoin_sep("    ", description, "\n\n", J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, ft_strjoin(msg, "\n\n",                     J_FREE_NONE), P_FREE_JOIN);
					print(STDOUT_FILENO, "SEE ALSO\n    42sh(1)\n\n",                              P_JOIN);
				}

			if (!no_print) print(STDOUT_FILENO, NULL, P_PRINT);

			return (0);
		}

	#pragma endregion

	#pragma region "Version"

		static int version() {
			char *msg =
				"command 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by "DEVELOPER" ("LOGIN42").\n";

			print(STDOUT_FILENO, msg, P_RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Check"

	#pragma region "Alias"

		static int check_alias(char *arg) {
			if (!arg) return (0);

			t_alias *alias = alias_get(arg);
			if (alias) {
				print(STDOUT_FILENO, alias->name, P_JOIN);
				print(STDOUT_FILENO, ft_strjoin_sep(" is aliased to `", alias->value, "'\n", J_FREE_NONE), P_FREE_JOIN);
				return (1);
			}

			return (0);
		}

	#pragma endregion

	#pragma region "Builtin"

		static int check_builtin(char *arg) {
			if (!arg) return (0);

			if (alias_get(arg)) return (0);

			t_builtin *builtin = builtin_find(arg);
			if (builtin && !builtin->disabled) {
				print(STDOUT_FILENO, ft_strjoin(builtin->name, "\n", J_FREE_NONE), P_FREE_JOIN);
				return (1);
			}

			return (0);
		}

	#pragma endregion

	#pragma region "Function"

		static int check_function(char *arg) {
			if (!arg) return (0);
			
			if (alias_get(arg) || builtin_isactive(arg)) return (0);

			// t_builtin *builtin = builtin_find(arg);
			// if (builtin && !builtin->disabled) {
			// 	print(STDOUT_FILENO, ft_strjoin(builtin->name, "\n", 0), P_FREE_JOIN);
			// 	return (1);
			// }

			return (0);
		}

	#pragma endregion

	#pragma region "Command"

		static int check_command(char *arg, int has_p) {
			if (!arg) return (0);

			if (alias_get(arg) || builtin_isactive(arg)) return (0);

			char *path = path_find_first(arg, (has_p) ? PATH : NULL);
			if (path) {
				print(STDOUT_FILENO, ft_strjoin(path, "\n", J_FREE_NONE), P_FREE_JOIN);
				return (free(path), 1);
			}

			return (free(path), 0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Command"

	int bt_command(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, "pVv", NULL, long_opts, "command [-pVv] command [arg ...]", IGNORE_OFF);
		if (!result) return (free_options(result), (shell.error == E_OPT_MAX || shell.error == E_OPT_INVALID) ? 2 : 1);

		if (find_long_option(result, "help"))		return (free_options(result), bt_command_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());


		int ret = 0;

		if (!result->argc) return (free_options(result), 0);

		if (has_option(result, 'V', 0)) {
			// ret = bt_type(result->argc, result->argv);
			return (free_options(result), ret);
		}

		if (has_option(result, 'v', 0)) {
			print(STDOUT_FILENO, NULL, P_RESET);
			print(STDERR_FILENO, NULL, P_RESET);

			for (int i = 0; i < result->argc; ++i) {
				int tmp_ret = 0;
				tmp_ret += check_alias(result->argv[i]);
				tmp_ret += check_builtin(result->argv[i]);
				tmp_ret += check_function(result->argv[i]);
				tmp_ret += check_command(result->argv[i], has_option(result, 'p', 0));
				if (!tmp_ret && ret == 0) ret = 1;
				if (tmp_ret) ret = 2;
			}

			print(STDOUT_FILENO, NULL, P_PRINT);
			//print(STDERR_FILENO, "\n", P_PRINT);

			if (ret == 2) ret = 0;
			return (free_options(result), ret);
		}

		t_alias *alias_cmd = alias_get(result->argv[0]);
		t_builtin *builtin_cmd = builtin_find(result->argv[0]);
		if (alias_cmd && alias_cmd->value) {
			if (builtin_find(alias_cmd->value)) {
				// t_arg *cmd = test_create_args(alias_cmd->value);
				// builtin_exec(cmd);
				// args_clear(&cmd);	// quitar
			} else {
				// t_arg *cmd = test_create_args(alias_cmd->value);
				// char *path = path_find_first(cmd->value, (has_option(result, 'p')) ? PATH : NULL);
				// if (path) {
					// printf("Se ejecuta el comando '%s' con la ruta '%s'\n", cmd->value, path);
					// args_clear(&cmd);
					// free(path);
					// return (free_options(result), 1);
				// }
				// args_clear(&cmd);	// quitar
			}
		} else if (builtin_cmd) {
			// builtin_exec(opts->args);
		} else if (check_function(result->argv[0])) {
			printf("Se ejecuta la función '%s()'\n", result->argv[0]);
			//exec_func();
		} else {
			char *path = path_find_first(result->argv[0], (has_option(result, 'p', 0)) ? PATH : NULL);
			if (path) {
				printf("Se ejecuta el comando '%s' con la ruta '%s'\n", result->argv[0], path);
				free(path);
				return (free_options(result), 1);
			}
		}

		return (free_options(result), -1);	// porque -1 ???
	}

#pragma endregion

#pragma region "Information"

	//	-v				Muestra la primera ocurrencia solo
	//		alias		alias [alias]='[value]'
	//		builtin		[builtin]
	//		cmd			[cmd_path]
	//		no_valid	NOTHING

	//	-V				Muestra el tipo:							(solo muestra la primera ocurrencia)
	//		alias		[alias] is aliased to `[value]'
	//		builtin		[builtin] is a shell builtin
	//		cmd			[cmd] is [cmd path]							(muestra todas las rutas donde se encuentre, no solo la primera)
	//		no_valid	42sh: type: [cmd]: not found

	//	-p				Usa un path interno para buscar el comando (/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin)

	//	no_opts			Ejecuta el comando con los argumentos

	//	Exit Status:
	//		no_opts		Returns exit status of COMMAND
	//		-vV			failure if COMMAND is not found, but if a command is found even if there are commands not found, returns success

#pragma endregion
