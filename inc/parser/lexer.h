/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 12:14:29 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/29 00:12:34 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include <stddef.h>

#pragma endregion

#pragma region "Variables"

	#pragma region "Enumerators"

		typedef enum e_token_type {
			TOKEN_WORD,
			TOKEN_CMD_SUB,				// $(
			TOKEN_ARITH_SUB,			// $((
			TOKEN_PARAM_EXP,			// ${
			TOKEN_BRACE,				// {
			TOKEN_SHELL,				// (
			TOKEN_ARITH,				// ((

			TOKEN_REDIRECT_IN,			// <	(accept fd number)
			TOKEN_REDIRECT_HEREDOC,		// <<	(accept fd number)
			TOKEN_REDIRECT_HERESTRING,	// <<<	(accept fd number)
			TOKEN_REDIRECT_IN_OUT,		// <>	(accept fd number)
			TOKEN_REDIRECT_DUP_IN,		// <&	(accept fd number)
			TOKEN_REDIRECT_OUT,			// >	(accept fd number)
			TOKEN_REDIRECT_APPEND,		// >>	(accept fd number)
			TOKEN_REDIRECT_FORCE_OUT,	// >|	(accept fd number)
			TOKEN_REDIRECT_DUP_OUT,		// >&	(accept fd number)
			TOKEN_REDIRECT_OUT_ALL,		// &>
			TOKEN_REDIRECT_APPEND_ALL,	// &>>
			TOKEN_REDIRECT_PIPE_ALL,	// |&
			TOKEN_REDIRECT_PIPE,		// |

			TOKEN_BACKGROUND,			// &
			TOKEN_AND,					// &&
			TOKEN_OR,					// ||
			TOKEN_SEMICOLON,			// ;

			TOKEN_IF,					// 
			TOKEN_THEN,					// 
			TOKEN_ELSE,					// 
			TOKEN_ELIF,					// 
			TOKEN_FI,					// 

			TOKEN_CASE,					// 
			TOKEN_ESAC,					// 
			TOKEN_IN,					// 
			TOKEN_TERMINATOR,			// ;;
			TOKEN_FALLTHROUGH,			// ;&
			TOKEN_CONTINUE,				// ;;&

			TOKEN_WHILE,				// 
			TOKEN_UNTIL,				// 
			TOKEN_DO,					// 
			TOKEN_DONE,					// 

			TOKEN_FOR,					// 
			TOKEN_FUNCTION,				// 

			TOKEN_NEGATE,				// !
			TOKEN_LBRACKET2,			// [[
			TOKEN_RBRACKET2,			// ]]

			TOKEN_EOF,					// End of input
			TOKEN_INPUT					// Needs more input
		} t_token_type;

	#pragma endregion

	#pragma region "Structures"

		typedef struct s_token {
			t_token_type	type;
			char			*value;
			int				left_space;
			int				right_space;
			int				quoted;
		} t_token;
	
		typedef char *(*t_input_callback)();

		typedef struct s_lexer {
			char				*input;
			size_t				pos;
			size_t				len;
			int					append_inline;
			t_input_callback	more_input;
			char				*stack;
			size_t				stack_size;
			size_t				stack_capacity;
		} t_lexer;

	#pragma endregion

	extern t_lexer lexer;

#pragma endregion

#pragma region "Methods"

	void	lexer_init(char *input, t_input_callback callback);
	void	lexer_free();
	void	lexer_append_input();
	
	void	token_free(t_token *tok);
	t_token *token_create(t_token_type type, size_t start);
	t_token	*token_next();
	
	void	stack_push(char delim);
	char	stack_pop();
	char	stack_top();

	char	peek(size_t n);
	char	peek_back(size_t n);
	char	advance(size_t n);

	char	handle_quotes();
	int		is_space(int n);

	t_token	*expansion();
	t_token	*grouping();
	t_token	*operator();
	t_token	*redirection();
	t_token	*keyword();
	t_token	*word();

#pragma endregion
