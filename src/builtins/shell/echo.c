/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:08:56 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/24 19:39:44 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "terminal/print.h"
	#include "parser/args.h"
	#include "builtins/builtins.h"
	#include "builtins/options.h"

#pragma endregion

#pragma region "Echo"

	int echo(t_arg *args) {
		args_print(args);
		return (0);
	}

#pragma endregion
