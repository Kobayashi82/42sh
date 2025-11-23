/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 15:15:32 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/23 11:37:10 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/lexer.h"
#include "utils/libft.h"

t_lx_token	*lx_tokens;

int lexer(const char *input) {
	(void) input;
	return (LX_COMPLETE);
}

void lexer_print() {
	const char *type_names[] = { "WORD", "PIPE", "BACKGROND", "AND", "OR", "BRACE", "BRACE_GROUP", "VAR", "CMD_SHELL", "SHELL", "CMD_ARIT", "ARIT", "SEMICOLON", "REDIR_HEREDOC", "REDIR_IN", "REDIR_OUT", "REDIR_APPEND", "REDIR_HERESTRING" };
	t_lx_token *token = lx_tokens;

	while (token) {
		printf("[%s] %s\n", type_names[token->type], token->token);
		token = token->next;
	}
}
