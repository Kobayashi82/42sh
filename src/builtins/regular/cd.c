/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:09:18 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/20 21:13:32 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "builtins/builtins.h"
	#include "builtins/options.h"
	#include "hashes/variable.h"
	#include "main/options.h"
	#include "main/shell.h"
	#include "utils/libft.h"
	#include "utils/paths.h"
	#include "utils/print.h"

	#include "tests/args.h"

#pragma endregion

#pragma region "Help"

	static int print_help() {
		char *msg =
		"cd: cd [-L|[-P] [dir]\n"
		"    Change the shell working directory.\n\n"

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
	
		print(STDOUT_FILENO, msg, RESET_PRINT);

		return (0);
	}

#pragma endregion

#pragma region "Change Dir"

	static int change_dir(char **main_path, t_opt *opts) {
		char *path = ft_strdup(*main_path);
		char *original = *main_path;
		if (*path != '/') {
			char *tmp = ft_strjoin_sep(shell.cwd, "/", path, 3);
			path = resolve_path(tmp); free(tmp);
		} else {
			char *tmp = resolve_path(path); free(path);
			path = tmp;
		}
		
		if (options.cdspell && shell.source == SRC_INTERACTIVE) path = correct_path(path);
		
		if (options.cdable_vars && original && path && access(path, F_OK) == -1) {
			char *var_path = variables_find_value(vars_table, original);
			if (var_path) {
				free(path); path = ft_strdup(var_path);

				if (*path != '/') {
					char *tmp = ft_strjoin_sep(shell.cwd, "/", path, 3);
					path = resolve_path(tmp); free(tmp);
				} else {
					char *tmp = resolve_path(path); free(path);
					path = tmp;
				}

				if (options.cdspell && shell.source == SRC_INTERACTIVE) path = correct_path(path);
			}
		}

		if ((!options.cd_resolve && !*opts->valid) || strchr(opts->valid, 'L')) {
			;
		} else if ((options.cd_resolve && !*opts->valid) || (strchr(opts->valid, 'P'))) {
			char *tmp = ft_strdup(resolve_symlink(path)); free(path);
			path = tmp;
		}

		if (chdir(path)) { free(path); return (1); }
		
		free(*main_path);
		*main_path = path;
		return (0);
	}

#pragma endregion

#pragma region "Check CDPATH"

	int check_CDPATH(char **main_path, t_opt *opts, int *is_dash) {
		char *vars = variables_find_value(vars_table, "CDPATH");
		if (vars) {
			char *token = ft_strtok(vars, ":", 61);

			while (token) {
				if (*token) {
					char *path = NULL;

					if (!strcmp(token, "-")) { *is_dash = 1;
						path = ft_strdup(variables_find_value(vars_table, "OLDPWD"));
					} else path = ft_strdup(token);
					
					if (path && !change_dir(&path, opts)) {
						free(*main_path);
						*main_path = path;
						return (0);
					}

					free(path);
				}

				token = ft_strtok(NULL, ":", 61);
			}
		} return (1);
	}


#pragma endregion

#pragma region "CD"

	int bt_cd(t_arg *args) {
		t_arg *tmp_arg = args;
		while (tmp_arg && tmp_arg->value) {
			if (!strcmp(tmp_arg->value, "-")) tmp_arg->value[0] = '|';
			tmp_arg = tmp_arg->next;
		}

		t_opt *opts = parse_options(args, "LP", '-', 0);
		
		tmp_arg = args;
		while (tmp_arg && tmp_arg->value) {
			if (!strcmp(tmp_arg->value, "|")) tmp_arg->value[0] = '-';
			tmp_arg = tmp_arg->next;
		}

		if (*opts->invalid) {
			invalid_option("cd", opts->invalid, "[-L|[-P] [dir]");
			return (free(opts), 1);
		}

		if (strchr(opts->valid, '?')) return (free(opts), print_help());
		if (strchr(opts->valid, '#')) return (free(opts), print_version("cd", "1.0"));

		int result = 0;
		char *path = NULL;
		int is_dash = 0;

		if (!opts->args) {
			path = ft_strdup(variables_find_value(vars_table, "HOME"));
			if (!path) { result = 2;
				print(STDERR_FILENO, PROYECTNAME ": cd: HOME not set\n", RESET_PRINT);
			}
		} else if (opts->args->value && *opts->args->value) {
			if (opts->args->next) { result = 2;
				print(STDERR_FILENO, PROYECTNAME ": cd: too many arguments\n", RESET_PRINT);
			} else if (!strcmp(opts->args->value, "-")) { is_dash = 1;
				path = ft_strdup(variables_find_value(vars_table, "OLDPWD"));
				if (!path) { result = 2;
					print(STDERR_FILENO, PROYECTNAME ": cd: OLDPWD not set\n", RESET_PRINT);
				}
			} else path = ft_strdup(opts->args->value);
		}
		
		if (!result && (!path || !*path)) result = 3;
		if (!result) result = change_dir(&path, opts);
		if (result == 1) {
			if (!check_CDPATH(&path, opts, &is_dash)) result = 0;
			else if (access(path, F_OK) != -1 && !is_directory(path))		print(STDERR_FILENO, ft_strjoin_sep(PROYECTNAME ": cd: ", opts->args->value, ": Not a directory\n", 0), FREE_RESET_PRINT);
			else if (access(path, F_OK) != -1 && access(path, X_OK) == -1)	print(STDERR_FILENO, ft_strjoin_sep(PROYECTNAME ": cd: ", opts->args->value, ": Permission denied\n", 0), FREE_RESET_PRINT);
			else															print(STDERR_FILENO, ft_strjoin_sep(PROYECTNAME ": cd: ", opts->args->value, ": No such file or directory\n", 0), FREE_RESET_PRINT);
		}

		if (!result) {
			if (is_dash) print(STDOUT_FILENO, ft_strjoin(path, "\n", 0), FREE_RESET_PRINT);

			t_var *var = variables_find(vars_table, "OLDPWD");
			if (var && var->readonly) {
				print(STDERR_FILENO, PROYECTNAME ": OLDPWD: readonly variable\n", RESET_PRINT);
				result = 1;
			} else variables_add(vars_table, "OLDPWD", shell.cwd, -1, -1, -1, -1);
				
			free(shell.cwd); shell.cwd = ft_strdup(path);
			var = variables_find(vars_table, "PWD");
			if (var && var->readonly) {
				print(STDERR_FILENO, PROYECTNAME ": PWD: readonly variable\n", RESET_PRINT);
				result = 1;
			} else variables_add(vars_table, "PWD", path, -1, -1, -1, -1);
		}

		return (free(path), free(opts), (result != 0));
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
