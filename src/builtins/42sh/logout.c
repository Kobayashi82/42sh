/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logout.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 12:07:07 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/29 18:54:30 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "utils/libft.h"
	#include "terminal/terminal.h"
	#include "utils/print.h"
	#include "tests/args.h"
	#include "hashes/builtin.h"
	#include "builtins/options.h"
	#include "main/shell.h"

	#include "parser/parser.h"

#pragma endregion

#pragma region "Help"

	static int print_help() {
		char *msg =
		"logout: logout [n]\n"
		"    Exit a login shell.\n\n"

		"    Exits a login shell with exit status N. Returns an error if not executed\n"
		"    in a login shell.\n";

		print(STDOUT_FILENO, msg, RESET_PRINT);

		return (0);
	}

#pragma endregion

#pragma region "Logout"

	int bt_logout(t_arg *args) {
		t_opt *opts = parse_options_old(args->next, "", '-', 0);

		if (strchr(opts->valid, '?')) return (free(opts), print_help());
		if (strchr(opts->valid, '#')) return (free(opts), print_version("logout", "1.0"));

		int result = 0;

		if (opts->args && opts->args->next) {
			print(STDOUT_FILENO, "logout: too many arguments\n", RESET_PRINT);
			result = 1;
		} else if (opts->args && !ft_isdigit_s(opts->args->value)) {
			print(STDOUT_FILENO, "logout: numeric argument required\n", RESET_PRINT);
			result = 2;
		} else if (opts->args && opts->args->value)
			result = atol(opts->args->value);

		free(opts);
		args_clear(&args);
		exit_error(NOTHING, result, NULL, 1);

		return (result);
	}

#pragma endregion

// int login_shell = 0;
// int interactive_shell = 0; // 

// // Detectar si es login shell (llamado como "-42sh" o si pasaron -l o --login)
// if (argv[0][0] == '-' || has_flag("--login") || has_flag("-l")) login_shell = 1;

// if (login_shell) {
//     source_file("/etc/profile");	// 1º (configuración global del sistema)
// 	// El primero disponible de los siguientes
// 	source_file("~/.42sh_profile");	// 2º (preferido por 42sh)
// 	source_file("~/.42sh_login");	// 2º (archivo alternativo)
// 	source_file("~/.profile");		// 2º (POSIX, compatible con todos los shells)
// } else if (interactive_shell) {
// 	source_file("~/.bashrc");		// 1º (configuración interactiva)
// }

// // Builtin logout:
// int bt_logout() {
//     if (!login_shell) {
//         fprintf(stderr, "logout: not login shell: use 'exit'\n");
//         return (1);
//     }

//     source_file("~/.42sh_logout");	// Ejecutar si existe

// 	// Cleanup
//     exit(0);
// }
