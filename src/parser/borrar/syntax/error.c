/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:18:34 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/28 16:14:36 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "utils/libft.h"
	#include "utils/print.h"
	#include "parser/syntax.h"
	#include "main/shell.h"
	#include "main/project.h"

#pragma endregion

#pragma region "Error"

	void syntax_error(t_syntax_error error, char *value, int line) {
		char *pname = ft_strjoin(PROYECTNAME, ": ", 0);
		if (shell.source == SRC_ARGUMENT) pname = ft_strjoin(pname, "-c: ", 1);
		if (shell.source == SRC_NO_INTERACTIVE  && line >= 0) { 
			pname = ft_strjoin_sep(pname, "line ", ft_itoa(line), 6);
			pname = ft_strjoin(pname, ": ", 1);
		}

		if (value) value = ft_strjoin_sep("`", value, "'", 2);

		print(STDERR_FILENO, NULL, RESET);

		if (error == IN_TOKEN)
			print(STDERR_FILENO, ft_strjoin(pname, "syntax error: unexpected end of file", 0), FREE_JOIN);
		if (error == IN_TOKEN_EOF)
			print(STDERR_FILENO, ft_strjoin_sep(pname, "unexpected EOF while looking for matching ", value, 0), FREE_JOIN);
		if (error == TOKEN_NEAR)
			print(STDERR_FILENO, ft_strjoin_sep(pname, "syntax error near unexpected token ", value, 0), FREE_JOIN);
		if (error == ARGS_ARITHMETIC)
			print(STDERR_FILENO, ft_strjoin(pname, "syntax error: invalid arithmetic expression ", 0), FREE_JOIN);
		if (error == ARGS_SUBSHELL)
			print(STDERR_FILENO, ft_strjoin(pname, "syntax error: invalid subshell ", 0), FREE_JOIN);

		print(STDERR_FILENO, "\n", PRINT);
		free(pname); free(value);
	}

#pragma endregion

