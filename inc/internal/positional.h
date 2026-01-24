/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   positional.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 15:39:40 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/24 15:41:06 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Variables"

	#pragma region "Structures"

		typedef struct s_env t_env;

	#pragma endregion

#pragma endregion

#pragma region "Methods"

	int		positional_params_shift(t_env *env, int offset);
	void	positional_params_clear(t_env *env);
	int		positional_params_set(t_env *env, int argc, char **argv);
	void	positional_params_print(t_env *env, int index);

#pragma endregion
