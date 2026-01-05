/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   let.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 10:59:53 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/05 23:50:32 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "hashes/builtin.h"
	#include "utils/libft.h"
	#include "utils/print.h"
	#include "utils/getopt2.h"

#pragma endregion

#pragma region "Help / Version"

	#pragma region "Help"

		int bt_let_help(int format, int no_print) {
			char *name = "let";
			char *syntax = "let arg [arg ...]";
			char *description = "Evaluate arithmetic expressions.";
			char *msg =
				"    Evaluate each ARG as an arithmetic expression.  Evaluation is done in\n"
				"    fixed-width integers with no check for overflow, though division by 0\n"
				"    is trapped and flagged as an error.  The following list of operators is\n"
				"    grouped into levels of equal-precedence operators.  The levels are listed\n"
				"    in order of decreasing precedence.\n\n"

				"        id++, id--      variable post-increment, post-decrement\n"
				"        ++id, --id      variable pre-increment, pre-decrement\n"
				"        -, +            unary minus, plus\n"
				"        !, ~            logical and bitwise negation\n"
				"        **              exponentiation\n"
				"        *, /, %         multiplication, division, remainder\n"
				"        +, -            addition, subtraction\n"
				"        <<, >>          left and right bitwise shifts\n"
				"        <=, >=, <, >    comparison\n"
				"        ==, !=          equality, inequality\n"
				"        &               bitwise AND\n"
				"        ^               bitwise XOR\n"
				"        |               bitwise OR\n"
				"        &&              logical AND\n"
				"        ||              logical OR\n"
				"        expr ? expr : expr\n"
				"                        conditional operator\n"
				"        =, *=, /=, %=,\n"
				"        +=, -=, <<=, >>=,\n"
				"        &=, ^=, |=      assignment\n\n"

				"    Shell variables are allowed as operands.  The name of the variable\n"
				"    is replaced by its value (coerced to a fixed-width integer) within\n"
				"    an expression.  The variable need not have its integer attribute\n"
				"    turned on to be used in an expression.\n\n"

				"    Operators are evaluated in order of precedence.  Sub-expressions in\n"
				"    parentheses are evaluated first and may override the precedence\n"
				"    rules above.\n\n"

				"    Exit Status:\n"
				"      If the last ARG evaluates to 0, let returns 1; let returns 0 otherwise.\n";

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
				"let 1.0.\n"
				"Copyright (C) 2026 Kobayashi Corp ⓒ.\n"
				"This is free software: you are free to change and redistribute it.\n"
				"There is NO WARRANTY, to the extent permitted by law.\n\n"

				"Written by Kobayashi82 (vzurera-).\n";

			print(STDOUT_FILENO, msg, RESET_PRINT);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Let"

	int bt_let(int argc, char **argv) {
		t_long_option long_opts[] = {
			{"help",	NO_ARGUMENT, 0},
			{"version",	NO_ARGUMENT, 0},
			{NULL, 0, 0}
		};

		t_parse_result *result = parse_options(argc, argv, "", NULL, long_opts, "let arg [arg ...]", 0);
		if (!result)		return (1);
		if (result->error)	return (free_options(result), 1);

		if (find_long_option(result, "help"))		return (free_options(result), bt_let_help(HELP_NORMAL, 0));
		if (find_long_option(result, "version"))	return (free_options(result), version());


		int ret = 0;


		return (free_options(result), ret);
	}

#pragma endregion
