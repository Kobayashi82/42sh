/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 12:14:29 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/22 15:11:55 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

typedef enum e_lx_status { LX_COMPLETE, LX_INCOMPLETE, LX_FAILED } t_lx_status;
typedef enum e_type { LX_WORD, LX_PIPE, LX_AND, LX_OR, LX_BRACE, LX_VAR, LX_CMD_SHELL, LX_SHELL, LX_CMD_ARIT, LX_ARIT, LX_SEMICOLON, LX_REDIR_HEREDOC, LX_REDIR_IN, LX_REDIR_OUT, LX_REDIR_APPEND, LX_REDIR_HERESTRING } t_lx_type;

typedef struct s_lexer {
	t_lx_type		type;
	t_lx_status		status;
	char			*token;
	struct s_lexer	*prev;
	struct s_lexer	*next;
} t_lx_token;

extern t_lx_token	*lx_tokens;

int		lexer(const char *input);
void	lexer_print();