/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 20:45:33 by vzurera-          #+#    #+#             */
/*   Updated: 2025/03/01 20:04:42 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "libft.h"
	#include "parser/input.h"

#pragma endregion

// typedef enum e_syntax_error {
// 	ERRORCITO
// } t_syntax_error;

// void syntax_error(t_syntax_error error, char *value) {

// }

// static int syntax_shell(const char *input, size_t *i, t_context *context) {
// 	if (!input || !*input) return (0);


// 	return (0);
// }


int	check_syntax(const char *input, t_context *context) {
	if (!input || !*input) return (0);
	(void) input;
	(void) context;
	int invalid = 0;

	if (invalid) {
		write(1, "Error chiquitin\n", 16);
		return (1);
	}

	return (0);
}
