/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vi.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 09:42:13 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/05 21:50:20 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#pragma region Vi

	int vi(int readed) {
		(void) readed;
		return (0);
	}

#pragma endregion

#pragma region Info

	// set -o emacs / vi

	// vi
	// --
	// #		añade # al principio de la línea y termina el input
	// h		cursor left
	// l, space	cursor right
	// w		cursor right start word
	// W		cursor right start bigword
	// e		cursor right end   word
	// E		cursor right end   bigword
	// b		cursor left  start word
	// B		cursor left  start bigword
	// x		delete char		(cut line really)
	// X		delete previous char	(cut line really)
	// d		elimina hasta donde se indique (0, ^, $, d) mirar manual para mas info		(cut line really)
	// D		elimina desde el carácter actual hasta el final de la linea			(cut line really)
	// p		paste
	// a		modo inserción después del cursor
	// i		modo inserción donde el cursor
	// A		modo inserción al final de la linea
	// I		modo inserción al principio de la linea
	// R		modo inserción pero hace algo raro
	// ^		mueve al primer carácter de la linea que no sea un isspace
	// $		mueve al final de la línea
	// 0		mueve al principio de la línea
	// |		mueve al carácter (1 por defecto)
	// f[c]		mueve al primer carácter después del cursor
	// F[c]		mueve al primer carácter antes del cursor
	// t		mueve al carácter anterior al primer carácter después del cursor
	// T		mueve al carácter posterior al primer carácter antes del cursor
	// ;		repite el comando mas reciente (f, F, t T)
	// ,		repite el comando mas reciente (f, F, t T) en modo reverse
	// C		delete from cursor to end of line
	// S		elimina la linea y entra en modo insercion
	// c		elimina hasta donde se indique (0, ^, $, c) mirar manual para mas info
	// r[c]		reemplaza el carácter actual por el indicado
	// y		copia hasta donde se indique (0, ^, $, c)
	// Y		copia hasta el final de la linea
	// p		pega lo copiado después del cursor
	// P		pega lo copiado antes del cursor
	// u		undo last change
	// U		undo all changes
	// [n] k		establece la linea por el comando del historial indicado
	// j		algo que ver con comandos y el historial
	// v		abre vi... y mas polleces

	// readline
	// --------
	// CTRL + b	cursor left
	// CTRL + f	cursor right
	// CTRL + p	history prev
	// CTRL + n	history next
	// CTRL + _	undo
	// CTRL + t	swap char
	// ALT + t		swap word


	// https://pubs.opengroup.org/onlinepubs/9699919799/utilities/sh.html#tag_20_117_13_04

#pragma endregion
