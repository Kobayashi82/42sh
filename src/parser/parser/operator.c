/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:38:23 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/03 21:11:15 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "parser/parser.h"

#pragma endregion

#pragma region "Parse Pipeline"

	t_ast *parse_pipeline() {
		t_ast *left = parse_command();
		if (!left) return (NULL);

		while (g_parser->token->type == TOKEN_PIPE || g_parser->token->type == TOKEN_PIPE_ALL) {
			t_ast *node = ast_create(g_parser->token->type);
			token_advance();

			g_parser->lexer.can_expand_alias = 1;
			g_parser->lexer.command_position = 1;

			node->left = left;
			node->right = parse_command();
			if (!node->right) {
				ast_free(&node);
				return (NULL);
			}
			left = node;
		}

		return (left);
	}

#pragma endregion

#pragma region "Parse And Or"

	t_ast *parse_and_or() {
		t_ast *left = parse_pipeline();
		if (!left) return (NULL);

		while (g_parser->token->type == TOKEN_AND || g_parser->token->type == TOKEN_OR) {
			t_ast *node = ast_create(g_parser->token->type);
			
			g_parser->lexer.can_expand_alias = 1;
			g_parser->lexer.command_position = 1;
			token_advance();

			while (g_parser->token->type == TOKEN_NEWLINE) {
				g_parser->lexer.can_expand_alias = 1;
				g_parser->lexer.command_position = 1;
				token_advance();
			}

	        if (g_parser->token->type == TOKEN_AND || 
				g_parser->token->type == TOKEN_OR ||
				g_parser->token->type == TOKEN_PIPE ||
				g_parser->token->type == TOKEN_PIPE_ALL ||
				g_parser->token->type == TOKEN_SEMICOLON ||
				g_parser->token->type == TOKEN_BACKGROUND) {
				ast_free(&left);
				ast_free(&node);
				syntax_error("unexpected token after operator");
				return (NULL);
			}

			node->left = left;
			node->right = parse_pipeline();
			if (!node->right) {
				ast_free(&node);
				return (NULL);
			}
			left = node;
		}

		return (left);
	}

#pragma endregion

#pragma region "Parse List"

	t_ast *parse_list() {
		t_ast *left = parse_and_or();
		if (!left) return (NULL);

		while (1) {
			while (g_parser->token->type == TOKEN_NEWLINE) token_advance();

			if (g_parser->token->type != TOKEN_SEMICOLON && g_parser->token->type != TOKEN_BACKGROUND) break;

			t_ast *node = ast_create(g_parser->token->type);

			g_parser->lexer.can_expand_alias = 1;
			g_parser->lexer.command_position = 1;
			token_advance();

			while (g_parser->token->type == TOKEN_NEWLINE) {
				g_parser->lexer.can_expand_alias = 1;
				g_parser->lexer.command_position = 1;
				token_advance();
			}

			node->left = left;
			node->right = parse_and_or();
			if (!node->right) {
				ast_free(&node);
				return (NULL);
			}
			left = node;
		}

		return (left);
	}

#pragma endregion
