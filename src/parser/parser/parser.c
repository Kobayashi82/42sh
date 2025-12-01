/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:38:21 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/01 23:27:02 by vzurera-         ###   ########.fr       */
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

#pragma region "AST"

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

	#pragma region "Print"

		const char *token_type_to_string(t_token_type type) {
			switch (type) {
				case TOKEN_REDIRECT_IN: return "<";
				case TOKEN_REDIRECT_OUT: return ">";
				case TOKEN_REDIRECT_APPEND: return ">>";
				case TOKEN_REDIRECT_HEREDOC: return "<<";
				default: return "?";
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

				case TOKEN_WORD:
					printf("COMMAND\n");

					// Args
					if (node->args) {
						ast_print_indent(level + 1);
						printf("args:\n");
						t_args *arg = node->args;
						while (arg) {
							ast_print_indent(level + 2);
							printf("- \"%s\" %s\n", arg->value, 
								arg->quoted ? "(quoted)" : "");
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
							printf("- %s %s (fd: %d)\n", 
								token_type_to_string(redir->type),
								redir->file, redir->fd);
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
			ast_print_recursive(ast, 0);
		}

	#pragma endregion

	#pragma region "Create"

		t_ast *ast_create(t_token_type type) {
			t_ast *node = calloc(1, sizeof(t_ast));
			node->type = type;

			return (node);
		}

	#pragma endregion

#pragma endregion

#pragma region "Next token"

	void next_token() {
		token_free(g_parser->token);
		g_parser->token = token_next(&g_parser->lexer);
	}

#pragma endregion

#pragma region "Syntax Error"

	void syntax_error(const char *msg) {
		fprintf(stderr, "minishell: syntax error: %s\n", msg);

		if (g_parser && g_parser->token) {
			if (g_parser->token->filename)	fprintf(stderr, "  at %s:%d\n", g_parser->token->filename, g_parser->token->line);
			if (g_parser->token->full_line)	fprintf(stderr, "  %s\n", g_parser->token->full_line);
		}

		if (!g_parser->interactive) exit(1);
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

		next_token();
		t_ast *ast = parse_complete_command();
		token_free(g_parser->token);

		if (interactive) history_add(parser.lexer.full_input, 0);
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
