/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:38:28 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/19 19:35:32 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "parser/parser.h"
	#include "utils/libft.h"

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

		void redir_create(t_redir **redirs, int type, int fd, t_segment *file) {
			t_redir *new_redir = malloc(sizeof(t_redir));
			new_redir->type = type;
			new_redir->file = file;
			new_redir->fd = fd;

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

		int parse_redirect(t_redir **redirs, t_args **args) {
			int type = g_parser->token->type;
			int fd = -1;
			if (type >= TOKEN_REDIRECT_IN && type <= TOKEN_REDIRECT_FORCE_OUT) {
				if (args && *args) {
					t_args *curr = *args;
					while (curr->next)
						curr = curr->next;
					if (!curr->right_space && ft_isnum_s(curr->segment->string.value)) {
						fd = atoi(curr->segment->string.value);
						if (curr->prev)	curr->prev->next = NULL;
						else			*args = NULL;
						segment_free(curr->segment);
						free(curr);
					}
				}
			}

			token_advance();

			if (g_parser->token->type != TOKEN_WORD && g_parser->token->type != TOKEN_PROCESS_SUB_IN && g_parser->token->type != TOKEN_PROCESS_SUB_OUT) return (1);

			t_segment *file = g_parser->token->segment;
			g_parser->token->segment = NULL;

			redir_create(redirs, type, fd, file);

			if (type == TOKEN_REDIRECT_HEREDOC) {
				// Procesar heredoc
			}

			return (0);
		}

	#pragma endregion

#pragma endregion

#pragma region "Assign Create"

	void assign_create(t_assign **assign, t_segment *segment) {
		t_assign *new_assign = malloc(sizeof(t_assign));
		new_assign->segment = segment;
		new_assign->right_space = 0;

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

	void args_create(t_args **args, t_segment *segment, int right_space) {
		t_args *new_arg = malloc(sizeof(t_args));
		new_arg->segment = segment;
		new_arg->right_space = right_space;

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

		int is_word = (g_parser->token->type >= TOKEN_WORD && g_parser->token->type <= TOKEN_PARAM_EXP);
		while (is_word || is_redirect(g_parser->token->type)) {
			if (is_word) {
				char *equal = strchr(g_parser->token->segment->string.value, '=');
				if (g_parser->token->type == TOKEN_WORD && !cmd_found && equal && equal != g_parser->token->segment->string.value)
					assign_create(&node->assign, g_parser->token->segment);
				else {
					cmd_found = 1;
					args_create(&node->args, g_parser->token->segment, g_parser->token->right_space);
				}
				g_parser->token->segment = NULL;
			}
			else {
				if (parse_redirect(&node->redirs, &node->args)) {
					syntax_error(0, "archivo necesario...");
					ast_free(&node);
					return (NULL);
				}
			}

			token_advance();
			is_word = (g_parser->token->type >= TOKEN_WORD && g_parser->token->type <= TOKEN_PARAM_EXP);
		}

		if (g_parser->token->type != TOKEN_EOF && !node->assign && !node->args && !node->redirs) {
			syntax_error(1, "expected command");
			ast_free(&node);
			return (NULL);
		}

		if (stack_top(&g_parser->lexer) && !g_parser->lexer.input) {
			syntax_error(1, stack_get(&g_parser->lexer));
			ast_free(&node);
			return (NULL);
		}

		if (g_parser->token->type == TOKEN_SUBSHELL || g_parser->token->type == TOKEN_PROCESS_SUB_IN) {
			syntax_error(1, "(");
			ast_free(&node);
			return (NULL);
		}

		return (node);
	}

#pragma endregion

// | Operator | Syntax     | Left FD         | Right FD | File  | Description                          | Example                       |
// |----------|------------|-----------------|----------|-------|--------------------------------------|-------------------------------|
// | `<`      | [n]<file   | YES (default 0) | NO       | YES   | Redirect input from file             | command < file 3< file        |
// | `>`      | [n]>file   | YES (default 1) | NO       | YES   | Redirect output to file (overwrite)  | command > file 3> file        |
// | `>>`     | [n]>>file  | YES (default 1) | NO       | YES   | Redirect output to file (append)     | command >> file 3>> file      |
// | `<<`     | [n]<<DELIM | YES (default 0) | NO       | DELIM | Here document (multi-line text)      | command <<EOF 3<<EOF          |
// | `<<<`    | [n]<<<text | YES (default 0) | NO       | TEXT  | Here string (single-line text)       | grep foo <<<"text" 3<<<"text" |
// | `<>`     | [n]<>file  | YES (default 0) | NO       | YES   | Open file for read/write             | exec 3<> file                 |
// | `<&`     | [n]<&m     | YES (default 0) | YES (-)  | NO    | Duplicate input fd from m to n       | 3<&0 exec 4<&3                |
// | `>&`     | [n]>&m     | YES (default 1) | YES (-)  | NO    | Duplicate output fd from m to n      | 2>&1 3>&2                     |
// | `>|`     | [n]>|file  | YES (default 1) | NO       | YES   | Overwrite ignoring noclobber         | command >| file 3>| file      |
// | `&>`     | &>file     | NO              | NO       | YES   | Redirect stdout + stderr (overwrite) | command &> file               |
// | `&>>`    | &>>file    | NO              | NO       | YES   | Redirect stdout + stderr (append)    | command &>> file              |
