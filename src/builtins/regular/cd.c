/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:09:18 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/10 17:51:29 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "main/shell.h"
	#include "utils/utils.h"
	#include "utils/getopt.h"

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Help"

		int bt_cd_help(int format, int no_print) {
			char *name = "cd";
			char *syntax = "cd [-L|[-P] [dir]";
			char *description = "Change the shell working directory.";
			char *msg =
				"    Change the current directory to DIR.  The default DIR is the value of the\n"
				"    HOME shell variable.\n\n"

				"    The variable CDPATH defines the search path for the directory containing\n"
				"    DIR.  Alternative directory names in CDPATH are separated by a colon (:).\n"
				"    A null directory name is the same as the current directory.  If DIR begins\n"
				"    with a slash (/), then CDPATH is not used.\n\n"

				"    If the directory is not found, and the shell option `cdable_vars' is set,\n"
				"    the word is assumed to be  a variable name.  If that variable has a value,\n"
				"    its value is used for DIR.\n\n"

				"    Options:\n"
				"      -L        force symbolic links to be followed: resolve symbolic\n"
				"                links in DIR after processing instances of `..'\n"
				"      -P        use the physical directory structure without following\n"
				"                symbolic links: resolve symbolic links in DIR before\n"
				"                processing instances of `..'\n"

				"    The default is to follow symbolic links, as if `-L' were specified.\n"
				"    `..' is processed by removing the immediately previous pathname component\n"
				"    back to a slash or the beginning of DIR.\n\n"

				"    Exit Status:\n"
				"      Returns 0 if the directory is changed, and if $PWD is set successfully when\n"
				"      -P is used; non-zero otherwise.\n";

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
				"cd 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by "DEVELOPER" ("LOGIN42").\n";

			print(STDOUT_FILENO, msg, RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Change Dir"

	static int change_dir(t_parse_result *result, char **main_path) {
		char *original = *main_path;
		char *path = ft_strdup(original);

		if (*path != '/') {
			char *tmp = ft_strjoin_sep(shell.cwd, "/", path, 3);
			path = resolve_path(tmp);
			free(tmp);
		} else {
			char *tmp = resolve_path(path);
			free(path);
			path = tmp;
		}

		if (shell.options.cdspell && shell.mode == MD_INTERACTIVE) path = correct_path(path);

		if (shell.options.cdable_vars && original && path && access(path, F_OK) == -1) {
			char *var_path = variable_scalar_value(shell.env, original);
			if (var_path) {
				free(path);
				path = ft_strdup(var_path);

				if (*path != '/') {
					char *tmp = ft_strjoin_sep(shell.cwd, "/", path, 3);
					path = resolve_path(tmp);
					free(tmp);
				} else {
					char *tmp = resolve_path(path);
					free(path);
					path = tmp;
				}

				if (shell.options.cdspell && shell.mode == MD_INTERACTIVE) path = correct_path(path);
			}
		}

		if ((!shell.options.cd_resolve && !result->options) || has_option(result, 'L')) {
			;
		} else if ((shell.options.cd_resolve && !result->options) || has_option(result, 'P')) {
			char *tmp = ft_strdup(resolve_symlink(path));
			free(path);
			path = tmp;
		}

		if (chdir(path)) {
			free(path);
			return (1);
		}

		free(*main_path);
		*main_path = path;
		return (0);
	}

#pragma endregion

#pragma region "Check CDPATH"

	int check_CDPATH(t_parse_result *result, char **main_path, int *is_dash) {
		char *vars = variable_scalar_value(shell.env, "CDPATH");
		if (vars) {
			char *token = ft_strtok(vars, ":", 61);

			while (token) {
				if (*token) {
					char *path = NULL;

					if (!strcmp(token, "-")) {
						*is_dash = 1;
						path = ft_strdup(variable_scalar_value(shell.env, "OLDPWD"));
					} else {
						path = ft_strdup(token);
					}

					if (path && !change_dir(result, &path)) {
						free(*main_path);
						*main_path = path;
						return (0);
					}

					free(path);
				}

				token = ft_strtok(NULL, ":", 61);
			}
		}

		return (1);
	}


#pragma endregion

#pragma region "Cd"

	int bt_cd(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, "LP", NULL, long_opts, "cd [-L|[-P] [dir]", IGNORE_OFF);
		if (!result)		return (1);
		if (result->error)	return (free_options(result), 2);

		if (find_long_option(result, "help"))		return (free_options(result), bt_cd_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());


		int ret = 0;

		char *path = NULL;
		int is_dash = 0;

		if (!result->argc) {
			path = ft_strdup(variable_scalar_value(shell.env, "HOME"));
			if (!path) {
				print(STDERR_FILENO, ft_strjoin(shell.name, ": cd: HOME not set\n", 0), FREE_RESET_PRINT);
				ret = 2;
			}
		} else {
			if (result->argc > 1) {
				print(STDERR_FILENO, ft_strjoin(shell.name, ": cd: too many arguments\n", 0), FREE_RESET_PRINT);
				ret = 2;
			} else if (!strcmp(result->argv[0], "-")) {
				is_dash = 1;
				path = ft_strdup(variable_scalar_value(shell.env, "OLDPWD"));
				if (!path) {
					print(STDERR_FILENO, ft_strjoin(shell.name, ": cd: OLDPWD not set\n", 0), FREE_RESET_PRINT);
					ret = 2;
				}
			} else {
				path = ft_strdup(result->argv[0]);
				if (!path) ret = 3;
			}
		}

		if (!ret) ret = change_dir(result, &path);
		if (ret == 1) {
			if		(!check_CDPATH(result, &path, &is_dash))				ret = 0;
			else if	(access(path, F_OK) != -1 && !is_directory(path)) {
				print(STDERR_FILENO, ft_strjoin(shell.name, ": cd: ", 0),                   FREE_RESET);
				print(STDERR_FILENO, ft_strjoin(result->argv[0], ": Not a directory\n", 0), FREE_PRINT);
			} else if (access(path, F_OK) != -1 && access(path, X_OK) == -1) {
				print(STDERR_FILENO, ft_strjoin(shell.name, ": cd: ", 0),                     FREE_RESET);
				print(STDERR_FILENO, ft_strjoin(result->argv[0], ": Permission denied\n", 0), FREE_PRINT);
			} else {
				print(STDERR_FILENO, ft_strjoin(shell.name, ": cd: ", 0),                             FREE_RESET);
				print(STDERR_FILENO, ft_strjoin(result->argv[0], ": No such file or directory\n", 0), FREE_PRINT);
			}
		}

		if (!ret) {
			if (is_dash) print(STDOUT_FILENO, ft_strjoin(path, "\n", 0), FREE_RESET_PRINT);

			// t_var *var = variable_find(shell.env->table, "OLDPWD");
			// if (var && var->readonly) {
			// 	print(STDERR_FILENO, ft_strjoin(shell.name, ": OLDPWD: readonly variable\n", 0), FREE_RESET_PRINT);
			// 	ret = 1;
			// } else {
			// 	variables_add(shell.env->table, "OLDPWD", shell.cwd, -1, -1, -1, -1);
			// }

			free(shell.cwd);
			shell.cwd = ft_strdup(path);
			// var = variable_find(shell.env->table, "PWD");
			// if (var && var->readonly) {
			// 	print(STDERR_FILENO, ft_strjoin(shell.name, ": PWD: readonly variable\n", 0), FREE_RESET_PRINT);
			// 	ret = 1;
			// } else {
			// 	variables_add(shell.env->table, "PWD", path, -1, -1, -1, -1);
			// }
		}

		free(path);

		return (free_options(result), (ret != 0));
	}

#pragma endregion

#pragma region "Info"

	// /home/pollon/
	// ├── real_dir/
	// │	   ├── real_subdir/
	// ├── symlink -> /home/pollon/real_dir/real_subdir

	// Inicialmente:
	// - symlink apunta a /home/pollon/real_dir/real_subdir.

	// Caso 1: cd symlink
	//   pwd: /home/pollon/symlink

	// Caso 2: cd symlink; cd -L ..    (o cd .. sin el -L)
	//   pwd: /home/pollon
	//   (Usa el directorio lógico: trata symlink como parte del árbol lógico)

	// Caso 3: cd symlink; cd -P ..
	//   pwd: /home/pollon/real_dir
	//   (Usa el directorio físico real: ignora el enlace simbólico y sube desde real_subdir)
	//	Si la opción [-e] se establece y el directorio del nivel inferior no se puede obtener, sale con un codigo diferente a 0 (1 probablemente)

	// - Cambia al directorio anterior ($OLDPWD)

	//	La variable CDPATH es como PATH, pero se usa para buscar rutas relativas desde esas ubicaciones en orden.
	//	Si no se encuentra una ruta válida y la opción 'cdable_vars' está activada, se tratará el argumento como una variable
	//	y si existe, se usará como ruta a seguir (siguiendo el mismo proceso de CDPATH y tal...)

	//	En CDPATH como en PATH si se pone un ':' al principio, al final o dos seguidos entremedio se considera la ruta actual.
	//	Esto también es válido con '.' como ruta.

	//	Exit Status: returns 0 if the directory is changed, and if $PWD is set successfully when -P is used; non-zero otherwise.

#pragma endregion
