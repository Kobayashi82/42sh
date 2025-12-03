/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 13:28:34 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/03 19:26:44 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Variables"

	#pragma region "Structures"

		typedef struct s_args {
			char			*value;
			int				quoted;	// Tengo que cambiarlo por un enum
			struct s_args	*prev;
			struct s_args	*next;
		} t_args;

		typedef struct s_redir {
			int				type;
			char			*file;
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
			// Comandos
			t_args			*args;
			t_redir			*redirs;
		} t_ast;

	#pragma endregion

#pragma endregion

#pragma region "Methods"

	void	ast_print(t_ast *ast);
	void	ast_free(t_ast **ast);
	t_ast	*ast_create(int type);
	const char *type_to_string(int type);

#pragma endregion
