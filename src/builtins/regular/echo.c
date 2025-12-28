/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:08:56 by vzurera-          #+#    #+#             */
/*   Updated: 2025/12/29 00:30:38 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "utils/print.h"
	#include "tests/args.h"
	#include "hashes/builtin.h"
	#include "builtins/options.h"

#pragma endregion

#pragma region "Echo"

	int bt_echo(t_arg *args) {
		args_print(args);
		return (0);
	}

#pragma endregion
