/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 12:14:29 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/10 00:09:38 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Includes"

	#include <stddef.h>

#pragma endregion

#pragma region "Variables"

	#pragma region "Enumerators"

		enum e_token_type {
			TOKEN_WORD,
			TOKEN_BACKTICK,				// `
			TOKEN_ARITH,				// ((
			TOKEN_ARITH_SUB,			// $((
			TOKEN_SUBSHELL,				// (
			TOKEN_CMD_SUB,				// $(
			TOKEN_PROCESS_SUB_IN,		// <(
			TOKEN_PROCESS_SUB_OUT,		// >(
			TOKEN_GROUP,				// {[isspace]
			TOKEN_CONDITIONAL,			// [[
			TOKEN_PARAM_EXP,			// ${
			TOKEN_BRACE,				// {

			TOKEN_REDIRECT_IN,			// <	(accept fd number)
			TOKEN_REDIRECT_OUT,			// >	(accept fd number)
			TOKEN_REDIRECT_HEREDOC,		// <<	(accept fd number)
			TOKEN_REDIRECT_APPEND,		// >>	(accept fd number)
			TOKEN_REDIRECT_HERESTRING,	// <<<	(accept fd number)
			TOKEN_REDIRECT_IN_OUT,		// <>	(accept fd number)
			TOKEN_REDIRECT_DUP_IN,		// <&	(accept fd number)
			TOKEN_REDIRECT_DUP_OUT,		// >&	(accept fd number)
			TOKEN_REDIRECT_FORCE_OUT,	// >|	(accept fd number)
			TOKEN_REDIRECT_OUT_ALL,		// &>
			TOKEN_REDIRECT_APPEND_ALL,	// &>>

			TOKEN_PIPE,					// |
			TOKEN_PIPE_ALL,				// |&
			TOKEN_BACKGROUND,			// &
			TOKEN_AND,					// &&
			TOKEN_OR,					// ||
			TOKEN_SEMICOLON,			// ;
			
			TOKEN_IF,					// if
			TOKEN_THEN,					// then
			TOKEN_ELSE,					// else
			TOKEN_ELIF,					// elif
			TOKEN_FI,					// fi
			
			TOKEN_FUNCTION,				// function
			TOKEN_CASE,					// case
			TOKEN_ESAC,					// esac
			TOKEN_IN,					// in
			TOKEN_TERMINATOR,			// ;;
			TOKEN_FALLTHROUGH,			// ;&
			TOKEN_CONTINUE,				// ;;&

			TOKEN_WHILE,				// while
			TOKEN_UNTIL,				// until
			TOKEN_DO,					// do
			TOKEN_DONE,					// done
			TOKEN_FOR,					// for
			TOKEN_NEGATE,				// !
			TOKEN_LBRACKET2,			// [[
			TOKEN_RBRACKET2,			// ]]

			TOKEN_NEWLINE,				// New line
			TOKEN_EOF,					// End of input
			TOKEN_ERROR,				// Error
		};

	#pragma endregion

	#pragma region "Structures"

		#pragma region "Token"
			
			typedef struct s_token {
				int				type;
				char			*value;
				int				right_space;
				char			*full_line;
				char			*filename;
				int				line;
			} t_token;

		#pragma endregion

		#pragma region "Stack"
		
			typedef struct s_stack {
				char			*value;
				size_t			len;
				size_t			capacity;
			} t_stack;

		#pragma endregion

		#pragma region "String"
		
			typedef struct s_string {
				char			*value;
				size_t			len;
				size_t			capacity;
			} t_string;

		#pragma endregion

		#pragma region "Buffer"

			typedef struct s_buff {
				char			*value;
				size_t			position;
				char			*alias_name;
				int				is_user_input;
				struct s_buff	*next;
			} t_buff;

		#pragma endregion

		#pragma region "Lexer"

			typedef char *(*t_callback)();
			typedef struct s_lexer {
				char			*full_input;
				t_buff			*input;
				t_buff			*user_buffer;
				int				interactive;
				int				append_inline;
				int				right_space;
				int				command_position;
				int				can_expand_alias;
				char			*filename;
				int				line;
				t_callback		more_input;
				t_stack			stack;
			} t_lexer;

		#pragma endregion

	#pragma endregion

#pragma endregion

#pragma region "Methods"

	// Containers
	char	*stack_get(t_lexer *lexer);
	char	stack_top(t_lexer *lexer);
	char	stack_pop(t_lexer *lexer);
	void	stack_push(t_lexer *lexer, char c);

	void	buffer_pop(t_lexer *lexer);
	void	buffer_push(t_lexer *lexer, char *value, char *alias_name);
	void	buffer_push_user(t_lexer *lexer, char *value);

	void	string_append(t_string *string, char c);
	void	string_init(t_string *string);

	// Token
	void	token_free(t_token *token);
	t_token *token_create(t_lexer *lexer, int type, char *value, int line, char *full_line);
	t_token	*token_get(t_lexer *lexer);

	// Input
	void	lexer_free(t_lexer *lexer);
	void	lexer_append(t_lexer *lexer);
	void	lexer_init(t_lexer *lexer, char *input, t_callback callback, int interactive, char *filename, int line);

	// Navigation
	char	peek(t_lexer *lexer, size_t offset);
	char	advance(t_lexer *lexer);

	// Utils
	int		handle_quotes(t_lexer *lexer, t_string *string);
	char	handle_quotes2(t_lexer *lexer, t_string *string);
	int		is_operator(char c);

	// Expansion
	t_token *variables(t_lexer *lexer);
	t_token	*expansion(t_lexer *lexer);

	// Operator
	t_token	*operator(t_lexer *lexer);

	// Redirection
	t_token	*redirection(t_lexer *lexer);

	// Keyword
	t_token	*keyword(t_lexer *lexer);

	// Word
	t_token	*word(t_lexer *lexer);

#pragma endregion
