/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 12:14:29 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/24 13:03:17 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stddef.h>
// typedef enum e_lx_status { LX_COMPLETE, LX_INCOMPLETE, LX_FAILED } t_lx_status;
// typedef enum e_type { LX_WORD, LX_PIPE, LX_BACKGROND, LX_AND, LX_OR, LX_BRACE, LX_BRACE_GROUP, LX_VAR, LX_CMD_SHELL, LX_SHELL, LX_CMD_ARIT, LX_ARIT, LX_SEMICOLON, LX_REDIR_HEREDOC, LX_REDIR_IN, LX_REDIR_OUT, LX_REDIR_APPEND, LX_REDIR_HERESTRING } t_lx_type;

// typedef struct s_lexer {
// 	t_lx_type		type;
// 	t_lx_status		status;
// 	char			*token;
// 	struct s_lexer	*prev;
// 	struct s_lexer	*next;
// } t_lx_token;

// extern t_lx_token	*lx_tokens;

// int		lexer(const char *input);
// void	lexer_print();


// Tipos de tokens
typedef enum {
    TOKEN_WORD,
    TOKEN_SUBSHELL_START,   // $(
    TOKEN_SUBSHELL_END,     // )
    TOKEN_PARAM_EXP_START,  // ${
    TOKEN_PARAM_EXP_END,    // }
    TOKEN_PIPE,             // |
    TOKEN_REDIRECT_IN,      // <
    TOKEN_REDIRECT_OUT,     // >
    TOKEN_REDIRECT_APPEND,  // >>
    TOKEN_AND,              // &&
    TOKEN_OR,               // ||
    TOKEN_SEMICOLON,        // ;
    TOKEN_LPAREN,           // (
    TOKEN_RPAREN,           // )
    TOKEN_EOF
} token_type_t;

typedef struct {
    token_type_t type;
    char *value;
    int had_left_space;
    int had_right_space;
    int quoted;            // Si estaba entre comillas (para word splitting)
} token_t;

// Stack simple de delimitadores
typedef struct {
    char *input;
    size_t pos;
    size_t len;
    
    // Stack de delimitadores: '"', '\'', ')', '}'
    char *delim_stack;
    size_t stack_size;
    size_t stack_capacity;
    
    int	needs_more_input;
} lexer_state_t;

void lexer_init(char *input);
void lexer_append_input(char *more_input);
int lexer_needs_continuation(void);
void lexer_cleanup(void);
void token_free(token_t *tok);
token_t *lexer_next_token(void);
