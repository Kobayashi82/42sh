/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:12:14 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/09 12:29:26 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "main/shell.h"
	#include "utils/utils.h"
	#include "utils/getopt.h"

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Bracket"

		#pragma region "Help"

			int bt_bracket_help(int format, int no_print) {
				char *name = "[";
				char *syntax = "[ arg... ]";
				char *description = "Evaluate conditional expression.";
				char *msg = 
					"    This is a synonym for the \"test\" builtin, but the last argument must\n"
					"    be a literal `]', to match the opening `['.  See `help test'.\n";

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

			static int version_bracket() {
				char *msg =
					"[ 1.0.\n"
					"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
					"This is free software: you are free to change and redistribute it.\n"
					"There is NO WARRANTY, to the extent permitted by law.\n\n"

					"Written by "DEVELOPER" ("LOGIN42").\n";

				print(STDOUT_FILENO, msg, RESET_PRINT);

				return (0);
			}

		#pragma endregion

	#pragma endregion

	#pragma region "Help"

		int bt_test_help(int format, int no_print) {
			char *name = "test";
			char *syntax = "test [expr]";
			char *description = "Evaluate conditional expression.";
			char *msg =
				"    Exits with a status of 0 (true) or 1 (false) depending on\n"
				"    the evaluation of EXPR.  Expressions may be unary or binary.  Unary\n"
				"    expressions are often used to examine the status of a file.  There\n"
				"    are string operators and numeric comparison operators as well.\n\n"

				"    The behavior of test depends on the number of arguments.  Read the\n"
				"    bash manual page for the complete specification.\n\n"

				"    File operators:\n\n"

				"      -a FILE        True if file exists.\n"
				"      -b FILE        True if file is block special.\n"
				"      -c FILE        True if file is character special.\n"
				"      -d FILE        True if file is a directory.\n"
				"      -e FILE        True if file exists.\n"
				"      -f FILE        True if file exists and is a regular file.\n"
				"      -g FILE        True if file is set-group-id.\n"
				"      -h FILE        True if file is a symbolic link.\n"
				"      -L FILE        True if file is a symbolic link.\n"
				"      -k FILE        True if file has its `sticky' bit set.\n"
				"      -p FILE        True if file is a named pipe.\n"
				"      -r FILE        True if file is readable by you.\n"
				"      -s FILE        True if file exists and is not empty.\n"
				"      -S FILE        True if file is a socket.\n"
				"      -t FD          True if FD is opened on a terminal.\n"
				"      -u FILE        True if the file is set-user-id.\n"
				"      -w FILE        True if the file is writable by you.\n"
				"      -x FILE        True if the file is executable by you.\n"
				"      -O FILE        True if the file is effectively owned by you.\n"
				"      -G FILE        True if the file is effectively owned by your group.\n"
				"      -N FILE        True if the file has been modified since it was last read.\n\n"

				"      FILE1 -nt FILE2  True if file1 is newer than file2 (according to\n"
				"                       modification date).\n\n"

				"      FILE1 -ot FILE2  True if file1 is older than file2.\n\n"

				"      FILE1 -ef FILE2  True if file1 is a hard link to file2.\n\n"

				"    All file operators except -h and -L are acting on the target of a symbolic\n"
				"    link, not on the symlink itself, if FILE is a symbolic link.\n\n"

				"    String operators:\n\n"

				"      -z STRING      True if string is empty.\n\n"

				"      -n STRING\n"
				"         STRING      True if string is not empty.\n\n"

				"      STRING1 = STRING2\n"
				"                     True if the strings are equal.\n"
				"      STRING1 != STRING2\n"
				"                     True if the strings are not equal.\n"
				"      STRING1 < STRING2\n"
				"                     True if STRING1 sorts before STRING2 lexicographically.\n"
				"      STRING1 > STRING2\n"
				"                     True if STRING1 sorts after STRING2 lexicographically.\n\n"

				"    Other operators:\n\n"

				"      -o OPTION      True if the shell option OPTION is enabled.\n"
				"      -v VAR         True if the shell variable VAR is set.\n"
				"      -R VAR         True if the shell variable VAR is set and is a name\n"
				"                     reference.\n"
				"      ! EXPR         True if expr is false.\n"
				"      EXPR1 -a EXPR2 True if both expr1 AND expr2 are true.\n"
				"      EXPR1 -o EXPR2 True if either expr1 OR expr2 is true.\n\n"

				"      arg1 OP arg2   Arithmetic tests.  OP is one of -eq, -ne,\n"
				"                     -lt, -le, -gt, or -ge.\n\n"

				"    Arithmetic binary operators return true if ARG1 is equal, not-equal,\n"
				"    less-than, less-than-or-equal, greater-than, or greater-than-or-equal\n"
				"    than ARG2.\n\n"

				"    See the bash manual page bash(1) for the handling of parameters (i.e.\n"
				"    missing parameters).\n\n"

				"    Exit Status:\n"
				"      Returns success if EXPR evaluates to true; fails if EXPR evaluates to\n"
				"      false or an invalid argument is given.\n";

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
				"test 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by "DEVELOPER" ("LOGIN42").\n";

			print(STDOUT_FILENO, msg, RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Test"

	int bt_test(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		char *syntax = "test [expr]";
		if (argc && !strcmp(argv[0], "[")) syntax = "[ arg... ]";

		t_parse_result *result = parse_options(argc, argv, "", NULL, long_opts, syntax, IGNORE_OFF);
		if (!result)		return (1);
		if (result->error)	return (free_options(result), 2);

		if (find_long_option(result, "help") && !strcmp(argv[0], "bracket"))		return (free_options(result), bt_bracket_help(HELP_NORMAL, 0));
		if (find_long_option(result, "help"))										return (free_options(result), bt_test_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version") && !strcmp(argv[0], "bracket"))		return (free_options(result), version_bracket());
		if (find_long_option(result, "version"))									return (free_options(result), version());


		int ret = 0;


		return (free_options(result), ret);
	}

#pragma endregion

#pragma region "Info"

	// [] es un alias de test, por decirlo de alguna manera

	// El builtin test evalúa condiciones relacionadas con archivos, cadenas y números.
	// Aquí tienes una descripción de los operadores que necesitas implementar:
	//
	//	Operadores para archivos
	//		-b: Verdadero si el archivo es un especial de bloque.
	//		-c: Verdadero si el archivo es un especial de carácter.
	//		-d: Verdadero si el archivo es un directorio.
	//		-e: Verdadero si el archivo existe.
	//		-f: Verdadero si el archivo es un archivo regular.
	//		-g: Verdadero si el archivo tiene el bit SGID activado.
	//		-L: Verdadero si el archivo es un enlace simbólico.
	//		-p: Verdadero si el archivo es un FIFO o pipe con nombre.
	//		-r: Verdadero si el archivo es legible.
	//		-S: Verdadero si el archivo es un socket.
	//		-s: Verdadero si el archivo tiene un tamaño mayor que 0.
	//		-u: Verdadero si el archivo tiene el bit SUID activado.
	//		-w: Verdadero si el archivo es escribible.
	//		-x: Verdadero si el archivo es ejecutable.
	//
	//	Operadores para cadenas
	//		-z: Verdadero si la longitud de la cadena es 0.
	//		=: Verdadero si dos cadenas son iguales.
	//		!=: Verdadero si dos cadenas son diferentes.
	//
	//	Operadores para números
	//		-eq: Verdadero si los números son iguales.
	//		-ne: Verdadero si los números son diferentes.
	//		-ge: Verdadero si el primer número es mayor o igual al segundo.
	//		-lt: Verdadero si el primer número es menor que el segundo.
	//		-le: Verdadero si el primer número es menor o igual al segundo.
	//	Otros operadores
	//		!: Negación de la condición.
	//		Sin operador: Un solo operando se evalúa como verdadero si no está vacío.
	//
	//	Notas importantes
	//		Las expresiones tienen que evaluarse de manera estricta con la sintaxis esperada.
	//		El builtin debe manejar tanto expresiones simples como compuestas. Ejemplo:
	//
	//	Ejemplo
	//		test -f archivo.txt && echo "Es un archivo regular"
	//

#pragma endregion
