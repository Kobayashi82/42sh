/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:38:21 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/09 10:48:36 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "terminal/readinput/history.h"
	#include "parser/parser.h"
	#include "main/shell.h"
	#include "utils/utils.h"

	#include "terminal/terminal.h"	// borrar

#pragma endregion

#pragma region "Variables"

	t_parser *g_parser;

#pragma endregion

#pragma region "Syntax Error"

	void syntax_error(int type, const char *msg) {
		if (g_parser && g_parser->token) {
			if (shell.mode == MD_INTERACTIVE) {
				if (type == 1) {
					if (g_parser->prev_token) dprintf(2, "42sh: -c: line %d: unexpected EOF while looking for matching `%s'\n", g_parser->prev_token->line, msg);
					if (g_parser->token) dprintf(2, "42sh: -c: line %d: syntax error: unexpected end of file\n", g_parser->token->line);
				}
			}
			if (shell.mode == MD_ARGUMENT) {
				if (type == 1) {
					if (g_parser->prev_token) dprintf(2, "42sh: -c: line %d: unexpected EOF while looking for matching `%s'\n", g_parser->prev_token->line, msg);
					if (g_parser->token) dprintf(2, "42sh: -c: line %d: syntax error: unexpected end of file\n", g_parser->token->line);
				}
			}
			else {
				if (g_parser->token->filename)	dprintf(2, "  at %s:%d\n", g_parser->token->filename, g_parser->token->line);
				if (g_parser->token->full_line)	dprintf(2, "  %s\n", g_parser->token->full_line);
				dprintf(2, "  Error: %s\n", msg);
			}
		}
	}

#pragma endregion

#pragma region "Next token"

	t_token *token_advance() {
		if (!g_parser) return (NULL);

		token_free(g_parser->prev_token);
		g_parser->prev_token = g_parser->token;
		g_parser->token = token_get(&g_parser->lexer);

		return (g_parser->token);
	}

#pragma endregion

#pragma region "SubParser"

	static char *get_content(int type, char *value) {
		switch (type) {
			case TOKEN_BACKTICK:
				if (ft_strlen(value) >= 2) {
					char *content = ft_substr(value, 1, ft_strlen(value) - 2);
					return (free(value), content);
				}
			case TOKEN_ARITH:
				if (ft_strlen(value) >= 4) {
					char *content = ft_substr(value, 2, ft_strlen(value) - 4);
					return (free(value), content);
				}
			case TOKEN_ARITH_SUB:
				if (ft_strlen(value) >= 5) {
					char *content = ft_substr(value, 3, ft_strlen(value) - 5);
					return (free(value), content);
				}
			case TOKEN_SUBSHELL:
				if (ft_strlen(value) >= 2) {
					char *content = ft_substr(value, 1, ft_strlen(value) - 2);
					return (free(value), content);
				}
			case TOKEN_CMD_SUB:
				if (ft_strlen(value) >= 3) {
					char *content = ft_substr(value, 2, ft_strlen(value) - 3);
					return (free(value), content);
				}
			case TOKEN_PROCESS_SUB_IN:
				if (ft_strlen(value) >= 3) {
					char *content = ft_substr(value, 2, ft_strlen(value) - 3);
					return (free(value), content);
				}
			case TOKEN_PROCESS_SUB_OUT:
				if (ft_strlen(value) >= 3) {
					char *content = ft_substr(value, 2, ft_strlen(value) - 3);
					return (free(value), content);
				}
			case TOKEN_GROUP:
				if (ft_strlen(value) >= 2) {
					char *content = ft_substr(value, 1, ft_strlen(value) - 2);
					return (free(value), content);
				}
			case TOKEN_CONDITIONAL:
				if (ft_strlen(value) >= 4) {
					char *content = ft_substr(value, 2, ft_strlen(value) - 4);
					return (free(value), content);
				}
		}

		free(value);
		return (NULL);
	}

	t_ast *sub_parse(int type, char *value) {
		t_parser	parser;
		t_parser	*old_parser = g_parser;

		parser.prev_token = NULL;
		parser.token = NULL;
		parser.interactive = 0;
		g_parser = &parser;
		
		char *content = get_content(type, value);
		if (!content) {
			g_parser = old_parser;
			return (NULL);
		}

		lexer_init(&parser.lexer, content, NULL, 0, NULL, -1);

		token_advance();

		t_ast *ast = NULL;
		switch (type) {
			case TOKEN_BACKTICK:		ast = parse_list();		break;
			case TOKEN_ARITH:									break;
			case TOKEN_ARITH_SUB:								break;
			case TOKEN_SUBSHELL:		ast = parse_list();		break;
			case TOKEN_CMD_SUB:			ast = parse_list();		break;
			case TOKEN_PROCESS_SUB_IN:	ast = parse_list();		break;
			case TOKEN_PROCESS_SUB_OUT:	ast = parse_list();		break;
			case TOKEN_GROUP:			ast = parse_list();		break;
			case TOKEN_CONDITIONAL:								break;
		}

		token_free(parser.prev_token);
		token_free(parser.token);
		lexer_free(&parser.lexer);
		g_parser = old_parser;

		return (ast);
	}

#pragma endregion

#pragma region "Parse Command"

	t_ast *parse_command() {
		if (!g_parser->token || !g_parser->token->segment) return (NULL);

		if (g_parser->token->type >= TOKEN_BACKTICK && g_parser->token->type <= TOKEN_CONDITIONAL) {
			// Flatten segments (will be process again in subparser, fuck optimization XD)
			char *value = segment_flatten(g_parser->token->segment);
			int type = g_parser->token->type;

			t_ast *node = ast_create(g_parser->token->type);
			token_advance();

			if (stack_top(&g_parser->lexer) && !g_parser->lexer.input) {
				syntax_error(1, stack_get(&g_parser->lexer));
				ast_free(&node);
				return (NULL);
			}

			node->child = sub_parse(type, value);
			if (!node->child) ast_free(&node);

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
				syntax_error(0, "unexpected token after operator");
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

		parser.prev_token = NULL;
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

		token_free(g_parser->prev_token);
		token_free(g_parser->token);

		if (interactive && full_ast && full_ast->type != TOKEN_EOF) history_add(parser.lexer.full_input, 0);
		if (interactive)	terminal.input = ft_strdup(parser.lexer.full_input);	// borrar
		else				terminal.input = ft_strdup("");							// borrar

		lexer_free(&parser.lexer);
		return (full_ast);
	}

#pragma endregion
