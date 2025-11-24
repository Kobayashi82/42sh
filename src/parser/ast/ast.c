/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:40:20 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/25 00:41:31 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/libft.h"
#include "parser/ast.h"

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
    printf("WORD \"%s\"\n", node->value);
	print_ast_indent(node->child, indent + 1);
}

void ast_print(t_ast_node *node) {
    print_ast_indent(node, 0);
}
