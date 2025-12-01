/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:38:23 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/01 22:55:55 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "parser/parser.h"

#pragma endregion

#pragma region "Parse Pipeline"

	t_ast *parse_pipeline() {
		t_ast *left = parse_command();

		while (g_parser->token->type == TOKEN_PIPE || g_parser->token->type == TOKEN_PIPE_ALL) {
			t_ast *node = ast_create(g_parser->token->type);
			next_token();
			node->left = left;
			node->right = parse_command();
			left = node;
		}

		return (left);
	}

#pragma endregion

#pragma region "Parse And Or"

	t_ast *parse_and_or() {
		t_ast *left = parse_pipeline();

		while (g_parser->token->type == TOKEN_AND || g_parser->token->type == TOKEN_OR) {
			t_ast *node = ast_create(g_parser->token->type);
			next_token();
			node->left = left;
			node->right = parse_pipeline();
			left = node;
		}

		return (left);
	}

#pragma endregion

#pragma region "Parse List"

	t_ast *parse_list() {
		t_ast *left = parse_and_or();

		while (g_parser->token->type == TOKEN_SEMICOLON || g_parser->token->type == TOKEN_BACKGROUND) {
			t_ast *node = ast_create(g_parser->token->type);
			next_token();
			node->left = left;
			node->right = parse_and_or();
			left = node;
		}

		return (left);
	}

#pragma endregion
