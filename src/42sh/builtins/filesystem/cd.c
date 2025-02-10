/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:09:18 by vzurera-          #+#    #+#             */
/*   Updated: 2025/02/10 11:11:27 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"
	#include "terminal/print.h"
	#include "parser/args.h"
	#include "builtins/builtins.h"
	#include "builtins/options.h"
	#include "main/shell.h"
	#include "utils/paths.h"

#pragma endregion


#pragma region "Help"

	static int print_help() {
		char *msg =
		"cd: cd [-L|[-P [-e]] [dir]\n"
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
		"      -e        if the -P option is supplied, and the current working\n"
		"                directory cannot be determined successfully, exit with\n"
		"                a non-zero status\n"

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

#pragma region "CD"

	int cd(t_arg *args) {
		t_opt *opts = parse_options(args, "LPe", '-', false);

		if (*opts->invalid) {
			invalid_option("cd", opts->invalid, "[-L|[-P [-e]] [dir]");
			return (sfree(opts), 1);
		}

		if (ft_strchr(opts->valid, '?')) return (sfree(opts), print_help());
		if (ft_strchr(opts->valid, '#')) return (sfree(opts), print_version("cd", "1.0"));

		int result = 0;

		// if (ft_strchr(opts->valid, 'P')) {
		// 	char *cwd = get_cwd("cwd");
		// 	if (!cwd) 		result = 1;
		// 	else {
		// 		print(STDOUT_FILENO, cwd, RESET);
		// 		print(STDOUT_FILENO, "\n", PRINT);
		// 		sfree(cwd);
		// 	}
		// } else {
		// 	if (shell.cwd) {
		// 		print(STDOUT_FILENO, shell.cwd, RESET);
		// 		print(STDOUT_FILENO, "\n", PRINT);
		// 	} else {
		// 		print(STDERR_FILENO, "pwd: no se ha encontrado nada\n", RESET_PRINT);
		// 		result = 1;
		// 	}
		// }

		return (sfree(opts), result);
	}

#pragma endregion




// /home/pollon/
// ├── real_dir/
// │       ├── real_subdir/
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
