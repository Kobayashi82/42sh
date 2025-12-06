/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:38:21 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/06 21:59:11 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "terminal/readinput/history.h"
	#include "parser/parser.h"
	#include "utils/libft.h"

	#include <stdio.h>				// borrar
	#include "terminal/terminal.h"	// borrar

#pragma endregion

#pragma region "Variables"

	t_parser *g_parser;

#pragma endregion

#pragma region "Syntax Error"

	void syntax_error(const char *msg) {
		dprintf(2, "42sh: syntax error: %s\n", msg);

		if (g_parser && g_parser->token) {
			if (g_parser->token->filename)	dprintf(2, "  at %s:%d\n", g_parser->token->filename, g_parser->token->line);
			if (g_parser->token->full_line)	dprintf(2, "  %s\n", g_parser->token->full_line);
		}

		// if (!g_parser->interactive) exit(1);
	}

#pragma endregion

#pragma region "Next token"

	t_token *token_advance() {
		if (!g_parser) return (NULL);

		token_free(g_parser->token);
		g_parser->token = token_get(&g_parser->lexer);

		return (g_parser->token);
	}

#pragma endregion

#pragma region "SubParser"

	t_ast *sub_parse(char *content) {
		t_parser	parser;
		t_parser	*old_parser = g_parser;

		parser.token = NULL;
		parser.interactive = 0;
		g_parser = &parser;

		lexer_init(&parser.lexer, content, NULL, 0, NULL, -1);

		token_advance();
		t_ast *ast = parse_list();

		token_free(parser.token);
		lexer_free(&parser.lexer);

		g_parser = old_parser;

		return (ast);
	}

#pragma endregion

#pragma region "Parse Command"

	t_ast *parse_command() {

		if (g_parser->token->type == TOKEN_SUBSHELL) {
			t_ast *node = ast_create(TOKEN_SUBSHELL);
			char *content = g_parser->token->value;
			g_parser->token->value = NULL;
			token_advance();

			node->child = sub_parse(content);

			return (node);
		}

		return (parse_simple_command());
	}

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

#pragma region "Parser"

	t_ast *parse(char *input, t_callback callback, int interactive, char *filename, int line) {
		t_parser	parser;
		t_ast		*full_ast = NULL;
		t_ast		*current = NULL;

		parser.token = NULL;
		parser.interactive = interactive;
		parser.lexer.can_expand_alias = 1;
		parser.lexer.command_position = 1;
		g_parser = &parser;

		lexer_init(&parser.lexer, input, callback, interactive, filename, line);
		token_advance();

		if (!interactive) {
			while (g_parser->token->type != TOKEN_EOF) {
				while (g_parser->token->type == TOKEN_NEWLINE) token_advance();
				if (g_parser->token->type == TOKEN_EOF) break;

				t_ast *ast = parse_list();
				if (!ast) {
					ast_free(&full_ast);
					break;
				}

				if (!full_ast) {
					full_ast = ast;
					current = ast;
				} else {
					t_ast *separator = ast_create(TOKEN_SEMICOLON);
					separator->left = current;
					separator->right = ast;
					current = separator;
					full_ast = current;
				}
			}
		} else full_ast = parse_list();

		token_free(g_parser->token);

		if (interactive && full_ast && full_ast->type != TOKEN_EOF) history_add(parser.lexer.full_input, 0);
		terminal.input = ft_strdup(parser.lexer.full_input);	// borrar

		lexer_free(&parser.lexer);
		return (full_ast);
	}

#pragma endregion

// Si termina en && u otro parecido, pero no hay mas lineas, error (no interactivo)
