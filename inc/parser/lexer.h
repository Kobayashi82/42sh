/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 12:14:29 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/24 14:15:28 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stddef.h>

typedef enum e_token_type {
    TOKEN_WORD,
    TOKEN_CMD_SUB_START,		// $(
    TOKEN_CMD_SUB_END,			// )
	TOKEN_ARITH_SUB_START,		// $((
    TOKEN_ARITH_SUB_END,		// ))
    TOKEN_PARAM_EXP_START,		// ${
    TOKEN_PARAM_EXP_END,		// }
	TOKEN_BRACE_START,			// {
	TOKEN_BRACE_END,			// }
    TOKEN_SHELL_START,			// (
    TOKEN_SHELL_END,			// )
	TOKEN_ARITH_START,			// ((
    TOKEN_ARITH_END,			// ))

	TOKEN_REDIRECT_IN,			// <	(accept fd number)
	TOKEN_REDIRECT_HEREDOC,		// <<	(accept fd number)
	TOKEN_REDIRECT_HERESTRING,	// <<<	(accept fd number)
    TOKEN_REDIRECT_OUT,			// >	(accept fd number)
    TOKEN_REDIRECT_APPEND,		// >>	(accept fd number)
	TOKEN_REDIRECT_FORCE_OUT,	// >|	(accept fd number)
	TOKEN_REDIRECT_IN_OUT,		// <>	(accept fd number)
	TOKEN_REDIRECT_DUP_IN,		// <&	(accept fd number)
	TOKEN_REDIRECT_DUP_OUT,		// >&	(accept fd number)
	TOKEN_REDIRECT_OUT_ALL,		// &>
	TOKEN_REDIRECT_APPEND_ALL,	// &>>
	TOKEN_REDIRECT_PIPE_ALL,	// |&
	TOKEN_REDIRECT_PIPE,		// |

	TOKEN_BACKGROUND,			// &
    TOKEN_AND,					// &&
    TOKEN_OR,					// ||
	TOKEN_SEMICOLON,			// ;
    TOKEN_EOF
} t_token_type;

typedef struct s_token {
    t_token_type	type;
    char			*value;
    int				had_left_space;
    int				had_right_space;
    int				quoted;
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
