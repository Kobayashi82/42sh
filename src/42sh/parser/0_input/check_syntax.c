/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 20:45:33 by vzurera-          #+#    #+#             */
/*   Updated: 2025/03/01 22:03:28 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"
	#include "terminal/print.h"
	#include "parser/input.h"
	#include "main/shell.h"

#pragma endregion

#pragma region "Variables"

	typedef enum e_syntax_error {
		ERRORCITO
	} t_syntax_error;

#pragma endregion

#pragma region "Syntax"

	#pragma region "Error"

		void syntax_error(t_syntax_error error, char *value) {
			if (error == ERRORCITO)
				print(STDOUT_FILENO, ft_strjoin_sep("Errorcito: ", value, "\n", 0), FREE_RESET_PRINT);
		}

	#pragma endregion

	#pragma region "Shell"
	
		static int syntax_shell(const char *input, size_t *i, t_context *context) {
			if (!input || !*input || !context) return (0);

			if (input[*i] == 'z') {
				syntax_error(ERRORCITO, "bonito");
				return (2);
			}

			return (0);
		}

	#pragma endregion

	#pragma region "Check"
	
		int	syntax_check(const char *input, t_context *context) {
			if (!input || !*input || !context) return (1);

			size_t i = 0;
			int result = syntax_shell(input, &i, context);
			if (result) shell.exit_code = result;

			return (result);
		}

	#pragma endregion
	
#pragma endregion
