/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 13:56:01 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/23 23:06:56 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region Methods

	t_arg	*test_create_args(char *line);
	void	test_free_args(t_arg *args);

	void	test_userinfo();

	int		test_aliases();
	int		test_variables(const char **envp);
	int		test_options();
	int		test_history();

	int		test_builtin(const char **envp);

	int		tests(int argc, const char **argv, const char **envp);

	extern char **environ;

#pragma endregion
