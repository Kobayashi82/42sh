/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 18:57:50 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/06 18:57:54 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include "parser/ast.h"
	#include "parser/lexer.h"

#pragma endregion

#pragma region "Variables"

	#pragma region "Structures"

		typedef struct e_parser {
			t_lexer	lexer;
			t_token	*token;
			int		interactive;
		} t_parser;

	#pragma endregion

	extern t_parser *g_parser;

#pragma endregion

#pragma region "Methods"

	void	syntax_error(const char *msg);
	t_token	*token_advance();

	t_ast	*sub_parse(char *content);
	t_ast	*parse_simple_command();
	t_ast	*parse_list();
	t_ast	*parse(char *input, t_callback callback, int interactive, char *filename, int line);

#pragma endregion
