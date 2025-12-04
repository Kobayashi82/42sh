/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:38:14 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/04 15:36:19 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "parser/parser.h"
	#include "utils/libft.h"

#pragma endregion

#pragma region "Redirect Append"

	void redir_append(t_redir **list, t_redir *new_redir) {
		if (!new_redir) return;

		if (!*list) {
			*list = new_redir;
			new_redir->prev = NULL;
			new_redir->next = NULL;
			return;
		}

		t_redir *curr = *list;
		while (curr->next)
			curr = curr->next;

		curr->next = new_redir;
		new_redir->prev = curr;
		new_redir->next = NULL;
	}

#pragma endregion

#pragma region "Redirect Create"

	t_redir *redir_create(int type, char *file, int fd) {
		t_redir *redir = malloc(sizeof(t_redir));
		redir->type = type;
		redir->file = file;
		redir->fd = fd;
		redir->prev = NULL;
		redir->next = NULL;
		return (redir);
	}

#pragma endregion

#pragma region "Parse Redirect"

	t_redir *parse_redirect() {
		int redir_type = g_parser->token->type;
		int fd = 1; // Por defecto stdout, pero hay que poner el fd si se indica antes de la redireccion o despues (quizas dos si hay que duplicar)

		token_advance();

		if (g_parser->token->type != TOKEN_WORD) syntax_error("archivo necesario... no todas las redirects usan archivo o fd... creo");

		char *file = g_parser->token->value;
		g_parser->token->value = NULL;
		token_advance();

		return (redir_create(redir_type, file, fd));
	}

#pragma endregion
