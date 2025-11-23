/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 11:30:41 by vzurera-          #+#    #+#             */
/*   Updated: 2025/11/23 11:33:28 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/lexer.h"
#include "utils/libft.h"

// EXPANSION (recursive parsing)
//
// ``		command substitution
// $()		command substitution
// $(())	arithmetic substitution
// ${}		parameter expression
