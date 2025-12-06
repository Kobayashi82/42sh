/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:38:28 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/06 19:01:39 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "parser/parser.h"

	#include <stdlib.h>
	#include <string.h>

#pragma endregion

#pragma region "Redirection"

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

	#pragma region "Redir Create"

		void redir_create(t_redir **redirs, int type, char *file) {
			t_redir *new_redir = malloc(sizeof(t_redir));
			new_redir->type = type;
			new_redir->file = file;
			new_redir->fd = -1;

			if (!*redirs) {
				*redirs = new_redir;
				new_redir->prev = NULL;
				new_redir->next = NULL;
				return;
			}

			t_redir *curr = *redirs;
			while (curr->next)
				curr = curr->next;

			curr->next = new_redir;
			new_redir->prev = curr;
			new_redir->next = NULL;
		}

	#pragma endregion

	#pragma region "Parse Redirect"

		int parse_redirect(t_redir **redirs) {
			int type = g_parser->token->type;

			token_advance();

			if (g_parser->token->type != TOKEN_WORD) {
				syntax_error("archivo necesario... no todas las redirects usan archivo o fd... creo");
				return (1);
			}

			char *file = g_parser->token->value;
			g_parser->token->value = NULL;

			token_advance();

			redir_create(redirs, type, file);

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Assign Create"

	void assign_create(t_assign **assign, char *value) {
		t_assign *new_assign = malloc(sizeof(t_assign));
		new_assign->value = value;

		if (!*assign) {
			*assign = new_assign;
			new_assign->prev = NULL;
			new_assign->next = NULL;
			return;
		}

		t_assign *curr = *assign;
		while (curr->next)
			curr = curr->next;

		curr->next = new_assign;
		new_assign->prev = curr;
		new_assign->next = NULL;
	}

#pragma endregion

#pragma region "Args Create"

	// Aqui habria que redividir dependiendo de lo que haya, comillas, expansion, literal, etc.
	void args_create(t_args **args, char *value) {
		t_args *new_arg = malloc(sizeof(t_args));
		new_arg->value = value;
		new_arg->quoted = 0;

		if (!*args) {
			*args = new_arg;
			new_arg->prev = NULL;
			new_arg->next = NULL;
			return;
		}

		t_args *curr = *args;
		while (curr->next)
			curr = curr->next;

		curr->next = new_arg;
		new_arg->prev = curr;
		new_arg->next = NULL;
	}

#pragma endregion

#pragma region "Parse Simple Command"

	t_ast *parse_simple_command() {
		t_ast *node = ast_create(TOKEN_WORD);
		int cmd_found = 0;

		while (g_parser->token->type == TOKEN_WORD || is_redirect(g_parser->token->type)) {

			if (g_parser->token->type == TOKEN_WORD) {
				char *equal = strchr(g_parser->token->value, '=');
				if (!cmd_found && equal && equal != g_parser->token->value)
					assign_create(&node->assign, g_parser->token->value);
				else {
					cmd_found = 1;
					args_create(&node->args, g_parser->token->value);
				}
				g_parser->token->value = NULL;
				token_advance();
			} else {
				parse_redirect(&node->redirs);
			}
		}

		if (g_parser->token->type != TOKEN_EOF && !node->assign && !node->args && !node->redirs) {
			syntax_error("expected command");
			ast_free(&node);
			return (NULL);
		}

		if (stack_top(&g_parser->lexer)) {
			syntax_error("conexto abierto");
			ast_free(&node);
			return (NULL);
		}

		return (node);
	}

#pragma endregion
