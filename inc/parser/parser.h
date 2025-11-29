
#pragma once

#pragma region "Includes"

	#include "parser/lexer.h"

#pragma endregion

#pragma region "Variables"

	#pragma region "Structures"

		typedef struct s_ast {
			t_token_type	type;
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

	t_ast	*parse(char *input, char **full_input, t_callback callback);

	t_ast	*parse_command();

#pragma endregion
