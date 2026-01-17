/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dirs.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 10:47:22 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/17 13:39:21 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Variables"

	#pragma region "Structures"

		typedef struct e_dir_stack {
			char			*path;
			struct e_dirs	*prev;
			struct e_dirs	*next;
		} t_dir_stack;

		typedef struct e_dirs {
			char			*cwd;
			t_dir_stack		*stack;
		} t_dirs;

	#pragma endregion

#pragma endregion

#pragma region "Methods"

	char	*dirs_rotate(int n);
	char	*dirs_add(char *path);
	void	dirs_remove(int n);
	void	dirs_clear();
	void	dirs_print(int n, int line_mode, int no_tilde);
	void	dirs_initialize();

#pragma endregion


