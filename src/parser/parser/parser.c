/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:38:21 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/25 00:34:56 by vzurera-         ###   ########.fr       */
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

t_ast_node *parse(t_ast_node **ast) {
	t_token *token;

	t_ast_node *current = *ast;
	if (current && current->child) current = current->child;

	token = lexer_token_next();
	while (token) {
		t_ast_node *new_node = malloc(sizeof(t_ast_node));

		if (!*ast) {
			*ast = new_node;
			current = *ast;
		} else current->child = new_node;

		new_node->value = token->value;
		new_node->child = NULL;
		token->value = NULL;

		lexer_token_free(token);
		token = lexer_token_next();
		if (!token) return (NULL);
		if (token->type == TOKEN_EOF) {
			lexer_token_free(token);
			break;
		}
	}

	return (*ast);
}
