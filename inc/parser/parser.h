
#pragma once

#pragma region "Variables"

	#pragma region "Enumerators"

		typedef enum e_ast_type {
			AST_WORD,
			AST_CMD_SUB,				// $(
			AST_ARITH_SUB,				// $((
			AST_PARAM_EXP,				// ${
			AST_BRACE,					// {
			AST_SHELL,					// (
			AST_ARITH,					// ((

			AST_REDIRECT_IN,			// <	(accept fd number)
			AST_REDIRECT_HEREDOC,		// <<	(accept fd number)
			AST_REDIRECT_HERESTRING,	// <<<	(accept fd number)
			AST_REDIRECT_IN_OUT,		// <>	(accept fd number)
			AST_REDIRECT_DUP_IN,		// <&	(accept fd number)
			AST_REDIRECT_OUT,			// >	(accept fd number)
			AST_REDIRECT_APPEND,		// >>	(accept fd number)
			AST_REDIRECT_FORCE_OUT,		// >|	(accept fd number)
			AST_REDIRECT_DUP_OUT,		// >&	(accept fd number)
			AST_REDIRECT_OUT_ALL,		// &>
			AST_REDIRECT_APPEND_ALL,	// &>>
			AST_REDIRECT_PIPE_ALL,		// |&
			AST_REDIRECT_PIPE,			// |

			AST_BACKGROUND,				// &
			AST_AND,					// &&
			AST_OR,						// ||
			AST_SEMICOLON,				// ;

			AST_IF,						// 
			AST_THEN,					// 
			AST_ELSE,					// 
			AST_ELIF,					// 
			AST_FI,						// 

			AST_CASE,					// 
			AST_ESAC,					// 
			AST_IN,						// 
			AST_TERMINATOR,				// ;;
			AST_FALLTHROUGH,			// ;&
			AST_CONTINUE,				// ;;&

			AST_WHILE,					// 
			AST_UNTIL,					// 
			AST_DO,						// 
			AST_DONE,					// 

			AST_FOR,					// 
			AST_FUNCTION,				// 

			AST_NEGATE,					// !
			AST_LBRACKET2,				// [[
			AST_RBRACKET2,				// ]]

			AST_EOF,					// End of input
			AST_INPUT					// Needs more input

			// AST_COMMAND,			// comando simple: echo hello
			// AST_PIPE,				// pipe: cmd1 | cmd2
			// AST_AND,				// and: cmd1 && cmd2
			// AST_OR,					// or: cmd1 || cmd2
			// AST_REDIRECT_IN,		// redirect input: cmd < file
			// AST_REDIRECT_OUT,		// redirect output: cmd > file
			// AST_REDIRECT_APPEND,	// append: cmd >> file
			// AST_SUBSHELL,			// subshell: $(cmd)
			// AST_PARAM_EXP,			// parameter expansion: ${var}
			// AST_WORD,				// palabra literal
			// AST_EOF,				// end of input
		} t_ast_type;

	#pragma endregion

	#pragma region "Structures"

		typedef struct s_ast {
			t_ast_type		type;
			char			*value;
			int				quoted;
			char			**argv;   // Array de nodos AST (palabras, subshells, expansiones)
			int				argc;
			
			// Para operadores binarios (pipe, &&, ||)
			struct s_ast	*left;
			struct s_ast	*right;
			
			// Para subshells y expansiones
			struct s_ast	*child;
			
			// Para redirecciones
			char			*cmd;       // El comando a ejecutar
			char			*file;      // El archivo para la redirección
		} t_ast;

	#pragma endregion

#pragma endregion

#pragma region "Methods"

	void	ast_print(t_ast *node);
	void	ast_free(t_ast **ast);

	t_ast	*parse();

	t_ast	*parse_command();

#pragma endregion
