/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:38:21 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/25 18:56:57 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/libft.h"
#include "parser/parser.h"

// Contexto para comillas, parentesis y llaves
// Analisis sintactico modular
// Solicitud de más input si es necesario
// Creación de AST
// Liberación de AST
// Impresión de AST
// Si error, devolver AST vacío, si más input, devolver NULL
// Establecer exit_code en error

t_ast_node *parse_word(t_token *token) {
	t_ast_node *new_node = malloc(sizeof(t_ast_node));

	new_node->value = token->value;
	new_node->child = NULL;
	new_node->type = (int)token->type;
	token->value = NULL;

	lexer_token_free(token);

	return (new_node);
}

t_parser_state parse(t_ast_node **ast) {
	t_token *token;

	t_ast_node *current = *ast;
	if (current && current->child) current = current->child;

	while ((token = lexer_token_next())) {

		if (token->type == TOKEN_INPUT) {
			lexer_token_free(token);
			return (PARSER_INPUT);
		}

		if (token->type == TOKEN_EOF) {
			lexer_token_free(token);
			return (PARSER_COMPLETED);
		}

		t_ast_node *new_node = parse_word(token);

		if (!*ast) {
			*ast = new_node;
			current = *ast;
		} else {
			current->child = new_node;
			current = new_node;
		}

	}

	return (PARSER_ERROR);
}
