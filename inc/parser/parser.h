
#pragma once

#pragma region "Includes"

	#include "parser/lexer.h"
	#include "parser/ast.h"

#pragma endregion

#pragma region "Variables"

	#pragma region "Enumerators"

		typedef enum e_ast_type {
			AST_COMMAND,        // comando simple: echo hello
			AST_PIPE,           // pipe: cmd1 | cmd2
			AST_AND,            // and: cmd1 && cmd2
			AST_OR,             // or: cmd1 || cmd2
			AST_REDIRECT_IN,    // redirect input: cmd < file
			AST_REDIRECT_OUT,   // redirect output: cmd > file
			AST_REDIRECT_APPEND,// append: cmd >> file
			AST_SUBSHELL,       // subshell: $(cmd)
			AST_PARAM_EXP,      // parameter expansion: ${var}
			AST_WORD,           // palabra literal
		} t_ast_type;

	#pragma endregion

	#pragma region "Structures"

		typedef struct s_ast_node {
			t_ast_type			type;
			char				*value;
			int					quoted;
			struct e_ast_node	**argv;   // Array de nodos AST (palabras, subshells, expansiones)
			int					argc;
			
			// Para operadores binarios (pipe, &&, ||)
			struct e_ast_node *left;
			struct e_ast_node *right;
			
			// Para subshells y expansiones
			struct e_ast_node *child;
			
			// Para redirecciones
			struct e_ast_node *cmd;       // El comando a ejecutar
			struct e_ast_node *file;      // El archivo para la redirección
		} t_ast_node;

	#pragma endregion

#pragma endregion

#pragma region "Methods"

	t_ast_node *parse(void);

#pragma endregion
