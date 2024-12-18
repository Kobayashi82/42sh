/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 13:56:01 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/18 19:16:51 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region Methods

	void	test_userinfo();

	int		test_built_alias();
	int		test_built_export();

	int		test_alias();
	int		test_variables(const char **envp);
	int		test_options();
	int		test_history();

	int		tests(int argc, const char **argv, const char **envp);

#pragma endregion
