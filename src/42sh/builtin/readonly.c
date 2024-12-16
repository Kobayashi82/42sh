/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readonly.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:06:39 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/16 12:06:58 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

//	READONLY
//
// readonly [-p] name[=value]
//
// 	compgen -v | while read var; do printf "%s=%q\n" "$var" "${!var}"; done
