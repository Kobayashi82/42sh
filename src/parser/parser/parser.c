/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:38:21 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/28 21:35:54 by vzurera-         ###   ########.fr       */
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

// Si no es interactivo, no debe pedir más input
// Detectar contexto abierto cuando llega a EOF

#pragma region "AST"

	void ast_free(t_ast_node **ast) {
		if (!ast || !*ast) return;
		
		free((*ast)->value);
		ast_free(&(*ast)->child);
		free(*ast);
		*ast = NULL;
	}

	static void print_ast_indent(t_ast_node *node, int indent) {
		if (!node) return;

		for (int i = 0; i < indent; i++) printf("  ");

		char *type = "WORD";

		if (node->type == TOKEN_CMD_SUB)				type = "CMD_SUB";
		if (node->type == TOKEN_ARITH_SUB)				type = "ARITH_SUB";
		if (node->type == TOKEN_PARAM_EXP)				type = "PARAM_EXP";
		if (node->type == TOKEN_BRACE)					type = "BRACE";
		if (node->type == TOKEN_SHELL)					type = "SHELL";
		if (node->type == TOKEN_ARITH)					type = "ARITH";

		if (node->type == TOKEN_REDIRECT_IN)			type = "IN";
		if (node->type == TOKEN_REDIRECT_HEREDOC)		type = "HEREDOC";
		if (node->type == TOKEN_REDIRECT_HERESTRING)	type = "HERESTRING";
		if (node->type == TOKEN_REDIRECT_IN_OUT)		type = "IN_OUT";
		if (node->type == TOKEN_REDIRECT_DUP_IN)		type = "DUP_IN";
		if (node->type == TOKEN_REDIRECT_OUT)			type = "OUT";
		if (node->type == TOKEN_REDIRECT_APPEND)		type = "APPEND";
		if (node->type == TOKEN_REDIRECT_FORCE_OUT)		type = "FORCE_OUT";
		if (node->type == TOKEN_REDIRECT_DUP_OUT)		type = "DUP_OUT";
		if (node->type == TOKEN_REDIRECT_OUT_ALL)		type = "OUT_ALL";
		if (node->type == TOKEN_REDIRECT_APPEND_ALL)	type = "APPEND_ALL";
		if (node->type == TOKEN_REDIRECT_PIPE_ALL)		type = "PIPE_ALL";
		if (node->type == TOKEN_REDIRECT_PIPE)			type = "PIPE";

		if (node->type == TOKEN_BACKGROUND)				type = "BACKGROUND";
		if (node->type == TOKEN_AND)					type = "AND";
		if (node->type == TOKEN_OR)						type = "OR";
		if (node->type == TOKEN_SEMICOLON)				type = "SEMICOLON";

		if (node->value)	printf("%s \"%s\"\n", type, node->value);
		else				printf("%s \n", type);

		print_ast_indent(node->child, indent + 1);
	}

	void ast_print(t_ast_node *node) {
		print_ast_indent(node, 0);
	}

#pragma endregion

#pragma region "Parser"

t_ast_node *parse_word(t_token *token) {
	t_ast_node *new_node = malloc(sizeof(t_ast_node));

	new_node->value = token->value;
	new_node->child = NULL;
	new_node->type = (int)token->type;
	token->value = NULL;

	token_free(token);

	return (new_node);
}

t_ast_node *parse() {
	t_ast_node *ast = NULL, *current = NULL;
	t_token		*token = NULL;

	while ((token = token_next())) {

		if (token->type == TOKEN_EOF) {
			token_free(token);
			return (ast);
		}

		t_ast_node *new_node = parse_word(token);

		if (!ast) {
			ast = new_node;
			current = ast;
		} else {
			current->child = new_node;
			current = new_node;
		}
	}

	return (ast);
}

#pragma endregion
