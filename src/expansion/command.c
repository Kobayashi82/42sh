/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:29:54 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/28 21:56:19 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "utils/libft.h"
	#include "expansion/arithmetic.h"
	#include "parser/syntax.h"

#pragma endregion

#pragma region "Is Separator"
	
	bool is_separator(const char *input, size_t *i, char *last_token) {
			// Check for 2-character operators
		if (!strncmp(&input[*i], "&&", 2) ||		// Logical AND
			!strncmp(&input[*i], "||", 2) ||		// Logical OR
			!strncmp(&input[*i], "$(", 2) ||		// Command Substitution
			!strncmp(&input[*i], "${", 2) || 	// Parameter Expansion
			!strncmp(&input[*i], "{ ", 2)) { 	// Command Group
			strncpy(last_token, &input[*i], 2); last_token[2] = '\0';
			*i += 1; return (true);
		}	// Check for 1-character operators
		else if (strchr(";|&\n", input[*i])) {
			strncpy(last_token, &input[*i], 1); last_token[1] = '\0';
			return (true);
		}

		last_token[0] = '\0';
		return (false);
	}

	bool is_not_separator(char c) {
		return (c == '$' || c == '`' || c == '(' || c == ')' || c == '\'' || c == '"' || c == '{' || c == '}' || c == ';' || c == '&' || c == '|' || c == '\n' || isspace(c));
	}

#pragma endregion

#pragma region "Shell"

	//	{ cmd; }				debe añadir '; ' al meterlo en varias lineas
	//	( (echo popo) ls )		comandos o argumentos despues de () o (()) o { cmd; } 
	//	( )						subshells vacias con espacios
	//	Terminar por ; o & permitido, otros no se permiten
	//	Token de cierre sin venir a cuento
	//	Redirecciones
	//	Error messages

	// echo $(echo "popo
	// 	> && echo pipi")
	// 	42sh: syntax error near unexpected token `invalid comand'

#pragma endregion
