/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:08:17 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/21 22:09:11 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Aplicar redirecciones permanentes al proceso actual (sin crear un subshell).
// Reemplazar el proceso actual por otro comando (cuando se usa con un comando explícito).

#pragma region "Includes"

	#include "terminal/terminal.h"
	#include "builtins/builtins.h"
	#include "builtins/options.h"

#pragma endregion
