/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:12:32 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/20 12:47:56 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "utils/libft.h"
	#include "utils/print.h"
	#include "tests/args.h"
	#include "builtins/builtins.h"
	#include "builtins/options.h"
	#include "hashes/alias.h"
	#include "hashes/cmdp.h"
	#include "hashes/builtin.h"
	#include "utils/paths.h"

#pragma endregion

#pragma region "Help"

	static int print_help() {
		char *msg =
			"type: type [-afptP] name [name ...]\n"
			"    Display information about command type.\n\n"

			"    For each NAME, indicate how it would be interpreted if used as a\n"
			"    command name.\n\n"

			"    Options:\n"
			"      -a        display all locations containing an executable named NAME;\n"
			"                includes aliases, builtins, and functions, if and only if\n"
			"                the `-p' option is not also used\n"
			"      -f        suppress shell function lookup\n"
			"      -P        force a PATH search for each NAME, even if it is an alias,\n"
			"                builtin, or function, and returns the name of the disk file\n"
			"                that would be executed\n"
			"      -p        returns either the name of the disk file that would be executed,\n"
			"                or nothing if `type -t NAME' would not return `file'\n"
			"      -t        output a single word which is one of `alias', `keyword',\n"
			"                `function', `builtin', `file' or `', if NAME is an alias,\n"
			"                shell reserved word, shell function, shell builtin, disk file,\n"
			"                or not found, respectively\n\n"

			"    Arguments:\n"
			"      NAME      Command name to be interpreted.\n\n"

			"    Exit Status:\n"
			"      Returns success if all of the NAMEs are found; fails if any are not found.\n";

		print(STDOUT_FILENO, msg, RESET_PRINT);

		return (0);
	}

#pragma endregion

#pragma region "Check"

	#pragma region "Alias"

		static int check_alias(char *arg, char *opts) {
			if (!arg) return (0);

			if (!strchr(opts, 'P') && strchr(opts, 't') && alias_find(arg)) {
				print(STDOUT_FILENO, "alias\n", JOIN);
				return (1);
			} if (!strchr(opts, 'p') && !strchr(opts, 'P')) {
				t_alias *alias = alias_find(arg);
				if (alias) {
					if (!strchr(opts, 't')) {
						print(STDOUT_FILENO, alias->name, JOIN);
						print(STDOUT_FILENO, ft_strjoin_sep(" is aliased to `", alias->value, "'\n", 0), FREE_JOIN);
					}
					return (1);
				}
			}

			return (0);
		}

	#pragma endregion

	#pragma region "Builtin"

		static int check_builtin(char *arg, char *opts) {
			if (!arg) return (0);

			if ((!opts || !*opts || !strcmp(opts, "t")) && alias_find(arg)) return (0);

			if (!strchr(opts, 'P') && strchr(opts, 't') && builtin_isactive(arg)) {
				print(STDOUT_FILENO, "builtin\n", JOIN);
				return (1);
			} else if (!strchr(opts, 'p') && !strchr(opts, 'P')) {
				t_builtin *builtin = builtin_find(arg);
				if (builtin && !builtin->disabled) {
					if (!strchr(opts, 't')) {
						print(STDOUT_FILENO, builtin->name, JOIN);
						print(STDOUT_FILENO, " is a shell builtin\n", JOIN);
					}
					return (1);
				}
			}

			return (0);
		}

	#pragma endregion

	#pragma region "Function"

		static int check_function(char *arg, char *opts) {
			if (!arg) return (0);
			
			if ((!opts || !*opts || !strcmp(opts, "t")) && (alias_find(arg) || builtin_isactive(arg))) return (0);

			// if (!strchr(opts, 'P') && strchr(opts, 't') && builtin_isactive(arg)) {
			// 	print(STDOUT_FILENO, "builtin\n", JOIN);
			// 	return (1);
			// } else if (!strchr(opts, 'p') && !strchr(opts, 'P')) {
			// 	t_builtin *builtin = builtin_find(arg);
			// 	if (builtin && !builtin->disabled) {
			// 		if (!strchr(opts, 't')) {
			// 			print(STDOUT_FILENO, builtin->name, JOIN);
			// 			print(STDOUT_FILENO, " is a shell builtin\n", JOIN);
			// 		}
			// 		return (1);
			// 	}
			// }

			return (0);
		}

	#pragma endregion

	#pragma region "Command"

		static int check_command(char *arg, char *opts, int is_command) {
			if (!arg) return (0);

			if (!opts || !*opts) {
				if (alias_find(arg) || builtin_isactive(arg)) return (0);
				t_cmdp *cmdp = cmdp_find(arg, 0);
				if (cmdp) {
					if (access(cmdp->path, F_OK) != -1) {
						print(STDOUT_FILENO, cmdp->name, JOIN);
						print(STDOUT_FILENO, ft_strjoin_sep(" is hashed (", cmdp->path, ")\n", 0), FREE_JOIN);
						return (1);
					}
				}

				char *path = path_find_first(arg, NULL);
				if (path) {
					print(STDOUT_FILENO, arg, JOIN);
					print(STDOUT_FILENO, ft_strjoin_sep(" is ", path, "\n", 0), FREE_JOIN);
					return (free(path), 1);
				}

				if (is_command)	print(STDERR_FILENO, ft_strjoin_sep("command: ", arg, ": not found\n", 0), FREE_JOIN);
				else			print(STDERR_FILENO, ft_strjoin_sep("type: ", arg, ": not found\n", 0), FREE_JOIN);
				return (free(path), 0);
			}

			if (!strchr(opts, 'P') && !strchr(opts, 'a') && (alias_find(arg) || builtin_isactive(arg))) return (0);

			char **paths = path_find_all(arg, NULL);
			if ((!paths || !*paths) && strchr(opts, 'a')) return (array_free(paths), 0);
			for (int i = 0; paths && paths[i]; ++i) {
				if (!strchr(opts, 'a') && i > 0) break;

				if (strchr(opts, 't'))
					print(STDOUT_FILENO, "file\n", JOIN);
				else if (strchr(opts, 'P') || strchr(opts, 'p'))
					print(STDOUT_FILENO, ft_strjoin(paths[i], "\n", 0), FREE_JOIN);
				else if (strchr(opts, 'a')) {
					print(STDOUT_FILENO, arg, JOIN);
					print(STDOUT_FILENO, ft_strjoin_sep(" is ", paths[i], "\n", 0), FREE_JOIN);
				}
			}

			if ((!paths || !*paths) && (!opts || !*opts)) {
				print(STDERR_FILENO, ft_strjoin_sep("type: ", arg, ": not found\n", 0), FREE_JOIN);
				return (array_free(paths), 0);
			}

			return (array_free(paths), 1);
		}

	#pragma endregion

#pragma endregion

#pragma region "Type"

	int bt_type(t_arg *args) {
		int is_command = 0;
		if (args && args->extra == 1) { is_command = 1; args->extra = 0; }
		t_opt *opts = parse_options(args, "afptP", '-', 0);

		if (*opts->invalid) {
			invalid_option("type", opts->invalid, "[-afptP] name [name ...]");
			return (free(opts), 1);
		}

		if (strchr(opts->valid, '?')) return (free(opts), print_help());
		if (strchr(opts->valid, '#')) return (free(opts), print_version("type", "1.0"));

		print(STDOUT_FILENO, NULL, RESET);
		print(STDERR_FILENO, NULL, RESET);

		int result = 0;
		while (opts->args) {
			if (opts->args->value) {
				int tmp_result = 0;
				tmp_result += check_alias(opts->args->value, opts->valid);
				tmp_result += check_builtin(opts->args->value, opts->valid);
				tmp_result += check_function(opts->args->value, opts->valid);
				tmp_result += check_command(opts->args->value, opts->valid, is_command);
				if (is_command) {
					if (!tmp_result && result == 0) result = 1;
					if (tmp_result) result = 2;
				} else if (!tmp_result) result = 1;

				if (!tmp_result && strchr(opts->valid, 'a'))
					print(STDERR_FILENO, ft_strjoin_sep("type: ", opts->args->value, ": not found\n", 0), FREE_JOIN);
			}
			opts->args = opts->args->next;
		}
		

		print(STDOUT_FILENO, NULL, PRINT);
		print(STDERR_FILENO, NULL, PRINT);

		if (result == 2) result = 0;
		return (free(opts), result);
	}

#pragma endregion

#pragma region "Information"

	//	-P				No buscar alias, builtins ni funciones, solo en $PATH y devuelve solo la primera ocurrencia o nada

	//	-a				Muestra el tipo:							(muestra todo, echo, por ejemplo muestra que es un builtin y las rutas)	(Si se indica -p y si es un alias, builtin o funcion devuelve nada)
	//		alias		[alias] is aliased to `[value]'
	//		builtin		[builtin] is a shell builtin
	//		cmd			[cmd] is [cmd path]							(muestra todas las rutas donde se encuentre, no solo la primera)
	//		no_valid	42sh: type: [cmd]: not found

	//	-f				No buscar funciones							(solo válido para no_opts y -a)
	//	-p				Devuelve solo la primera ocurrencia o nada	(si es un alias, builtin o funcion devuelve nada)

	//	-t				Indica el tipo de archivo					(si se junta con otras opciones, por cada linea se muestra solo el valor de -t ignorando las otra opcion)
	//		alias		alias
	//		builtin		builtin
	//		cmd			file
	//		no_valid	NOTHING

	//	no opts
	//		alias		[alias] is aliased to `[value]'
	//		builtin		[builtin] is a shell builtin
	//		cmd			[cmd] is hashed ([cmd path]) or [cmd] is [cmd path]
	//		no_valid	42sh: type: [cmd]: not found

	//	Exit Status:
	//		failure if COMMAND is not found

#pragma endregion
