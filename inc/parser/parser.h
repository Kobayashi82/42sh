
#pragma once

#pragma region "Includes"

	#include "parser/ast.h"
	#include "parser/lexer.h"

#pragma endregion

#pragma region "Variables"

	#pragma region "Structures"

		typedef struct e_parser {
			t_lexer	lexer;
			t_token	*token;
			int		interactive;
		} t_parser;

	#pragma endregion

	extern t_parser *g_parser;

#pragma endregion

#pragma region "Methods"

	void	syntax_error(const char *msg);
	t_token	*token_advance();
	t_ast	*parse_complete_command();
	t_ast	*parse(char *input, t_callback callback, int interactive, char *filename, int line);

	t_ast	*parse_command();
	t_redir	*parse_redirect();
	void	redir_append(t_redir **list, t_redir *new_redir);

	t_ast	*parse_list();

#pragma endregion
