/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 17:30:16 by vzurera-          #+#    #+#             */
/*   Updated: 2024/12/20 19:36:28 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"




//	TOKENIZER
//
//	Recorre la entrada y busca alias para cada primer comando despues de un operador	| && || ; & $( ` {
//	Comprueba sintaxis
//	Se vuelve a analizar la cadena separando comandos en base a los operadores			&& || ; &

//	LEXER
//
//	Asignaciones y redirecciones se sacan aparte										<> < > << >> <<< |& n> n>> n>&m n<&m &> n<&- 		>n tambien sirve, por ejemplo
//	Por cada comando se separan las palabras											" ' $ $() ${} $(()) $" $' () {} (())

//	REDIRECCIONES
//
//	<		Redirige la entrada estándar (stdin) desde un archivo.
//	>		Redirige la salida estándar (stdout) a un archivo (sobrescribe).
//	>>		Redirige la salida estándar (stdout) a un archivo (añade al final).
//	<>		Abre un archivo para lectura y escritura simultáneamente.
//	<<		Heredoc: Usa texto literal como entrada estándar (stdin).
//	<<<		Here-string: Usa una cadena como entrada estándar (stdin).

//	n< o <n	Redirige al descriptor n un archivo.
//	n>		Redirige el descriptor n a un archivo (sobrescribe).
//	n>>		Redirige el descriptor n a un archivo (añade al final).
//	n<>		Abre un archivo en modo bidireccional (lectura y escritura).
//	n>&m	Redirige el descriptor n al mismo destino que el descriptor m.
//	n<&m	Redirige el descriptor n para leer desde el mismo origen que m.
//	n<&-	Cierra el descriptor n para lectura.
//	n>&-	Cierra el descriptor n para escritura.

//	&>		Redirige tanto stdout como stderr a un archivo (sobrescribe).


//	PARSER
//	
//	


// Tilde expansion (~) ocurre antes de la expansión de variables.
// Globbing (*, ?, [...]) ocurre después de la expansión de variables