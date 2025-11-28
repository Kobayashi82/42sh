/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:40:20 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/28 16:24:06 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/libft.h"
#include "parser/parser.h"

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
