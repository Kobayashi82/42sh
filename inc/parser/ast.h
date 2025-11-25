/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 20:00:47 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/25 11:50:01 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Variables"

	#pragma region "Enumerators"

		typedef enum e_ast_type {
			AST_COMMAND,			// comando simple: echo hello
			AST_PIPE,				// pipe: cmd1 | cmd2
			AST_AND,				// and: cmd1 && cmd2
			AST_OR,					// or: cmd1 || cmd2
			AST_REDIRECT_IN,		// redirect input: cmd < file
			AST_REDIRECT_OUT,		// redirect output: cmd > file
			AST_REDIRECT_APPEND,	// append: cmd >> file
			AST_SUBSHELL,			// subshell: $(cmd)
			AST_PARAM_EXP,			// parameter expansion: ${var}
			AST_WORD,				// palabra literal
			AST_EOF,				// end of input
		} t_ast_type;

	#pragma endregion

	#pragma region "Structures"

		typedef struct s_ast_node {
			t_ast_type			type;
			char				*value;
			int					quoted;
			char **argv;   // Array de nodos AST (palabras, subshells, expansiones)
			int					argc;
			
			// Para operadores binarios (pipe, &&, ||)
			struct s_ast_node *left;
			struct s_ast_node *right;
			
			// Para subshells y expansiones
			struct s_ast_node *child;
			
			// Para redirecciones
			char *cmd;       // El comando a ejecutar
			char *file;      // El archivo para la redirección
		} t_ast_node;

	#pragma endregion

#pragma endregion

void ast_print(t_ast_node *node);
void ast_free(t_ast_node **ast);
