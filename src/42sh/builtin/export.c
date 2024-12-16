/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:06:34 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/16 12:07:16 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

//	EXPORT
//
// 	export c	// el value es NULL y por lo tanto no se exportará
//
// 			Escapado de values
//
// "	Para que las comillas dobles no terminen la cadena prematuramente.
// \	Siempre se escapa para preservar su literalidad.
// $	Para evitar la expansión de variables.
// `	Para evitar que se interprete como un comando (backtick).
// !	Escapado para prevenir la expansión del historial en configuraciones donde el historial está habilitado.
