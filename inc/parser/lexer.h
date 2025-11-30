/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 12:14:29 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/30 20:36:29 by vzurera-         ###   ########.fr       */
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
			TOKEN_GROUP,				// {[space]
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

			TOKEN_NEWLINE,				// New line
			TOKEN_EOF,					// End of input
		} t_token_type;

	#pragma endregion

	#pragma region "Structures"
		
		typedef char *(*t_callback)();

		typedef struct s_string {
			char			*value;
			size_t			len;
			size_t			capacity;
		} t_string;
		
		typedef struct s_token {
			t_token_type	type;
			char			*value;
			int				left_space;
			int				right_space;
			int				quoted;
			char			*filename;
			int				line;
			char			*full_line;
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
			t_buff			*user_buffer;	// Referencia al ultimo buffer de usuario
			int				interactive;
			int				append_inline;
			int				command_position;
			int				can_expand_alias;
			char			*filename;
			int				line;
			t_callback		more_input;
			char			*stack;
			size_t			stack_size;
			size_t			stack_capacity;
		} t_lexer;

	#pragma endregion

#pragma endregion

#pragma region "Methods"

	void	stack_push(t_lexer *lexer, char delim);
	char	stack_pop(t_lexer *lexer);
	char	stack_top(t_lexer *lexer);

	void	buffer_push(t_lexer *lexer, char *value, char *alias_name);
	void	buffer_push_user(t_lexer *lexer, char *value);
	void	buffer_pop(t_lexer *lexer);
	int		should_expand_alias(t_lexer *lexer, char *alias_name);

	void	string_init(t_string *string);
	void	string_append(t_string *string, char c);

	void	lexer_init(t_lexer *lexer, char *input, t_callback callback, int interactive, char *filename, int line);
	void	lexer_free(t_lexer *lexer);
	void	lexer_append(t_lexer *lexer);

	char	peek(t_lexer *lexer, size_t offset);
	char	advance(t_lexer *lexer);

	void	token_free(t_token *tok);
	t_token *token_create(t_lexer *lexer, t_token_type type, char *value, int line, char *full_line);
	t_token	*token_next(t_lexer *lexer);

	char	handle_quotes(t_lexer *lexer, t_string *string);

	t_token	*expansion(t_lexer *lexer);
	t_token	*grouping(t_lexer *lexer);
	t_token	*operator(t_lexer *lexer);
	t_token	*redirection(t_lexer *lexer);
	t_token	*keyword(t_lexer *lexer);
	t_token	*word(t_lexer *lexer);

#pragma endregion
