/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 14:04:42 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/28 23:41:00 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "utils/libft.h"
	#include "utils/print.h"
	#include "tests/args.h"
	#include "builtins/builtins.h"
	#include "builtins/options.h"
	#include "hashes/alias.h"
	#include "hashes/builtin.h"
	#include "utils/paths.h"

	#include "tests/tests.h"

#pragma endregion

#pragma region "Help"

	static int print_help() {
		char *msg =
			"command: command [-pVv] command [arg ...]\n"
			"    Execute a simple command or display information about commands.\n\n"

			"    Runs COMMAND with ARGS suppressing shell function lookup, or display\n"
			"    information about the specified COMMANDs.  Can be used to invoke commands\n"
			"    on disk when a function with the same name exists.\n\n"

			"    Options:\n"
			"      -p    use a default value for PATH that is guaranteed to find all of\n"
			"            the standard utilities\n"
			"      -v    print a description of COMMAND similar to the `type' builtin\n"
			"      -V    print a more verbose description of each COMMAND\n\n"

			"    Exit Status:\n"
			"      Returns exit status of COMMAND, or failure if COMMAND is not found.\n";

		print(STDOUT_FILENO, msg, RESET_PRINT);

		return (0);
	}

#pragma endregion

#pragma region "Check"

	#pragma region "Alias"

		static int check_alias(char *arg) {
			if (!arg) return (0);

			t_alias *alias = alias_find(arg);
			if (alias) {
				print(STDOUT_FILENO, alias->name, JOIN);
				print(STDOUT_FILENO, ft_strjoin_sep(" is aliased to `", alias->value, "'\n", 0), FREE_JOIN);
				return (1);
			}

			return (0);
		}

	#pragma endregion

	#pragma region "Builtin"

		static int check_builtin(char *arg) {
			if (!arg) return (0);

			if (alias_find(arg)) return (0);

			t_builtin *builtin = builtin_find(arg);
			if (builtin && !builtin->disabled) {
				print(STDOUT_FILENO, ft_strjoin(builtin->name, "\n", 0), FREE_JOIN);
				return (1);
			}

			return (0);
		}

	#pragma endregion

	#pragma region "Function"

		static int check_function(char *arg) {
			if (!arg) return (0);
			
			if (alias_find(arg) || builtin_isactive(arg)) return (0);

			// t_builtin *builtin = builtin_find(arg);
			// if (builtin && !builtin->disabled) {
			// 	print(STDOUT_FILENO, ft_strjoin(builtin->name, "\n", 0), FREE_JOIN);
			// 	return (1);
			// }

			return (0);
		}

	#pragma endregion

	#pragma region "Command"

		static int check_command(char *arg, char *opts) {
			if (!arg) return (0);

			if (alias_find(arg) || builtin_isactive(arg)) return (0);

			char *path = path_find_first(arg, strchr(opts, 'p') ? PATH : NULL);
			if (path) {
				print(STDOUT_FILENO, ft_strjoin(path, "\n", 0), FREE_JOIN);
				return (free(path), 1);
			}

			return (free(path), 0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Command"

	int command(t_arg *args) {
		t_opt *opts = parse_options(args, "pVv", '-', 0);

		if (*opts->invalid) {
			invalid_option("command", opts->invalid, "[-pVv] command [arg ...]");
			return (free(opts), 1);
		}

		if (strchr(opts->valid, '?')) return (free(opts), print_help());
		if (strchr(opts->valid, '#')) return (free(opts), print_version("command", "1.0"));
		
		if (!opts->args || !opts->args->value) return (free(opts), 0);
		
		if (strchr(opts->valid, 'V')) {
			t_arg *arg = opts->args; arg->extra = 1;
			return (free(opts), type(arg));
		}

		if (strchr(opts->valid, 'v')) {

			print(STDOUT_FILENO, NULL, RESET);
			print(STDERR_FILENO, NULL, RESET);

			int result = 0;
			while (opts->args) {
				if (opts->args->value) {
					int tmp_result = 0;
					tmp_result += check_alias(opts->args->value);
					tmp_result += check_builtin(opts->args->value);
					tmp_result += check_function(opts->args->value);
					tmp_result += check_command(opts->args->value, opts->valid);
					if (!tmp_result && result == 0) result = 1;
					if (tmp_result) result = 2;
				}
				opts->args = opts->args->next;
			}

			print(STDOUT_FILENO, NULL, PRINT);
			//print(STDERR_FILENO, "\n", PRINT);

			if (result == 2) result = 0;
			return (free(opts), result);
		}

		if (opts->args->value) {
			t_alias *alias_cmd = alias_find(opts->args->value);
			t_builtin *builtin_cmd = builtin_find(opts->args->value);
			if (alias_cmd && alias_cmd->value) {
				if (builtin_find(alias_cmd->value)) {
					t_arg *cmd = test_create_args(alias_cmd->value);
					builtin_exec(cmd);
					args_clear(&cmd);
				} else {
					t_arg *cmd = test_create_args(alias_cmd->value);
					//	Ejecutar comando indicando la ruta default dependiendo de '-p'
					char *path = path_find_first(cmd->value, strchr(opts->valid, 'p') ? PATH : NULL);
					if (path) {
						printf("Se ejecuta el comando '%s' con la ruta '%s'\n", cmd->value, path);
						args_clear(&cmd);
						return (free(opts), free(path), 1);
					} args_clear(&cmd);
				}
			} else if (builtin_cmd) {
				builtin_exec(opts->args);
			} else if (check_function(opts->args->value)) {
				printf("Se ejecuta la función '%s()'\n", args->value);
				//exec_func();
			} else {
				//	Ejecutar comando indicando la ruta default dependiendo de '-p'
				char *path = path_find_first(opts->args->value, strchr(opts->valid, 'p') ? PATH : NULL);
				if (path) {
					printf("Se ejecuta el comando '%s' con la ruta '%s'\n", opts->args->value, path);
					return (free(opts), free(path), 1);
				}
			}
		}


		return (free(opts), -1);
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
