/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   umask.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:08:17 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/27 14:45:36 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "internal/shell.h"
	#include "utils/utils.h"
	#include "utils/getopt.h"

	#include <sys/stat.h>
	#include <ctype.h>

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Help"

		int bt_umask_help(int format, int no_print) {
			char *name = "umask";
			char *syntax = "umask [-p] [-S] [mode]";
			char *description = "Display or set file mode mask.";
			char *msg =
				"    Sets the user file-creation mask to MODE.  If MODE is omitted, prints\n"
				"    the current value of the mask.\n\n"

				"    If MODE begins with a digit, it is interpreted as an octal number;\n"
				"    otherwise it is a symbolic mode string like that accepted by chmod(1).\n\n"

				"    Options:\n"
				"      -p        if MODE is omitted, output in a form that may be reused as input\n"
				"      -S        makes the output symbolic; otherwise an octal number is output\n\n"

				"    Exit Status:\n"
				"      Returns success unless MODE is invalid or an invalid option is given.\n";

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
				"umask 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by "DEVELOPER" ("LOGIN42").\n";

			print(STDOUT_FILENO, msg, P_RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Symbolic"

	#pragma region "Part"

		int parse_symbolic_part_umask(const char *part, mode_t *umask_val, int uses_equal) {
			mode_t	who_mask = 0;
			mode_t	perm_bits = 0;
			char	op = '\0';
			int		i = 0;

			// Parse "who" (u, g, o, a)
			while (part[i] && part[i] != '=' && part[i] != '+' && part[i] != '-') {
				if		(part[i] == 'u')	who_mask |= 0700;
				else if (part[i] == 'g')	who_mask |= 0070;
				else if (part[i] == 'o')	who_mask |= 0007;
				else if (part[i] == 'a')	who_mask = 0777;
				else						return (1);
				i++;
			}

			// If no "who" specified, default to 'a' (all)
			if (who_mask == 0) who_mask = 0777;

			// Parse operator
			if (part[i] == '=' || part[i] == '+' || part[i] == '-') op = part[i++];
			else return (1);

			// Parse permissions (r, w, x)
			while (part[i]) {
				if (part[i] == 'r') {
					if (who_mask & 0700) perm_bits |= S_IRUSR;
					if (who_mask & 0070) perm_bits |= S_IRGRP;
					if (who_mask & 0007) perm_bits |= S_IROTH;
				} else if (part[i] == 'w') {
					if (who_mask & 0700) perm_bits |= S_IWUSR;
					if (who_mask & 0070) perm_bits |= S_IWGRP;
					if (who_mask & 0007) perm_bits |= S_IWOTH;
				} else if (part[i] == 'x') {
					if (who_mask & 0700) perm_bits |= S_IXUSR;
					if (who_mask & 0070) perm_bits |= S_IXGRP;
					if (who_mask & 0007) perm_bits |= S_IXOTH;
				}
				else return (1);
				i++;
			}

			if (uses_equal) {
				mode_t allowed_perms = perm_bits & who_mask;
				mode_t blocked_perms = who_mask & ~allowed_perms;
				*umask_val = (*umask_val & ~who_mask) | blocked_perms;
			} else {
				if		(op == '+')	*umask_val &= ~perm_bits;
				else if (op == '-')	*umask_val |= perm_bits;
			}

			return (0);
		}

	#pragma endregion

	#pragma region "Symbolic"

		int parse_symbolic_umask(const char *str, mode_t *mask, mode_t current_mask) {
			int uses_equal = (strchr(str, '=') != NULL);
			mode_t working_mask = (uses_equal) ? 0 : current_mask;

			char *saveptr;
			char *str_copy = strdup(str);
			if (!str_copy) return (1);

			// Parse comma-separated parts: u=rwx,g=rx,o=
			char *token = strtok_r(str_copy, ",", &saveptr);
			while (token) {
				// Parse each part (who op perms)
				if (parse_symbolic_part_umask(token, &working_mask, uses_equal)) {
					free(str_copy);
					return (1);
				}
				token = strtok_r(NULL, ",", &saveptr);
			}

			*mask = working_mask;
			free(str_copy);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Octal"

	int parse_octal_umask(const char *str, mode_t *mask) {
		char *endptr;
		long val = strtol(str, &endptr, 8);

		// Check for invalid characters or out of range
		if (*endptr != '\0' || val < 0 || val > 0777) return (1);

		*mask = (mode_t)val;

		return (0);
	}

#pragma endregion

#pragma region "Mode"

	int parse_umask_mode(const char *str, mode_t *mask, mode_t current_mask) {
		if (str[0] >= '0' && str[0] <= '7')
			return (parse_octal_umask(str, mask));
		return (parse_symbolic_umask(str, mask, current_mask));
	}

#pragma endregion

#pragma region "Print"

	void print_umask_symbolic(mode_t mask) {
		mode_t perms = ~mask & 0777;

		printf("u=");
		if (perms & S_IRUSR) printf("r");
		if (perms & S_IWUSR) printf("w");
		if (perms & S_IXUSR) printf("x");

		printf(",g=");
		if (perms & S_IRGRP) printf("r");
		if (perms & S_IWGRP) printf("w");
		if (perms & S_IXGRP) printf("x");

		printf(",o=");
		if (perms & S_IROTH) printf("r");
		if (perms & S_IWOTH) printf("w");
		if (perms & S_IXOTH) printf("x");

		printf("\n");
	}

#pragma endregion

#pragma region "Umask"

	int bt_umask(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, "pS", NULL, long_opts, "umask [-p] [-S] [mode]", IGNORE_OFF);
		if (!result) return (free_options(result), (shell.error == E_OPT_MAX || shell.error == E_OPT_INVALID) ? 2 : 1);

		if (find_long_option(result, "help"))		return (free_options(result), bt_umask_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());


		mode_t new_mask;
		mode_t current_mask = umask(0);
		umask(current_mask);

		if (result->argc > 1) {
			exit_error(E_UMASK_ARGS, 1, "umask", NULL, EE_FREE_NONE, EE_RETURN);
			return (free_options(result), 1);
		}

		if (!result->argc) {

			if		(has_option(result, 'S', 0))	print_umask_symbolic(current_mask);
			else if (has_option(result, 'p', 0))	printf("umask %04o\n", current_mask);
			else									printf("%04o\n", current_mask);

			return (free_options(result), 0);
		}

		if (parse_umask_mode(result->argv[0], &new_mask, current_mask)) {
			exit_error(E_UMASK_INVALID, 1, "umask: ", result->argv[0], EE_FREE_NONE, EE_RETURN);
			return (free_options(result), 1);
		}
		
		umask(new_mask);

		return (free_options(result), 0);
	}

#pragma endregion

#pragma region "Test"

	// Ver umask actual
	//
	// umask                              // Formato octal: 0022
	// umask -S                           // Formato simbólico: u=rwx,g=rx,o=rx
	// umask -p                           // Formato reutilizable: umask 0022

	// Establecer umask en formato octal
	//
	// umask 0022                         // Estándar: user=rwx, group/other=rx (sin write)
	// umask 0077                         // Restrictivo: solo user tiene permisos
	// umask 0002                         // Colaborativo: group puede escribir
	// umask 0000                         // Permisivo: todos los permisos (peligroso)
	// umask 0027                         // user=rwx, group=rx, other=nada
	// umask 0007                         // user y group=rwx, other=nada
	// umask 022                          // Sin el 0 inicial también funciona
	// umask 77                           // Igual que 0077

	// Establecer umask en formato simbólico
	//
	// umask u=rwx,g=rx,o=rx              // Equivalente a 0022
	// umask u=rwx,g=rx,o=                // Equivalente a 0027
	// umask u=rwx,go=                    // Equivalente a 0077
	// umask a=rwx                        // Equivalente a 0000 (all = user+group+other)
	// umask u=rwx,g=rwx,o=rx             // Equivalente a 0002
	// umask u=rwx,g=,o=                  // user=rwx, group y other sin permisos

	// Operadores +, -, =
	//
	// umask u=rwx,g=rx,o=rx              // Establece permisos exactos
	// umask g-w                          // Quita write de group (sobre valor actual)
	// umask o-rwx                        // Quita todos los permisos de other
	// umask g+w                          // Añade write a group
	// umask a+r                          // Añade read a todos
	// umask u-x,g-x,o-x                  // Quita execute a todos

	// Casos edge/especiales
	//
	// umask                              // Sin argumentos
	// umask -S -p                        // Ambas opciones
	// umask 9999                         // Valor inválido (fuera de rango)
	// umask 0888                         // Valor inválido (8 no es octal)
	// umask xyz                          // Formato simbólico inválido
	// umask u=rwx extra_arg              // Demasiados argumentos
	// umask --help                       // Si lo soportas
	// umask -x                           // Opción inválida
	// umask u=                           // Solo user, sin permisos
	// umask a=                           // Todos sin permisos (0777)

	// Verificar herencia
	//
	// umask 0022                         // Establece en padre
	// umask                              // Verifica valor actual

#pragma endregion