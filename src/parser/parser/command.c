/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:38:28 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/03 17:17:47 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "parser/parser.h"

	#include <stdlib.h>

#pragma endregion

#pragma region "Is Redirect"

	int is_redirect(int type) {
		return (type == TOKEN_REDIRECT_IN ||
				type == TOKEN_REDIRECT_HEREDOC ||
				type == TOKEN_REDIRECT_HERESTRING ||
				type == TOKEN_REDIRECT_IN_OUT ||
				type == TOKEN_REDIRECT_DUP_IN ||
				type == TOKEN_REDIRECT_OUT ||
				type == TOKEN_REDIRECT_APPEND ||
				type == TOKEN_REDIRECT_FORCE_OUT ||
				type == TOKEN_REDIRECT_DUP_OUT ||
				type == TOKEN_REDIRECT_OUT_ALL ||
				type == TOKEN_REDIRECT_APPEND_ALL);
	}

#pragma endregion

#pragma region "Args Append"

	void args_append(t_args **list, t_args *new_arg) {
		if (!*list) {
			*list = new_arg;
			new_arg->prev = NULL;
			new_arg->next = NULL;
			return;
		}

		t_args *curr = *list;
		while (curr->next)
			curr = curr->next;

		curr->next = new_arg;
		new_arg->prev = curr;
		new_arg->next = NULL;
	}

#pragma endregion

#pragma region "Args Create"

	// Aqui habria que redividir dependiendo de lo que haya, comillas, expansion, literal, etc.
	t_args *args_create(char *value) {
		t_args *arg = malloc(sizeof(t_args));
		arg->value = value;
		arg->quoted = 0;	// Cambiar por un enum
		arg->prev = NULL;
		arg->next = NULL;
		return (arg);
	}

#pragma endregion

#pragma region "Parse From String"

	// Parsea una cadena (para subshells)
	t_ast *parse_from_string(char *content) {
		t_parser	parser;
		t_parser	*old_parser = g_parser;

		parser.token = NULL;
		parser.interactive = 0;
		g_parser = &parser;

		lexer_init(&parser.lexer, content, NULL, 0, NULL, -1);

		token_advance();
		t_ast *ast = parse_complete_command();

		token_free(parser.token);
		lexer_free(&parser.lexer);

		g_parser = old_parser;

		return (ast);
	}

#pragma endregion

#pragma region "Parse Simple Command"

	t_ast *parse_simple_command() {
		t_ast *node = ast_create(TOKEN_WORD);

		while (g_parser->token->type == TOKEN_WORD || is_redirect(g_parser->token->type)) {

			if (g_parser->token->type == TOKEN_WORD) {
				t_args *arg = args_create(g_parser->token->value);
				g_parser->token->value = NULL;
				args_append(&node->args, arg);
				token_advance();
			} else if (is_redirect(g_parser->token->type)) {
				t_redir *redir = parse_redirect();
				redir_append(&node->redirs, redir);
			}
		}

		return (node);
	}

#pragma endregion

#pragma region "Parse Command"

	t_ast *parse_command() {

		if (g_parser->token->type == TOKEN_SUBSHELL) {
			t_ast *node = ast_create(TOKEN_SUBSHELL);
			char *content = g_parser->token->value;
			g_parser->token->value = NULL;
			token_advance();

			node->child = parse_from_string(content);
			free(content);

			return (node);
		}

		return (parse_simple_command());
	}

#pragma endregion
