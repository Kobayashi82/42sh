/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fc.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 21:00:36 by vzurera-          #+#    #+#             */
/*   Updated: 2025/01/19 18:29:02 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins/builtins.h"

// fc [-e ename] [first] [last]
//
// -e ENAME  select which editor to use.  Default is FCEDIT, then EDITOR, then vi
//
//	el comando propio (fc ...) no se añade al historial (o se elimina el comando y el evento se reduce en 1)
//	[ENAME] tiene que ser un programa válido (absoluto o relativo) (ojo, porque parece que no muestra programas interactivos, por ejemplo fc -e ./42sh echo)
//	si no se indica [ENAME] se usará la variable EDITOR, si no, VISUAL, si no, se buscará en "/usr/bin/editor" y por ultimo se usará "nano". Si todo eso falla, muestra un error
//	[first] [last] son números de eventos del comando
//	si [first] [last] no se indican, se utilizará el último comando
//	si [+first] [+last] se aplica offset a partir del primer comando
//	si [-first] [-last] se aplica offset a partir del último comando
//	si [first] solo, se aplica desde el último comando hasta [first]
//	si [first] es un string, se muestra desde el último comando hasta la primera coincidencia (empezando por la cadena)
//	si [first] [last] son strings, utiliza los rangos de las coincidencias. Siempre se empieza la busqueda desde el último comando independientemente de si es [first] o [last]

// fc -s [pat=rep] [command]
//
//	pueden haber varios reemplazos (fc -s a=b b=c c=d)
//	[command] es el número de evento del comando
//	si [command] no se indica, se utilizará el último comando
//	si [+command] se aplica offset a partir del primer comando
//	si [-command] se aplica offset a partir del último comando
//	si [command] es un string, se aplica la primera coincidencia empezando desde el último comando
//	

// fc [-lnr] [first] [last]
//
// -l        list lines instead of editing
// -n        omit line numbers when listing
// -r        reverse the order of the lines (newest listed first)
//
//	[-nr] no tienen efecto (se ignoran) si [-l] no está.
//	[first] [last] son números de eventos del comando
//	si [first] [last] no se indican, se utilizarán los últimos 16 comandos
//	si [+first] [+last] se aplica offset a partir del primer comando
//	si [-first] [-last] se aplica offset a partir del último comando
//	si [first] solo, se aplica desde el último comando hasta [first]
//	si [first] es un string, se muestra desde el último comando hasta la primera coincidencia (empezando por la cadena)
//	si [first] [last] son strings, utiliza los rangos de las coincidencias. Siempre se empieza la busqueda desde el último comando independientemente de si es [first] o [last]

// Nota:	si se juntan flags, tiene prioridad [--help], [--version], [-s], [-l], [-e]
//			pero si está [-e], por ejemplo (fc -sle vim 500) se ignora vim si existe un argumento más, si solo está vim, se considera [command]
