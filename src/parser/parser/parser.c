/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:38:21 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/03 17:25:04 by vzurera-         ###   ########.fr       */
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

	t_ast *parse_complete_command();

#pragma endregion

#pragma region "Syntax Error"

	void syntax_error(const char *msg) {
		dprintf(2, "42sh: syntax error: %s\n", msg);

		if (g_parser && g_parser->token) {
			if (g_parser->token->filename)	dprintf(2, "  at %s:%d\n", g_parser->token->filename, g_parser->token->line);
			if (g_parser->token->full_line)	dprintf(2, "  %s\n", g_parser->token->full_line);
		}

		if (!g_parser->interactive) exit(1);
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

#pragma region "Parse Complete Command"

	t_ast *parse_complete_command() {
		t_ast *result = parse_list();

		if (g_parser->token->type != TOKEN_EOF) syntax_error("unexpected token");

		return (result);
	}

#pragma endregion

#pragma region "Parser"

	t_ast *parse(char *input, t_callback callback, int interactive, char *filename, int line) {
		t_parser parser;

		parser.token = NULL;
		parser.interactive = interactive;
		g_parser = &parser;

		lexer_init(&parser.lexer, input, callback, interactive, filename, line);

		token_advance();
		t_ast *ast = parse_complete_command();
		token_free(g_parser->token);

		if (interactive && ast && ast->type != TOKEN_EOF) history_add(parser.lexer.full_input, 0);
		terminal.input = ft_strdup(parser.lexer.full_input);	// borrar

		lexer_free(&parser.lexer);
		return (ast);
	}

#pragma endregion

// parse_complete_command()
//     └─ parse_list()          // ; y &
//         └─ parse_and_or()    // && y ||
//             └─ parse_pipeline()   // | y |&
//                 └─ parse_command()     // (Subshell) y ((Arithmetic))
//                     └─ parse_simple_command()  // Comando con args, redirs y asignaciones
