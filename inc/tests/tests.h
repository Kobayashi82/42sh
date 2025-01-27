/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 13:56:01 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/27 15:54:16 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Variables"

	#pragma region "Structures"

		typedef struct s_arg	t_arg;

	#pragma endregion

#pragma endregion

#pragma region "Methods"

	//	----------- ARGS -----------
	t_arg	*test_create_args(char *line);
	void	test_free_args(t_arg *args);

	//	---------- UNTEST ----------
	void	test_userinfo();
	void	test_globbing();

	//	--------- BUILTIN ----------
	int		test_builtins(const char **envp);

	//	--------- INTERNAL ---------
	int		test_aliases();
	int		test_variables(const char **envp);
	int		test_options();
	int		test_history();

	//	----------- TEST -----------
	int		tests(int argc, const char **argv, const char **envp);

#pragma endregion
