
#pragma once

#pragma region "Includes"

	#include "parser/lexer.h"

#pragma endregion

#pragma region "Variables"

	#pragma region "Structures"

		typedef struct s_args {
			char			*value;
			int				quoted;	// Tengo que cambiarlo por un enum
			struct s_args	*prev;
			struct s_args	*next;
		} t_args;

		typedef struct s_redir {
			t_token_type	type;
			char			*file;
			int				fd;
			int				expand;
			struct s_redir	*prev;
			struct s_redir	*next;
		} t_redir;

		typedef struct s_ast {
			t_token_type	type;
			char			*value;	// No necesario, realmente, aunque lo revisaré
			// Operators
			struct s_ast	*left;
			struct s_ast	*right;
			// Subshell
			struct s_ast	*child;
			// Comandos
			t_args			*args;
			t_redir			*redirs;
		} t_ast;

		typedef struct e_parser {
			t_lexer	lexer;
			t_token	*token;
			int		interactive;
		} t_parser;

	#pragma endregion

	extern t_parser *g_parser;

#pragma endregion

#pragma region "Methods"

	void	ast_print(t_ast *ast);
	void	ast_free(t_ast **ast);
	t_ast	*ast_create(t_token_type type);

	void	syntax_error(const char *msg);
	void	next_token();
	t_ast	*parse_complete_command();
	t_ast	*parse(char *input, t_callback callback, int interactive, char *filename, int line);

	t_ast	*parse_command();
	t_redir	*parse_redirect();
	void	redir_append(t_redir **list, t_redir *new_redir);

	t_ast	*parse_list();

#pragma endregion
