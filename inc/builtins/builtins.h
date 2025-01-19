/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 20:59:56 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/19 18:27:30 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "terminal.h"
#include "builtins/options.h"

#pragma region Methods

	//	========== ALIAS ===========
	int			alias(t_arg *args);

	//	=========== TYPE ===========
	int			type(t_arg *args);

	//	========= COMMAND ==========
	int			command(t_arg *args);

	//	========= UNALIAS ==========
	int			unalias(t_arg *args);

	//	========= DECLARE ==========
	int			declare(t_arg *args);

	//	========= READONLY =========
	int			readonly(t_arg *args);

	//	========== EXPORT ==========
	int			export(t_arg *args);

	//	=========== HASH ===========
	int			hash(t_arg *args);

	//	=========== UNSET ==========
	int			unset(t_arg *args);

	//	========== ENABLE ==========
	int			enable(t_arg *args);

	//	============ FC ============

	int			builtin_exec(t_arg *args);

#pragma endregion
