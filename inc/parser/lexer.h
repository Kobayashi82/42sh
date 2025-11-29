/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 12:14:29 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/29 14:24:30 by vzurera-         ###   ########.fr       */
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
		
		typedef char *(*t_callback)();

		typedef struct s_token {
			t_token_type	type;
			char			*value;
			int				left_space;
			int				right_space;
			int				quoted;
		} t_token;

		typedef struct s_buff {
			char			*value;
			size_t			position;
			char			*alias_name;
			int				is_user_input;
			struct s_buff	*next;
		} t_buff;

		typedef struct s_lexer {
			char			*full_input;
			t_buff			*input;
			t_buff			*base_buffer;	// Referencia al ultimo buffer de usuario
			int				append_inline;
			t_callback		more_input;
			char			*stack;
			size_t			stack_size;
			size_t			stack_capacity;
		} t_lexer;

	#pragma endregion

	extern t_lexer lexer;

#pragma endregion

#pragma region "Methods"

	void	lexer_init(char *input, t_callback callback);
	void	lexer_free();
	void	lexer_append_input();
	
	void	buffer_push(char *value, char *alias_name);
	void	buffer_push_user(char *value);
	void	buffer_pop();
	int		is_alias_expanding(char *alias_name);
	
	void	token_free(t_token *tok);
	t_token *token_create(t_token_type type, size_t start);
	t_token	*token_next();
	
	void	stack_push(char delim);
	char	stack_pop();
	char	stack_top();

	char	peek(size_t offset);
	char	advance(size_t offset);

	char	handle_quotes();
	int		is_space(int n);

	t_token	*expansion();
	t_token	*grouping();
	t_token	*operator();
	t_token	*redirection();
	t_token	*keyword();
	t_token	*word();

#pragma endregion
