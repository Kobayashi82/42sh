/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dirs.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 10:47:22 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/17 18:52:28 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Variables"

	#pragma region "Structures"

		typedef struct e_dir_stack {
			char				*path;
			struct e_dir_stack	*prev;
			struct e_dir_stack	*next;
		} t_dir_stack;

		typedef struct e_dirs {
			char			*cwd;
			t_dir_stack		*stack;
		} t_dirs;

	#pragma endregion

#pragma endregion

#pragma region "Methods"

	char	*dirs_rotate(int offset);
	char	*dirs_add(char *path);
	char	*dirs_remove(int offset);
	void	dirs_clear();
	int		dirs_print(int offset, int line_mode, int no_tilde, int no_offset);
	void	dirs_initialize();

#pragma endregion


