/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 20:59:56 by vzurera-          #+#    #+#             */
/*   Updated: 2025/03/12 17:27:55 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#pragma region "Variables"

	#pragma region "Structures"

		typedef struct s_arg	t_arg;

	#pragma endregion

#pragma endregion

#pragma region "Methods"

	//	========== ALIAS ===========
	int		alias(t_arg *args);

	//	=========== ECHO ===========
	int		echo(t_arg *args);

	//	========== BANNER ==========
	int		banner();
	void	welcome();

	//	=========== TYPE ===========
	int		type(t_arg *args);

	//	========= COMMAND ==========
	int		command(t_arg *args);

	//	========= UNALIAS ==========
	int		unalias(t_arg *args);

	//	========= DECLARE ==========
	int		declare(t_arg *args);

	//	========= READONLY =========
	int		readonly(t_arg *args);

	//	========== SHOPT ===========
	int		shopt(t_arg *args);

	//	========== EXPORT ==========
	int		export(t_arg *args);

	//	========= BUILTIN ==========
	int		bt_builtin(t_arg *args);

	//	=========== EXIT ===========
	int		bt_exit(t_arg *args);

	//	=========== HASH ===========
	int		hash(t_arg *args);

	//	============ CD ============
	int		cd(t_arg *args);

	//	=========== PWD ============
	int		pwd(t_arg *args);

	//	=========== UNSET ==========
	int		unset(t_arg *args);

	//	========== ENABLE ==========
	int		enable(t_arg *args);

	//	============ FC ============

	int		builtin_exec(t_arg *args);

#pragma endregion
