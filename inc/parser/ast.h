/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 13:28:34 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/09 21:08:58 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "parser/lexer.h"

#pragma region "Variables"

	#pragma region "Structures"

		typedef struct s_assign {
			t_segment		*segment;
			int				right_space;
			struct s_assign	*prev;
			struct s_assign	*next;
		} t_assign;

		typedef struct s_args {
			t_segment		*segment;
			int				right_space;
			struct s_args	*prev;
			struct s_args	*next;
		} t_args;

		typedef struct s_redir {
			int				type;
			t_segment		*file;
			int				fd;
			int				expand;
			struct s_redir	*prev;
			struct s_redir	*next;
		} t_redir;

		typedef struct s_ast {
			int				type;
			char			*value;	// No necesario, realmente, aunque lo revisaré
			// Operators
			struct s_ast	*left;
			struct s_ast	*right;
			// Subshell
			struct s_ast	*child;
			// Commands
			t_assign		*assign;
			t_args			*args;
			t_redir			*redirs;
		} t_ast;

	#pragma endregion

#pragma endregion

#pragma region "Methods"

	void	ast_print(t_ast *ast);
	void	ast_free(t_ast **ast);
	t_ast	*ast_create(int type);
	char	**get_argv_from_ast(t_ast *ast, int *argc);
	
	const char *type_to_string(int type);	// DEBUG

#pragma endregion
