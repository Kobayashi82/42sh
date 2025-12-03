/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 13:27:05 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/03 20:06:05 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "parser/parser.h"

	#include <unistd.h>
	#include <stdlib.h>

	#include <stdio.h>				// borrar

#pragma endregion

#pragma region "Print"

	const char *type_to_string(int type) {
		switch (type) {
			case TOKEN_WORD:				return ("WORD");
			case TOKEN_CMD_SUB:				return ("$(");
			case TOKEN_ARITH_SUB:			return ("$((");
			case TOKEN_PARAM_EXP:			return ("${");
			case TOKEN_BRACE:				return ("{");
			case TOKEN_GROUP:				return ("{ ");
			case TOKEN_SUBSHELL:			return ("(");
			case TOKEN_ARITH:				return ("((");

			case TOKEN_REDIRECT_IN:			return ("<");
			case TOKEN_REDIRECT_HEREDOC:	return ("<<");
			case TOKEN_REDIRECT_HERESTRING:	return ("<<<");
			case TOKEN_REDIRECT_IN_OUT:		return ("<>");
			case TOKEN_REDIRECT_DUP_IN:		return ("<&");
			case TOKEN_REDIRECT_OUT:		return (">");
			case TOKEN_REDIRECT_APPEND:		return (">>");
			case TOKEN_REDIRECT_FORCE_OUT:	return (">|");
			case TOKEN_REDIRECT_DUP_OUT:	return (">&");
			case TOKEN_REDIRECT_OUT_ALL:	return ("&>");
			case TOKEN_REDIRECT_APPEND_ALL:	return ("&>>");

			case TOKEN_PIPE:				return ("|");
			case TOKEN_PIPE_ALL:			return ("|&");
			case TOKEN_BACKGROUND:			return ("&");
			case TOKEN_AND:					return ("&&");
			case TOKEN_OR:					return ("||");
			case TOKEN_SEMICOLON:			return (";");

			case TOKEN_IF:					return ("if");
			case TOKEN_THEN:				return ("then");
			case TOKEN_ELSE:				return ("else");
			case TOKEN_ELIF:				return ("elif");
			case TOKEN_FI:					return ("fi");
			
			case TOKEN_CASE:				return ("case");
			case TOKEN_ESAC:				return ("esac");
			case TOKEN_IN:					return ("in");
			case TOKEN_TERMINATOR:			return (";;");
			case TOKEN_FALLTHROUGH:			return (";&");
			case TOKEN_CONTINUE:			return (";;&");
			
			case TOKEN_FUNCTION:			return ("function");
			case TOKEN_WHILE:				return ("while");
			case TOKEN_UNTIL:				return ("until");
			case TOKEN_DO:					return ("do");
			case TOKEN_DONE:				return ("done");
			case TOKEN_FOR:					return ("for");
			case TOKEN_NEGATE:				return ("!");
			case TOKEN_LBRACKET2:			return ("[[");
			case TOKEN_RBRACKET2:			return ("]]");

			case TOKEN_NEWLINE:				return ("NEWLINE");
			case TOKEN_EOF:					return ("EOF");

			default: return ("?");
		}
	}

	void ast_print_indent(int level) {
		for (int i = 0; i < level; i++) write(1, "  ", 2);
	}

	void ast_print_recursive(t_ast *node, int level) {
		if (!node) return;

		ast_print_indent(level);

		switch (node->type) {
			case TOKEN_PIPE:
				printf("PIPE\n");
				ast_print_recursive(node->left, level + 1);
				ast_print_recursive(node->right, level + 1);
				break;

			case TOKEN_AND:
				printf("AND (&&)\n");
				ast_print_recursive(node->left, level + 1);
				ast_print_recursive(node->right, level + 1);
				break;

			case TOKEN_OR:
				printf("OR (||)\n");
				ast_print_recursive(node->left, level + 1);
				ast_print_recursive(node->right, level + 1);
				break;

			case TOKEN_SEMICOLON:
				printf("SEQUENCE (;)\n");
				ast_print_recursive(node->left, level + 1);
				ast_print_recursive(node->right, level + 1);
				break;

			case TOKEN_BACKGROUND:
				printf("BACKGROUND (&)\n");
				ast_print_recursive(node->left, level + 1);
				ast_print_recursive(node->right, level + 1);
				break;

			case TOKEN_SUBSHELL:
				printf("SUBSHELL ()\n");
				ast_print_recursive(node->child, level + 1);
				break;

			case TOKEN_NEWLINE:
				printf("NEWLINE \\n\n");
				ast_print_recursive(node->child, level + 1);
				break;

			case TOKEN_WORD:
				printf("COMMAND\n");

				// Args
				if (node->args) {
					ast_print_indent(level + 1);
					printf("args:\n");
					t_args *arg = node->args;
					while (arg) {
						ast_print_indent(level + 2);
						printf("- \"%s\" %s\n", arg->value, arg->quoted ? "(quoted)" : "");
						arg = arg->next;
					}
				}

				// Redirs
				if (node->redirs) {
					ast_print_indent(level + 1);
					printf("redirs:\n");
					t_redir *redir = node->redirs;
					while (redir) {
						ast_print_indent(level + 2);
						printf("- %s %s (fd: %d)\n", type_to_string(redir->type), redir->file, redir->fd);
						redir = redir->next;
					}
				}
				break;

			default:
				printf("UNKNOWN (%d)\n", node->type);
				break;
		}
	}

	void ast_print(t_ast *ast) {
		if (!ast) return;
		ast_print_recursive(ast, 0);
	}

#pragma endregion

#pragma region "Free"

	void ast_free(t_ast **ast) {
		if (!ast || !*ast) return;

		t_ast *node = *ast;

		if (node->left)		ast_free(&node->left);
		if (node->right)	ast_free(&node->right);
		if (node->child)	ast_free(&node->child);

		// Args
		if (node->args) {
			t_args *arg = node->args;
			while (arg) {
				t_args *next = arg->next;
				if (arg->value) free(arg->value);
				free(arg);
				arg = next;
			}
		}

		// Redirs
		if (node->redirs) {
			t_redir *redir = node->redirs;
			while (redir) {
				t_redir *next = redir->next;
				if (redir->file) free(redir->file);
				free(redir);
				redir = next;
			}
		}

		if (node->value) free(node->value);

		free(node);
		*ast = NULL;
	}

#pragma endregion

#pragma region "Create"

	t_ast *ast_create(int type) {
		t_ast *node = calloc(1, sizeof(t_ast));
		node->type = type;

		return (node);
	}

#pragma endregion
