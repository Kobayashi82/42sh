
#pragma once

#pragma region "Includes"

	#include "parser/lexer.h"
	#include "parser/ast.h"

#pragma endregion

#pragma region "Variables"

	#pragma region "Enumerators"

		typedef enum e_parser_state {
			PARSER_INPUT,
			PARSER_COMPLETED,
			PARSER_ERROR,
		} t_parser_state;

	#pragma endregion

#pragma endregion

#pragma region "Methods"

	t_parser_state parse(t_ast_node **);

#pragma endregion
