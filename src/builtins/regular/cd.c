/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:09:18 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/18 16:44:29 by vzurera-         ###   ########.fr       */
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
			char *syntax = "cd [-L | [-P [-e]]] [dir]";
			char *description = "Change the shell working directory.";
			char *msg =
				"    Change the current directory to DIR.  The default DIR is the value of the\n"
				"    HOME shell variable. If DIR is "-", it is converted to $OLDPWD.\n\n"

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
				"      -e        if the -P option is supplied, and the current working\n"
				"                directory cannot be determined successfully, exit with\n"
				"                a non-zero status\n\n"

				"    The default is to follow symbolic links, as if `-L' were specified.\n"
				"    `..' is processed by removing the immediately previous pathname component\n"
				"    back to a slash or the beginning of DIR.\n\n"

				"    Exit Status:\n"
				"      Returns 0 if the directory is changed, and if $PWD is set successfully when\n"
				"      -P is used; non-zero otherwise.\n";

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
				"cd 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by "DEVELOPER" ("LOGIN42").\n";

			print(STDOUT_FILENO, msg, P_RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Change Dir"

	static int change_dir(t_parse_result *result, char *final_path) {
		char *original = final_path;
		char *path = ft_strdup(original);

		if (*path != '/') {
			char *tmp = ft_strjoin_sep(shell.dirs.cwd, "/", path, J_FREE_VAL_3);
			path = resolve_path(tmp);
			free(tmp);
		} else {
			char *tmp = resolve_path(path);
			free(path);
			path = tmp;
		}

		if (shell.options.cdspell && shell.mode == MD_INTERACTIVE) path = correct_path(path);

		if (shell.options.cdable_vars && original && path && access(path, F_OK) == -1) {
			char *var_path = variable_scalar_get(shell.env, original);
			if (var_path) {
				free(path);
				path = ft_strdup(var_path);

				if (*path != '/') {
					char *tmp = ft_strjoin_sep(shell.dirs.cwd, "/", path, J_FREE_VAL_3);
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

		free(final_path);
		final_path = path;
		return (0);
	}

#pragma endregion

#pragma region "Check CDPATH"

	// int check_CDPATH(t_parse_result *result, char **main_path) {
	// 	char *cdpath = variable_scalar_get(shell.env, "CDPATH");
	// 	if (!cdpath) return (1);

	// 	char *start = cdpath;
	// 	char *end;

	// 	while (1) {
	// 		end = strchr(start, ':');
	// 		size_t len = (end) ? (size_t)(end - start) : ft_strlen(start);

	// 		char *token;
	// 		if (!len)	token = ft_strdup(".");
	// 		else		token = ft_strndup(start, len);

	// 		if (token) {			
	// 			if (token && !change_dir(result, &token)) {
	// 				free(*main_path);
	// 				*main_path = token;
	// 				return (0);
	// 			}
	// 			free(token);
	// 		}

	// 		if (!end) break;
	// 		start = end + 1;
	// 	}

	// 	return (1);
	// }

	// char *check_CDPATH(char *path) {
	// 	char *cdpath = variable_scalar_get(shell.env, "CDPATH");
	// 	if (!cdpath) return (NULL);

	// 	char *start = cdpath;
	// 	char *end;

	// 	while (1) {
	// 		end = strchr(start, ':');
	// 		size_t len = (end) ? (size_t)(end - start) : ft_strlen(start);

	// 		char *token;
	// 		if (!len)	token = ft_strdup(".");
	// 		else		token = ft_strndup(start, len);

	// 		if (token) {			
	// 			if (token && !change_dir(result, &token)) {
	// 				free(*main_path);
	// 				*main_path = token;
	// 				return (0);
	// 			}
	// 			free(token);
	// 		}

	// 		if (!end) break;
	// 		start = end + 1;
	// 	}

	// 	return (1);
	// }

#pragma endregion

#pragma region "Cd"

	int bt_cd(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, "LPe", NULL, long_opts, "cd [-L | [-P [-e]]] [dir]", IGNORE_OFF);
		if (!result) return (free_options(result), (errno == E_OPT_MAX || errno == E_OPT_INVALID) ? 2 : 1);

		if (find_long_option(result, "help"))		return (free_options(result), bt_cd_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());


		int ret = 0;
		int no_cdpath = 0;
		int show_path = 0;
		char *path = NULL;

		// Get initial path
		if (result->argc > 1) {
			exit_error(E_CD_ARGS, 1, "cd", NULL, EE_FREE_NONE, EE_RETURN);
			return (free_options(result), 1);
		} else if (!result->argc) {
			path = variable_scalar_get(shell.env, "HOME");
			no_cdpath = 1;
			if (!path) {
				exit_error(E_CD_HOME, 1, "cd", NULL, EE_FREE_NONE, EE_RETURN);
				return (free_options(result), 1);
			}
		} else if (!strcmp(result->argv[0], "-")) {
			path = variable_scalar_get(shell.env, "OLDPWD");
			no_cdpath = 1;
			show_path = 1;
			if (!path) {
				exit_error(E_CD_OLDPWD, 1, "cd", NULL, EE_FREE_NONE, EE_RETURN);
				return (free_options(result), 1);
			}
		} else path = result->argv[0];

		// Set final path ()
		char *final_path = NULL;
		if (no_cdpath) {
			final_path = ft_strdup(path);
			if (!final_path) {
				errno == E_NO_MEMORY;
				exit_error(E_NO_MEMORY, 1, "cd", NULL, EE_FREE_NONE, EE_RETURN);
				return (free_options(result), 1);
			}
		} else {
			final_path = check_CDPATH(path);
			if (!final_path) {
				if (errno == E_NO_MEMORY) {
					exit_error(E_NO_MEMORY, 1, "cd", NULL, EE_FREE_NONE, EE_RETURN);
					return (free_options(result), 1);
				}
				final_path = ft_strdup(path);
			} else show_path = 1;
		}

		// Change path
		if (change_dir(result, &path)) {
			if (access(path, F_OK) != -1 && !is_directory(path)) {
				print(STDERR_FILENO, ft_strjoin(shell.name, ": cd: ",                   J_FREE_NONE), P_FREE_RESET);
				print(STDERR_FILENO, ft_strjoin(result->argv[0], ": Not a directory\n", J_FREE_NONE), P_FREE_PRINT);
			} else if (access(path, F_OK) != -1 && access(path, X_OK) == -1) {
				print(STDERR_FILENO, ft_strjoin(shell.name, ": cd: ",                     J_FREE_NONE), P_FREE_RESET);
				print(STDERR_FILENO, ft_strjoin(result->argv[0], ": Permission denied\n", J_FREE_NONE), P_FREE_PRINT);
			} else {
				print(STDERR_FILENO, ft_strjoin(shell.name, ": cd: ",                             J_FREE_NONE), P_FREE_RESET);
				print(STDERR_FILENO, ft_strjoin(result->argv[0], ": No such file or directory\n", J_FREE_NONE), P_FREE_PRINT);
			}
			free(final_path);
			return (free_options(result), 1);
		}

		// Print path (only if OLDPWD or CDPATH)
		if (show_path) print(STDOUT_FILENO, ft_strjoin(final_path, "\n", J_FREE_NONE), P_FREE_RESET_PRINT);


		// Update OLDPWD
		char ret_var = variable_scalar_set(shell.env->table, "OLDPWD", shell.dirs.cwd, 0, VAR_NONE, 0);
		if (ret_var) {
			if (errno == E_NO_MEMORY) {
				exit_error(E_NO_MEMORY, 1, "cd", NULL, EE_FREE_NONE, EE_RETURN);
				return (free_options(result), 1);
			}
			if (errno == E_VAR_IDENTIFIER) {
				exit_error(E_VAR_IDENTIFIER, 1, "cd: ", "OLDPWD",  EE_FREE_NONE, EE_RETURN);
			}
			if (errno == E_VAR_READONLY) {
				exit_error(E_VAR_READONLY, 1, "cd: ", "OLDPWD",  EE_FREE_NONE, EE_RETURN);
			}
			ret = 1;
		}

		// Update internal CWD
		char *absolute_path = resolve_path(final_path);
		free(final_path);
		if (!absolute_path) {
			errno == E_NO_MEMORY;
			exit_error(E_NO_MEMORY, 1, "cd", NULL, EE_FREE_NONE, EE_RETURN);
			return (free_options(result), 1);
		}
		free(shell.dirs.cwd);
		shell.dirs.cwd = ft_strdup(absolute_path);
		if (!shell.dirs.cwd) {
			errno == E_NO_MEMORY;
			exit_error(E_NO_MEMORY, 1, "cd", NULL, EE_FREE_NONE, EE_RETURN);
			free(absolute_path);
			return (free_options(result), 1);
		}

		// Update PWD
		ret_var = variable_scalar_set(shell.env->table, "PWD", absolute_path, 0, VAR_NONE, 0);
		if (ret_var) {
			if (errno == E_NO_MEMORY) {
				free(absolute_path);
				exit_error(E_NO_MEMORY, 1, "cd", NULL, EE_FREE_NONE, EE_RETURN);
				return (free_options(result), 1);
			}
			if (errno == E_VAR_IDENTIFIER) {
				exit_error(E_VAR_IDENTIFIER, 1, "cd: ", "OLDPWD",  EE_FREE_NONE, EE_RETURN);
			}
			if (errno == E_VAR_READONLY) {
				exit_error(E_VAR_READONLY, 1, "cd: ", "OLDPWD",  EE_FREE_NONE, EE_RETURN);
			}
			ret = 1;
		}

		free(absolute_path);
		return (free_options(result), ret);
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
