/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grouping.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:30:52 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/23 11:33:27 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/lexer.h"
#include "utils/libft.h"

// GROUPING (recursive parsing)
//
// ()		subshell
// (())		arithmetic
// { ; }	command group (si hay espacio después), close command group (si hay ; o newline antes)
